/*
   Copyright (c) 2023 by Plexim GmbH
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

#ifndef PLX_MCAN_TYPE0_H_
#define PLX_MCAN_TYPE0_H_

#include "driverlib/mcan.h"

#define PLX_MCAN_PADDING_VALUE 0xFF
#define PLX_MCAN_NUM_CHANNELS 32

typedef struct PLX_MCAN_RX_MAILBOX {
  uint32_t mask;
  unsigned char len;
  MCAN_RxBufElement buf;
} PLX_MCAN_RxMailbox_t;

typedef struct PLX_MCAN_TX_MAILBOX {
  uint32_t mask;
  MCAN_TxBufElement buf;
} PLX_MCAN_TxMailbox_t;

typedef struct PLX_MCAN_PARAMS {
  uint16_t tseg1;
  uint16_t tseg2;
  uint16_t brp;
  uint16_t sjw;
  bool enableRateSwitching;
  uint16_t tseg1_data;
  uint16_t tseg2_data;
  uint16_t brp_data;
  uint16_t sjw_data;

  bool enableSecondarySamplePoint;
  uint16_t tdcf;
  uint16_t tdco;

  PLX_MCAN_RxMailbox_t *rxMailboxes;
  uint16_t numRxMailboxes;

  PLX_MCAN_TxMailbox_t *txMailboxes;
  uint16_t numTxMailboxes;
} PLX_MCAN_Params_t;

typedef struct PLX_MCAN_OBJ {
  uint32_t portHandle;
  bool enableRateSwitching;

  PLX_MCAN_RxMailbox_t *rxMailboxes;
  uint16_t numRxMailboxes;

  PLX_MCAN_TxMailbox_t *txMailboxes;
  uint16_t numTxMailboxes;
} PLX_MCAN_Obj_t;

typedef PLX_MCAN_Obj_t *PLX_MCAN_Handle_t;

#endif /* PLX_MCAN_TYPE0_H_ */
