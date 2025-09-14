/*
   Copyright (c) 2014-2016 by Plexim GmbH
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

#ifndef PLX_SPI_TYPE2_H_
#define PLX_SPI_TYPE2_H_

#define PLX_SPI_FIFO_DEPTH 16

/* TI convention:
 * pol/phase = 0/0: inactive level low, peripheral read at falling edge
                    (transitions at rising edge)
 * pol/phase = 0/1: inactive level low, peripheral read at rising edge
                    (transitions at falling edge)
 * pol/phase = 1/0: inactive level high, peripheral read at rising edge
                    (transitions at falling edge)
 * pol/phase = 1/1: inactive level high, peripheral read at falling edge
                    (transitions at rising edge)
 */

typedef struct PLX_SPI_PARAMS {
  Uint16 SPICHAR;    // Character Length
  bool CLKPOLARITY;  // Shift Clock Polarity
  bool CLKPHASE;     // SPI Clock Phase Select
  Uint32 BAUDRATE;   // SPI Baud Rate Control
  bool CONTROLLERPERIPHERAL;  // Peripheral-0 Controller-1
} PLX_SPI_Params_t;

typedef struct PLX_SPI_OBJ {
  PLX_SPI_Unit_t unit;
  uint32_t portHandle;
  uint32_t clk;
  uint16_t txShift;
  uint16_t rxMask;
} PLX_SPI_Obj_t;

typedef PLX_SPI_Obj_t *PLX_SPI_Handle_t;

inline void PLX_SPI_sinit() {
}

#define PLX_SPI_REGS_PTR ((volatile struct SPI_REGS *)obj->portHandle)

inline uint16_t PLX_SPI_getTxFifoLevel(PLX_SPI_Handle_t aHandle) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  return PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFFST;
}

inline uint16_t PLX_SPI_getRxFifoLevel(PLX_SPI_Handle_t aHandle) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  return PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFST;
}

inline void PLX_SPI_resetFifos(PLX_SPI_Handle_t aHandle) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFIFORESET = 0;
  PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFIFO = 0;
  PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFIFORESET = 1;
  PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFIFO = 1;
}

inline bool PLX_SPI_getAndResetRxOverrunFlag(PLX_SPI_Handle_t aHandle) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  uint16_t flag = PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFOVF;  // read flag
  if (flag != 0) {
    PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFOVFCLR = 1;  // clear flag
    return true;
  } else {
    return false;
  }
}

inline bool PLX_SPI_putWords(PLX_SPI_Handle_t aHandle, uint16_t *aData,
                             uint16_t aLen) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  if (PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFFST > (PLX_SPI_FIFO_DEPTH - aLen)) {
    return false;
  }

  for (int i = 0; i < aLen; i++) {
    PLX_SPI_REGS_PTR->SPITXBUF = (aData[i] << obj->txShift);
  }
  return true;
}
inline bool PLX_SPI_getWords(PLX_SPI_Handle_t aHandle, uint16_t *aData,
                             uint16_t aLen) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  if (PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFST < aLen) {
    return false;
  }

  for (int i = 0; i < aLen; i++) {
    aData[i] = PLX_SPI_REGS_PTR->SPIRXBUF & obj->rxMask;
  }
  return true;
}

#endif /* PLX_SPI_TYPE2_H_ */
