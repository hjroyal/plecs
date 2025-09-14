/*
   Copyright (c) 2020 by Plexim GmbH
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

// timeout to prevent deadlock when configuring controller
#define PLX_CANBUS_CONF_TIMEOUT 100

PLX_CANBUS_Handle_t PLX_CANBUS_init(void *aMemory, const size_t aNumBytes) {
  if (aNumBytes < sizeof(PLX_CANBUS_Obj_t)) {
    return ((PLX_CANBUS_Handle_t)NULL);
  }
  PLX_CANBUS_Handle_t handle = (PLX_CANBUS_Handle_t)aMemory;
  return handle;
}

void PLX_CANBUS_configureViaPinSet(PLX_CANBUS_Handle_t aHandle,
                                   PLX_CANBUS_Unit_t aUnit, uint16_t aPinSet,
                                   const PLX_CANBUS_Params_t *aParams) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  EALLOW;             // EALLOW enables access to protected bits
  switch (aPinSet) {  // must match cans.pinsets defined in TIxxxx.lua

    case 0:  // A_GPIO30_GPIO31 = 0
      obj->eCanRegsPtr = &ECanaRegs;
      obj->mailboxArray = &ECanaMboxes.MBOX0;

      SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;  // enable eCAN-A clock

      GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;  // Enable pull-up for GPIO30 (CANRXA)
      GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;  // Enable pull-up for GPIO31 (CANTXA)
      GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;  // Asynch qual for GPIO30 (CANRXA)

      GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;  // Configure GPIO30 for CANRXA
      GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;  // Configure GPIO31 for CANTXA
      break;
#ifdef TARGET_2833x                        // CAN B supported for this family
    case 10:                               // B_GPIO21_GPIO20 = 10
      GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;  // Enable pull-up for GPIO21 (CANRXB)
      GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;  // Enable pull-up for GPIO20 (CANTXB)
      GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;  // Asynch qual for GPIO21 (CANRXB)

      GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB
      GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB

      SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 1;  // enable eCAN-B clock
      obj->eCanRegsPtr = &ECanbRegs;
      obj->mailboxArray = &ECanbMboxes.MBOX0;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }

  // configure eCAN RX and TX pins
  obj->eCanShadow.CANTIOC.all = obj->eCanRegsPtr->CANTIOC.all;
  obj->eCanShadow.CANTIOC.bit.TXFUNC = 1;
  obj->eCanRegsPtr->CANTIOC.all = obj->eCanShadow.CANTIOC.all;

  obj->eCanShadow.CANRIOC.all = obj->eCanRegsPtr->CANRIOC.all;
  obj->eCanShadow.CANRIOC.bit.RXFUNC = 1;
  obj->eCanRegsPtr->CANRIOC.all = obj->eCanShadow.CANRIOC.all;

  // get controller into configuration mode and set master controls
  obj->eCanShadow.CANMC.all = 0;
  obj->eCanShadow.CANMC.bit.CCR = 1;  // Set CCR = 1
  obj->eCanShadow.CANMC.bit.SCB = 1;  // configure in eCAN mode
                                      // (all 32 mailboxes used)
  if (aParams->autoBusOn) {
    obj->eCanShadow.CANMC.bit.ABO = 1;  // auto bus-on - tries to
                                        // recover from bus-off state
  } else {
    obj->eCanShadow.CANMC.bit.ABO = 0;
  }
  obj->eCanRegsPtr->CANMC.all = obj->eCanShadow.CANMC.all;

  // wait for CCE bit to be set..
  uint16_t timeout = PLX_CANBUS_CONF_TIMEOUT;
  do {
    timeout--;
    obj->eCanShadow.CANES.all = obj->eCanRegsPtr->CANES.all;
  } while ((obj->eCanShadow.CANES.bit.CCE != 1) && (timeout > 0));
  if (timeout == 0) {
    EDIS;
    PLX_ASSERT(0);
  }

  // configure bit timing parameters
  obj->eCanShadow.CANBTC.all = 0;
  obj->eCanShadow.CANBTC.bit.BRPREG = aParams->brp - 1;
  obj->eCanShadow.CANBTC.bit.TSEG1REG = aParams->tseg1 - 1;
  obj->eCanShadow.CANBTC.bit.TSEG2REG = aParams->tseg2 - 1;
  obj->eCanShadow.CANBTC.bit.SAM = aParams->sam;
  obj->eCanShadow.CANBTC.bit.SJWREG = aParams->sjw - 1;
  obj->eCanRegsPtr->CANBTC.all = obj->eCanShadow.CANBTC.all;

  // this is probably not needed (CANGAM only relevant for SCC mode)
  obj->eCanShadow.CANGAM.all = obj->eCanRegsPtr->CANGAM.all;
  obj->eCanShadow.CANGAM.bit.AMI = 0;  // AMI
  obj->eCanRegsPtr->CANGAM.all = obj->eCanShadow.CANGAM.all;

  // disable all mailboxes
  obj->eCanShadow.CANME.all = 0;
  obj->eCanRegsPtr->CANME.all = obj->eCanShadow.CANME.all;

  // release configuration mode
  obj->eCanShadow.CANMC.all = obj->eCanRegsPtr->CANMC.all;
  obj->eCanShadow.CANMC.bit.CCR = 0;
  obj->eCanRegsPtr->CANMC.all = obj->eCanShadow.CANMC.all;

  // initialize all MSGCTRL registers to zero
  for (int i = 0; i < 32; i++) {
    obj->mailboxArray[i].MSGCTRL.all = 0;
  }

  // Wait for CCE bit to be  cleared..
  timeout = PLX_CANBUS_CONF_TIMEOUT;
  do {
    timeout--;
    obj->eCanShadow.CANES.all = obj->eCanRegsPtr->CANES.all;
  } while ((obj->eCanShadow.CANES.bit.CCE != 0) && (timeout > 0));

  EDIS;
  PLX_ASSERT(timeout != 0);
}

void PLX_CANBUS_setupMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                             bool aConfigureForTransmit, uint32_t aId,
                             bool isExtended, unsigned char aLen) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  uint32_t mask = 1UL << aMailBox;

  if (aConfigureForTransmit) {
    uint16_t timeout;

    // abort transmission
    obj->eCanShadow.CANTRR.all = obj->eCanRegsPtr->CANTRR.all;
    obj->eCanShadow.CANTRR.all |= mask;
    obj->eCanRegsPtr->CANTRR.all = obj->eCanShadow.CANTRR.all;

    // wait for bit to be cleared
    timeout = PLX_CANBUS_CONF_TIMEOUT;
    do {
      timeout--;
      obj->eCanShadow.CANTRS.all = obj->eCanRegsPtr->CANTRS.all;
    } while (((obj->eCanShadow.CANTRS.all & mask) != 0) && (timeout > 0));
    if (timeout == 0) {
      PLX_ASSERT(0);
    }
  }

  // disable mailbox
  obj->eCanShadow.CANME.all = obj->eCanRegsPtr->CANME.all;
  obj->eCanShadow.CANME.all &= ~mask;
  obj->eCanRegsPtr->CANME.all = obj->eCanShadow.CANME.all;

  // load message identifier (AAM and AME = 0 for normal tx box)
  obj->mailboxArray[aMailBox].MSGID.bit.AME = 0;  // na
  obj->mailboxArray[aMailBox].MSGID.bit.AAM = 0;  // auto-answer = off

  if (isExtended) {
    obj->mailboxArray[aMailBox].MSGID.bit.IDE = 1;  // extended ID
    obj->mailboxArray[aMailBox].MSGID.bit.STDMSGID =
        (uint16_t)((aId >> 18) & 0x7FF);
    obj->mailboxArray[aMailBox].MSGID.bit.EXTMSGID_H =
        (uint16_t)((aId >> 16) & 0x3);
    obj->mailboxArray[aMailBox].MSGID.bit.EXTMSGID_L = (uint16_t)(aId & 0xFFFF);
  } else {
    obj->mailboxArray[aMailBox].MSGID.bit.IDE = 0;  // standard ID
    obj->mailboxArray[aMailBox].MSGID.bit.STDMSGID = (uint16_t)aId;
  }

  if (aConfigureForTransmit) {
    obj->mailboxArray[aMailBox].MSGCTRL.bit.RTR = 0;

    // set mailbox direction to transmit
    obj->eCanShadow.CANMD.all = obj->eCanRegsPtr->CANMD.all;
    obj->eCanShadow.CANMD.all &= ~mask;
    obj->eCanRegsPtr->CANMD.all = obj->eCanShadow.CANMD.all;
  } else {
    // set mailbox direction to receive
    obj->eCanShadow.CANMD.all = obj->eCanRegsPtr->CANMD.all;
    obj->eCanShadow.CANMD.all |= mask;
    obj->eCanRegsPtr->CANMD.all = obj->eCanShadow.CANMD.all;
  }

  // enable mailbox
  obj->eCanShadow.CANME.all = obj->eCanRegsPtr->CANME.all;
  obj->eCanShadow.CANME.all |= mask;
  obj->eCanRegsPtr->CANME.all = obj->eCanShadow.CANME.all;
}

void PLX_CANBUS_disableMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  uint32_t mask = 1UL << aMailBox;

  obj->eCanShadow.CANME.all = obj->eCanRegsPtr->CANME.all;
  obj->eCanShadow.CANME.all &= ~mask;
  obj->eCanRegsPtr->CANME.all = obj->eCanShadow.CANME.all;
}

bool PLX_CANBUS_isBusOn(PLX_CANBUS_Handle_t aHandle) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  obj->eCanShadow.CANES.all = obj->eCanRegsPtr->CANES.all;
  obj->eCanShadow.CANMC.all = obj->eCanRegsPtr->CANMC.all;
  return ((obj->eCanShadow.CANES.bit.BO == 0)
          && (obj->eCanShadow.CANMC.bit.CCR == 0));
}

bool PLX_CANBUS_isErrorActive(PLX_CANBUS_Handle_t aHandle) {
  if (!PLX_CANBUS_isBusOn(aHandle)) {
    return false;
  }
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  // EP flag does not seem to provide reliable info, use TEC instead
  uint16_t tec = obj->eCanRegsPtr->CANTEC.bit.TEC;
  return (tec != 128);
}

void PLX_CANBUS_setBusOn(PLX_CANBUS_Handle_t aHandle, bool busOn) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  obj->eCanShadow.CANMC.all = obj->eCanRegsPtr->CANMC.all;
  if (busOn) {
    obj->eCanShadow.CANMC.bit.CCR = 0;
  } else {
    obj->eCanShadow.CANMC.bit.CCR = 1;
  }
  EALLOW;
  obj->eCanRegsPtr->CANMC.all = obj->eCanShadow.CANMC.all;
  EDIS;
}

bool PLX_CANBUS_getMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                           unsigned char data[], unsigned char lenMax) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  uint32_t mask = 1UL << aMailBox;

  obj->eCanShadow.CANRMP.all = obj->eCanRegsPtr->CANRMP.all;

  if ((obj->eCanShadow.CANRMP.all & mask) == 0) {
    return false;  // no message there
  }

  // We have to loop to double check RMP. If a new message is received while we
  // are reading the data could be corrupted.
  while ((obj->eCanShadow.CANRMP.all & mask) != 0) {
    // first clear bit
    obj->eCanShadow.CANRMP.all = mask;
    obj->eCanRegsPtr->CANRMP.all = obj->eCanShadow.CANRMP.all;

    // read data
    unsigned char d[] = {obj->mailboxArray[aMailBox].MDL.byte.BYTE0,
                         obj->mailboxArray[aMailBox].MDL.byte.BYTE1,
                         obj->mailboxArray[aMailBox].MDL.byte.BYTE2,
                         obj->mailboxArray[aMailBox].MDL.byte.BYTE3,
                         obj->mailboxArray[aMailBox].MDH.byte.BYTE4,
                         obj->mailboxArray[aMailBox].MDH.byte.BYTE5,
                         obj->mailboxArray[aMailBox].MDH.byte.BYTE6,
                         obj->mailboxArray[aMailBox].MDH.byte.BYTE7};

    if (lenMax > 8) {
      lenMax = 8;
    }
    for (int i = 0; i < lenMax; i++) {
      if (i < obj->mailboxArray[aMailBox].MSGCTRL.bit.DLC) {
        data[i] = d[i];
      } else {
        data[i] = 0xFF;
      }
    }

    // Clear Receive Message Lost
    obj->eCanShadow.CANRML.all = obj->eCanRegsPtr->CANRML.all;
    if ((obj->eCanShadow.CANRML.all & mask) != 0) {
      obj->eCanShadow.CANRML.all = mask;
      obj->eCanRegsPtr->CANRML.all = obj->eCanShadow.CANRML.all;
    }

    // Refresh the Shadow register to check if data has been overwritten
    obj->eCanShadow.CANRMP.all = obj->eCanRegsPtr->CANRMP.all;
  }
  return true;
}

bool PLX_CANBUS_putMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox,
                           const unsigned char data[], unsigned char len) {
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t *)aHandle;

  uint32_t mask = 1UL << aMailBox;

  obj->eCanShadow.CANTRS.all = obj->eCanRegsPtr->CANTRS.all;
  if ((obj->eCanShadow.CANTRS.all & mask) != 0) {
    // tx mailbox is busy!
    return false;
  }

  if (len > 8) {
    len = 8;
  }

  unsigned char d[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  for (int i = 0; i < len; i++) {
    d[i] = data[i];
  }
  obj->mailboxArray[aMailBox].MSGCTRL.bit.DLC = len;
  obj->mailboxArray[aMailBox].MDL.byte.BYTE0 = d[0];
  obj->mailboxArray[aMailBox].MDL.byte.BYTE1 = d[1];
  obj->mailboxArray[aMailBox].MDL.byte.BYTE2 = d[2];
  obj->mailboxArray[aMailBox].MDL.byte.BYTE3 = d[3];
  obj->mailboxArray[aMailBox].MDH.byte.BYTE4 = d[4];
  obj->mailboxArray[aMailBox].MDH.byte.BYTE5 = d[5];
  obj->mailboxArray[aMailBox].MDH.byte.BYTE6 = d[6];
  obj->mailboxArray[aMailBox].MDH.byte.BYTE7 = d[7];

  obj->eCanShadow.CANTRS.all = mask;
  obj->eCanRegsPtr->CANTRS.all = obj->eCanShadow.CANTRS.all;
  return true;
}
