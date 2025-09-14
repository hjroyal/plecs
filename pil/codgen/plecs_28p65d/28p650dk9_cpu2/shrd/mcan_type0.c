/*
   Copyright (c) 2022 by Plexim GmbH
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

#include "plx_mcan.h"

#include "mcan.h"
#include "inc/stw_types.h"

#define MCAN_NUM_RX_MAILBOXES 64
#define MCAN_NUM_TX_MAILBOXES 32

#define MCAN_MEM_RX_MSG_SIZE_WORDS_32 18  // for 64 bytes data
#define MCAN_MEM_TX_MSG_SIZE_WORDS_32 18  // for 64 bytes data

#define MCAN_MEM_STD_MSG_FILTER_SIZE_WORDS_32 1
#define MCAN_MEM_EXT_MSG_FILTER_SIZE_WORDS_32 2

#define MCAN_STD_ID_FILT_START_ADDR (0)
#define MCAN_STD_ID_FILTER_NUM (MCAN_NUM_RX_MAILBOXES)
#define MCAN_STD_ID_FILT_END_ADDR \
  (MCAN_STD_ID_FILT_START_ADDR    \
   + MCAN_STD_ID_FILTER_NUM * MCAN_MEM_STD_MSG_FILTER_SIZE_WORDS_32)

#define MCAN_EXT_ID_FILT_START_ADDR (MCAN_STD_ID_FILT_END_ADDR)
#define MCAN_EXT_ID_FILTER_NUM (MCAN_NUM_RX_MAILBOXES)
#define MCAN_EXT_ID_FILT_END_ADDR \
  (MCAN_EXT_ID_FILT_START_ADDR    \
   + MCAN_EXT_ID_FILTER_NUM * MCAN_MEM_EXT_MSG_FILTER_SIZE_WORDS_32)

#define MCAN_TX_BUFF_START_ADDR (MCAN_EXT_ID_FILT_END_ADDR)
#define MCAN_TX_BUFF_SIZE (MCAN_NUM_TX_MAILBOXES)
#define MCAN_TX_BUFF_END_ADDR \
  (MCAN_TX_BUFF_START_ADDR + MCAN_TX_BUFF_SIZE * MCAN_MEM_TX_MSG_SIZE_WORDS_32)

#define MCAN_RX_BUFF_START_ADDR (MCAN_TX_BUFF_END_ADDR)
#define MCAN_RX_BUFF_SIZE (MCAN_NUM_RX_MAILBOXES)
#define MCAN_RX_BUFF_END_ADDR \
  (MCAN_RX_BUFF_START_ADDR + MCAN_RX_BUFF_SIZE * MCAN_MEM_RX_MSG_SIZE_WORDS_32)

#if MCAN_RX_BUFF_END_ADDR > MCAN_MEM_SIZE_WORDS_32
#error MCAN memory map overflow
#endif

#if (2 * PLX_MCAN_NUM_CHANNELS > MCAN_NUM_RX_MAILBOXES)
#error Insufficient number of rx mailboxes
#endif

#if (PLX_MCAN_NUM_CHANNELS > MCAN_NUM_TX_MAILBOXES)
#error Insufficient number of tx mailboxes
#endif

static uint32_t PLX_MCAN_getDataSize(uint32_t dlc) {
  uint32_t dataSize[16] = {0, 1,  2,  3,  4,  5,  6,  7,
                           8, 12, 16, 20, 24, 32, 48, 64};
  PLX_ASSERT(dlc < 16);
  return (dataSize[dlc]);
}

void PLX_MCAN_sinit() {
  ;
}

PLX_MCAN_Handle_t PLX_MCAN_init(void *aMemory, const size_t aNumBytes) {
  if (aNumBytes < sizeof(PLX_MCAN_Obj_t)) {
    return ((PLX_MCAN_Handle_t)NULL);
  }
  PLX_MCAN_Handle_t handle = (PLX_MCAN_Handle_t)aMemory;
  return handle;
}

void PLX_MCAN_configure(PLX_MCAN_Handle_t aHandle, PLX_MCAN_Unit_t aUnit,
                        const PLX_MCAN_Params_t *aParams) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  PLX_ASSERT(aParams->numRxMailboxes <= PLX_MCAN_NUM_CHANNELS);
  obj->rxMailboxes = aParams->rxMailboxes;
  obj->numRxMailboxes = aParams->numRxMailboxes;
  for (int i = 0; i < obj->numRxMailboxes; i++) {
    obj->rxMailboxes[i].mask = 0;
  }

  PLX_ASSERT(aParams->numTxMailboxes <= PLX_MCAN_NUM_CHANNELS);
  obj->txMailboxes = aParams->txMailboxes;
  obj->numTxMailboxes = aParams->numTxMailboxes;
  for (int i = 0; i < obj->numTxMailboxes; i++) {
    obj->txMailboxes[i].mask = 0;
  }

  switch (aUnit) {
    case PLX_MCAN_MCAN_A:
#ifdef TARGET_2838x
      obj->portHandle = MCAN0_BASE;
      SysCtl_allocateSharedPeripheral(
          SYSCTL_PALLOCATE_MCAN_A,
          0x0U);  // must be configured prior to enabling the peripheral clocks
      EALLOW;
      ClkCfgRegs.CLKSRCCTL2.bit.MCANABITCLKSEL = 0;  // SYSCLK
      EDIS;
#else
      obj->portHandle = MCANA_MSG_RAM_BASE;
      EALLOW;
      ClkCfgRegs.CLKSRCCTL2.bit.MCANABCLKSEL = 0;  // SYSCLK
      EDIS;
#endif

      SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_MCANA);

#if defined(TARGET_28p65x) || defined(TARGET_28p55x)
      SysCtl_setMCANClk(SYSCTL_MCANA, SYSCTL_MCANCLK_DIV_1);
#else
      SysCtl_setMCANClk(SYSCTL_MCANCLK_DIV_1);
#endif
      break;

#ifdef MCANB_BASE
    case PLX_MCAN_MCAN_B:
      obj->portHandle = MCANB_MSG_RAM_BASE;
      EALLOW;
      ClkCfgRegs.CLKSRCCTL2.bit.MCANBBCLKSEL = 0;  // SYSCLK
      EDIS;
      SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_MCANB);
      SysCtl_setMCANClk(SYSCTL_MCANB, SYSCTL_MCANCLK_DIV_1);
      break;
#endif

    default:
      PLX_ASSERT(0);
  }

  // keep these off the stack
  static MCAN_InitParams initParams = {0};
  static MCAN_MsgRAMConfigParams msgRAMConfigParams = {0};
  static MCAN_BitTimingParams bitTimes = {0};

  // modes
  initParams.fdMode = 1;  // no reason not to enable FD mode
  obj->enableRateSwitching = aParams->enableRateSwitching;
  if (aParams->enableRateSwitching) {
    initParams.brsEnable = 1;
  } else {
    initParams.brsEnable = 0;
  }

  // secondary sample point (SSP)
  initParams.tdcEnable = aParams->enableSecondarySamplePoint;
  initParams.tdcConfig.tdcf = aParams->tdcf - 1;
  initParams.tdcConfig.tdco = aParams->tdco - 1;

  // bit timing
  bitTimes.nomRatePrescalar = aParams->brp - 1;
  bitTimes.nomTimeSeg1 = aParams->tseg1 - 1;
  bitTimes.nomTimeSeg2 = aParams->tseg2 - 1;
  bitTimes.nomSynchJumpWidth = aParams->sjw - 1;

  if (aParams->enableRateSwitching) {
    bitTimes.dataRatePrescalar = aParams->brp_data - 1;
    bitTimes.dataTimeSeg1 = aParams->tseg1_data - 1;
    bitTimes.dataTimeSeg2 = aParams->tseg2_data - 1;
    bitTimes.dataSynchJumpWidth = aParams->sjw_data - 1;
  }

  // configure memory map for tx buffers
  msgRAMConfigParams.txStartAddr = MCAN_TX_BUFF_START_ADDR * 4;
  msgRAMConfigParams.txBufNum = MCAN_TX_BUFF_SIZE;
  msgRAMConfigParams.txBufMode = 0U;
  msgRAMConfigParams.txBufElemSize = MCAN_ELEM_SIZE_64BYTES;

  // configure memory map for rx buffers
  msgRAMConfigParams.rxBufStartAddr = MCAN_RX_BUFF_START_ADDR * 4;
  msgRAMConfigParams.rxBufElemSize = MCAN_ELEM_SIZE_64BYTES;

  // configure memory map for std id filters
  msgRAMConfigParams.flssa = MCAN_STD_ID_FILT_START_ADDR * 4;
  msgRAMConfigParams.lss = MCAN_STD_ID_FILTER_NUM;

  // configure memory map for ext id filters
  msgRAMConfigParams.flesa = MCAN_EXT_ID_FILT_START_ADDR * 4;
  msgRAMConfigParams.lse = MCAN_EXT_ID_FILTER_NUM;

  // Note: FIFOs are disabled if rxFIFOXsize, txFIFOSize, txEventFIFOSize = 0

  while (!MCAN_isMemInitDone(obj->portHandle)) {
    continue;
  }

  MCAN_setOpMode(obj->portHandle, MCAN_OPERATION_MODE_SW_INIT);
  while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(obj->portHandle)) {
    continue;
  }

  MCAN_init(obj->portHandle, &initParams);
  MCAN_setBitTime(obj->portHandle, &bitTimes);
  MCAN_msgRAMConfig(obj->portHandle, &msgRAMConfigParams);

  // disable all standard filters
  MCAN_StdMsgIDFilterElement stdFiltelem = {0};
  for (int i = 0; i < MCAN_STD_ID_FILTER_NUM; i++) {
    MCAN_addStdMsgIDFilter(obj->portHandle, i, &stdFiltelem);
  }

  // disable all extended filters
  MCAN_ExtMsgIDFilterElement extFiltelem = {0};
  for (int i = 0; i < MCAN_EXT_ID_FILTER_NUM; i++) {
    MCAN_addExtMsgIDFilter(obj->portHandle, i, &extFiltelem);
  }

  // at this point we are still "bus-off", i.e. MCAN_OPERATION_MODE_SW_INIT
  MCAN_setOpMode(obj->portHandle, MCAN_OPERATION_MODE_NORMAL);
}

void PLX_MCAN_setupRxMailbox(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox,
                             uint32_t aId, bool isExtended,
                             unsigned char aDlc) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;
  PLX_ASSERT(aMailBox < obj->numRxMailboxes);

  obj->rxMailboxes[aMailBox].mask = ((uint32_t)1U << aMailBox);
  obj->rxMailboxes[aMailBox].len = PLX_MCAN_getDataSize(aDlc);
  if (isExtended) {
    MCAN_ExtMsgIDFilterElement filterElement = {0};
    filterElement.efec = 0x7;        // destination is Rx Buffer
    filterElement.efid1 = aId;       // ID (no mask with rx buffer)
    filterElement.efid2 = aMailBox;  // store message into buffer aMailBox.
    MCAN_addExtMsgIDFilter(obj->portHandle, aMailBox, &filterElement);
    // create a double-buffer by enabling another filter/mailbox
    filterElement.efid2 = aMailBox + 32;
    MCAN_addExtMsgIDFilter(obj->portHandle, aMailBox + 32, &filterElement);
  } else {
    MCAN_StdMsgIDFilterElement filterElement = {0};
    filterElement.sfec = 0x7;        // destination is Rx Buffer
    filterElement.sfid1 = aId;       // (no mask with rx buffer)
    filterElement.sfid2 = aMailBox;  // store message into buffer aMailBox.
    // IGNORED: stdFiltelem.sft
    MCAN_addStdMsgIDFilter(obj->portHandle, aMailBox, &filterElement);
    // create a double-buffer by enabling another filter/mailbox
    filterElement.sfid2 = aMailBox + 32;
    MCAN_addStdMsgIDFilter(obj->portHandle, aMailBox + 32, &filterElement);
  }
}

void PLX_MCAN_setupTxMailbox(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox,
                             uint32_t aId, bool isExtended, unsigned char aDlc,
                             bool aEnableBrs) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;
  PLX_ASSERT(aMailBox < obj->numTxMailboxes);

  obj->txMailboxes[aMailBox].mask = ((uint32_t)1U << aMailBox);
  MCAN_TxBufElement *txMsg = &obj->txMailboxes[aMailBox].buf;

  txMsg->rtr = 0;
  if (isExtended) {
    txMsg->xtd = 1;
    txMsg->id = aId;
  } else {
    txMsg->xtd = 0;
    txMsg->id = aId << 18;
  }
  txMsg->esi = 0;  // ESI bit in CAN FD format depends
                   // only on error passive flag

  txMsg->dlc = aDlc;
  if ((aDlc > 8) || (obj->enableRateSwitching && aEnableBrs)) {
    txMsg->fdf = 1;  // frame transmitted in CAN FD format
  } else {
    txMsg->fdf = 0;
  }
  if (obj->enableRateSwitching) {
    txMsg->brs = aEnableBrs;
  } else {
    txMsg->brs = 0;
  }

  for (int i = 0; i < MCAN_MAX_PAYLOAD_BYTES; i++) {
    txMsg->data[i] = PLX_MCAN_PADDING_VALUE;
  }
}

bool PLX_MCAN_isBusOn(PLX_MCAN_Handle_t aHandle) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  if (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(obj->portHandle)) {
    return false;
  }

  MCAN_ProtocolStatus protStatus;
  MCAN_getProtocolStatus(obj->portHandle, &protStatus);

  return (protStatus.busOffStatus == 0);
}

bool PLX_MCAN_isErrorActive(PLX_MCAN_Handle_t aHandle) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  MCAN_ProtocolStatus protStatus;
  MCAN_getProtocolStatus(obj->portHandle, &protStatus);

  return (protStatus.errPassive == 0);
}

void PLX_MCAN_setBusOn(PLX_MCAN_Handle_t aHandle, bool busOn) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;
  if (busOn) {
    MCAN_setOpMode(obj->portHandle, MCAN_OPERATION_MODE_NORMAL);
  } else {
    MCAN_setOpMode(obj->portHandle, MCAN_OPERATION_MODE_SW_INIT);
  }
}

bool PLX_MCAN_readyForTx(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  PLX_ASSERT(obj->txMailboxes[aMailBox].mask);

  if (!PLX_MCAN_isBusOn(aHandle)) {
    return false;
  }
  // transmission occurred:
  // ((MCAN_getTxBufTransmissionStatus(obj->portHandle) & mask) != 0)
  return (
      (MCAN_getTxBufReqPend(obj->portHandle) & obj->txMailboxes[aMailBox].mask)
      == 0);
}

bool PLX_MCAN_getMessage(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox,
                         unsigned char data[], unsigned char lenMax,
                         uint16_t *aFlags) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  PLX_ASSERT(obj->rxMailboxes[aMailBox].mask);

  if (!PLX_MCAN_isBusOn(aHandle)) {
    return false;
  }

  MCAN_RxNewDataStatus newData = {0};
  MCAN_getNewDataStatus(obj->portHandle, &newData);

  // flags for Rx buffer no. 0 to 31
  newData.statusLow &= obj->rxMailboxes[aMailBox].mask;
  // flags for twin buffers (for double-buffering)
  newData.statusHigh &= obj->rxMailboxes[aMailBox].mask;
  if (newData.statusLow || newData.statusHigh) {
    MCAN_RxBufElement *rxMsg = &obj->rxMailboxes[aMailBox].buf;
    if (newData.statusLow) {
      MCAN_readMsgRam(obj->portHandle, MCAN_MEM_TYPE_BUF, aMailBox, 0, rxMsg);
      newData.statusHigh = 0;  // so that we don't clear pending flag below
    } else {
      MCAN_readMsgRam(obj->portHandle, MCAN_MEM_TYPE_BUF, aMailBox + 32, 0,
                      rxMsg);
    }
    int dataLen = PLX_MCAN_getDataSize(rxMsg->dlc);
    // we could be strict and check against obj->rxMailboxes[aMailBox].len
    for (int i = 0; i < lenMax; i++) {
      if (i < dataLen) {
        data[i] = rxMsg->data[i];
      } else {
        data[i] = PLX_MCAN_PADDING_VALUE;
      }
    }
    *aFlags = 0;
    if (rxMsg->esi) {
      *aFlags |= PLX_MCAN_ESI_FLAG;
    }
    if (rxMsg->fdf) {
      *aFlags |= PLX_MCAN_FD_FLAG;
    }
    if (rxMsg->brs) {
      *aFlags |= PLX_MCAN_BRS_FLAG;
    }
    MCAN_clearNewDataStatus(obj->portHandle, &newData);
    return true;
  } else {
    return false;
  }
}

bool PLX_MCAN_putMessage(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox,
                         const unsigned char data[], unsigned char len) {
  PLX_MCAN_Obj_t *obj = (PLX_MCAN_Obj_t *)aHandle;

  PLX_ASSERT(obj->txMailboxes[aMailBox].mask);

  if (!PLX_MCAN_isBusOn(aHandle)) {
    return false;
  }

  if ((MCAN_getTxBufReqPend(obj->portHandle) & obj->txMailboxes[aMailBox].mask)
      != 0) {
    return false;
  }

  MCAN_TxBufElement *txMsg = &obj->txMailboxes[aMailBox].buf;
  for (int i = 0; i < len; i++) {
    txMsg->data[i] = data[i];
  }
  MCAN_writeMsgRam(obj->portHandle, MCAN_MEM_TYPE_BUF, aMailBox, txMsg);
  if (MCAN_txBufAddReq(obj->portHandle, aMailBox) == STW_SOK) {
    return true;
  } else {
    return false;
  }
}
