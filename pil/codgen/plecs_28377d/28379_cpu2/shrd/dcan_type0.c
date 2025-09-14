/*
   Copyright (c) 2020-2022 by Plexim GmbH
   All rights reserved.

   A free license is granted to anyone to use this software for any legal
   non safety-critical purpose, including commercial applications, provided
   that:
   1) IT IS NOT USED TO DIRECTLY OR INDIRECTLY COMPETE WITH PLEXIM, and
   2) THIS COPYRIGHT NOTICE IS PRESERVED in its entirety.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#include "plx_canbus.h"

PLX_CANBUS_Handle_t PLX_CANBUS_init(void *aMemory, const size_t aNumBytes) {
  if (aNumBytes < sizeof(PLX_CANBUS_Obj_t)) {
    return ((PLX_CANBUS_Handle_t)NULL);
  }
  PLX_CANBUS_Handle_t handle = (PLX_CANBUS_Handle_t)aMemory;
  return handle;
}

void PLX_CANBUS_configure(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_Unit_t aUnit,
                          const PLX_CANBUS_Params_t *aParams) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  EALLOW;
  switch (aUnit) {
#ifdef CANA_BASE
    case PLX_CANBUS_CAN_A:
      obj->portHandle = CANA_BASE;

      CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
      ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = 0;  // 0 = SYSCLK
      break;
#endif
#ifdef CANB_BASE
    case PLX_CANBUS_CAN_B:
      obj->portHandle = CANB_BASE;

      CpuSysRegs.PCLKCR10.bit.CAN_B = 1;
      ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = 0;  // 0 = SYSCLK
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
  EDIS;

  CAN_initModule(obj->portHandle);
  {
    uint16_t brr = aParams->brp - 1;
    CAN_setBitTiming(obj->portHandle, (brr & 0x3F), (brr >> 6),
                     aParams->tseg1 - 1, aParams->tseg2 - 1, aParams->sjw - 1);
  }

  if (aParams->autoBusOn) {
    CAN_enableAutoBusOn(obj->portHandle);
    CAN_setAutoBusOnTime(obj->portHandle, 1000);  // number of clock cycles
  } else {
    CAN_disableAutoBusOn(obj->portHandle);  // note: if enabling, must also
                                            // PLX_CANBUS_setAutoBusTime
  }

  CAN_enableRetry(obj->portHandle);
  CAN_startModule(obj->portHandle);  // clears INIT and CCE bits

  //    CAN_enableController(obj->portHandle);
}

void PLX_CANBUS_setupMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                             bool aConfigureForTransmit, uint32_t aId,
                             bool isExtended, unsigned char aLen) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  CAN_MsgFrameType frame;
  uint32_t filter;

  if (isExtended) {
    frame = CAN_MSG_FRAME_EXT;
    filter = CAN_MSG_OBJ_USE_ID_FILTER | CAN_MSG_OBJ_USE_EXT_FILTER;
  } else {
    frame = CAN_MSG_FRAME_STD;
    filter = CAN_MSG_OBJ_USE_ID_FILTER;
  }

  if (aConfigureForTransmit) {
    CAN_setupMessageObject(obj->portHandle, aMailBox + 1, aId, frame,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, aLen);
  } else {
    CAN_setupMessageObject(obj->portHandle, aMailBox + 1, aId, frame,
                           CAN_MSG_OBJ_TYPE_RX, 0x1FFFFFFF, filter,
                           aLen);  // strict filtering; obj->rxLen does not seem
                                   // to affect filtering!
  }
}

void PLX_CANBUS_disableMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  CAN_clearMessage(obj->portHandle, aMailBox + 1);
}

bool PLX_CANBUS_isBusOn(PLX_CANBUS_Handle_t aHandle) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  if ((CAN_getStatus(obj->portHandle) & CAN_STATUS_BUS_OFF) != 0) {
    return false;
  } else {
    return ((HWREGH(obj->portHandle + CAN_O_CTL) & CAN_CTL_INIT) == 0);
  }
}

bool PLX_CANBUS_isErrorActive(PLX_CANBUS_Handle_t aHandle) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  if ((CAN_getStatus(obj->portHandle) & CAN_STATUS_EPASS) != 0) {
    return false;
  } else {
    return ((HWREGH(obj->portHandle + CAN_O_CTL) & CAN_CTL_INIT) == 0);
  }
}

void PLX_CANBUS_setBusOn(PLX_CANBUS_Handle_t aHandle, bool busOn) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  if (busOn) {
    CAN_enableController(obj->portHandle);  // clear INIT bit
  } else {
    CAN_disableController(obj->portHandle);  // set INIT bit
  }
}

bool PLX_CANBUS_getMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                           unsigned char data[], unsigned char lenMax) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  uint32_t mask = 1UL << aMailBox;
  if ((CAN_getNewDataFlags(obj->portHandle) & mask) == 0) {  // CAN_NDAT_21
    return false;                                            // no new data
  } else {
    uint16_t temp[8] = {PLX_CANBUS_PADDING_VALUE, PLX_CANBUS_PADDING_VALUE,
                        PLX_CANBUS_PADDING_VALUE, PLX_CANBUS_PADDING_VALUE,
                        PLX_CANBUS_PADDING_VALUE, PLX_CANBUS_PADDING_VALUE,
                        PLX_CANBUS_PADDING_VALUE, PLX_CANBUS_PADDING_VALUE};
    CAN_readMessage(obj->portHandle, aMailBox + 1, &temp[0]);
    for (uint16_t i = 0; i < lenMax; i++) {
      data[i] = temp[i];
    }
    return true;
  }
}

bool PLX_CANBUS_putMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                           const unsigned char data[], unsigned char len) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  HWREG_BP(obj->portHandle + CAN_O_IF1CMD) =
      ((uint32_t)CAN_IF1CMD_CONTROL | ((aMailBox + 1) & CAN_IF1CMD_MSG_NUM_M));
  uint16_t configuredLen = HWREGH(obj->portHandle + CAN_O_IF1MCTL)
                           & CAN_IF1MCTL_DLC_M;

  if (len != (unsigned char)configuredLen) {
    // catch here, b/c CAN_sendMessage has an ASSERT on this condition
    return false;
  }


  uint32_t mask = 1UL << aMailBox;
  if ((CAN_getTxRequests(obj->portHandle) & mask) != 0) {  // CAN_TXRQ_21
    return false;                                          // still busy!
  } else {
    CAN_sendMessage(obj->portHandle, aMailBox + 1, len, (uint16_t *)data);
    return true;
  }
}
