/*
   Copyright (c) 2019 by Plexim GmbH
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

#include "includes.h"

#ifndef PLX_SCI_IMPL_H_
#define PLX_SCI_IMPL_H_

typedef enum PLX_SCI_UNIT { PLX_SCI_SCI_A = 0, PLX_SCI_SCI_B } PLX_SCI_Unit_t;

typedef struct PLX_SCI_OBJ {
  PLX_SCI_Unit_t unit;
  uint32_t portHandle;
  uint32_t clk;
} PLX_SCI_Obj_t;

typedef PLX_SCI_Obj_t *PLX_SCI_Handle_t;

// bit-masks for "mode"
#define ONE_STOPBIT 0x00
#define TWO_STOPBITS 0x80
#define NO_PARITY 0x00
#define ODD_PARITY 0x20
#define EVEN_PARITY 0x60
#define SEVEN_BITS 0x06
#define EIGHT_BITS 0x07
#define PLX_SCI_LOOPBACK 0x10

#define PLX_SCI_REGS_PTR ((volatile struct SCI_REGS *)obj->portHandle)

inline void PLX_SCI_reset(PLX_SCI_Handle_t aHandle) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;

  EALLOW;
  PLX_SCI_REGS_PTR->SCICTL1.all = 0;       // reset
  PLX_SCI_REGS_PTR->SCICTL1.all = 0x0013;  // enable tx, rx
  PLX_SCI_REGS_PTR->SCICTL1.all = 0x0033;  // relinquish from reset
  EDIS;
}

inline uint16_t PLX_SCI_getChar(PLX_SCI_Handle_t aHandle) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
  return PLX_SCI_REGS_PTR->SCIRXBUF.all;
}

inline void PLX_SCI_putChar(PLX_SCI_Handle_t aHandle, uint16_t c) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
  PLX_SCI_REGS_PTR->SCITXBUF.all = c;
}

inline bool PLX_SCI_rxReady(PLX_SCI_Handle_t aHandle) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
  return (PLX_SCI_REGS_PTR->SCIFFRX.bit.RXFFST != 0);
}

inline bool PLX_SCI_txIsBusy(PLX_SCI_Handle_t aHandle) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
  return (PLX_SCI_REGS_PTR->SCICTL2.bit.TXRDY == 0);
}

inline bool PLX_SCI_breakOccurred(PLX_SCI_Handle_t aHandle) {
  PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
  return ((PLX_SCI_REGS_PTR->SCIRXST.all & 0x0080) != 0);
}

#endif /* PLX_SCI_IMPL_H_ */
