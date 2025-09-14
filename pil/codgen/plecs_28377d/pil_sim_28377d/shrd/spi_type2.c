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

#include "inc/hw_memmap.h"
#include "plx_spi.h"

#pragma diag_suppress 112  // PLX_ASSERT(0) in switch statements

PLX_SPI_Handle_t PLX_SPI_init(void *aMemory, const size_t aNumBytes) {
  if (aNumBytes < sizeof(PLX_SPI_Obj_t)) {
    return ((PLX_SPI_Handle_t)NULL);
  }
  PLX_SPI_Handle_t handle = (PLX_SPI_Handle_t)aMemory;
  return handle;
}

void PLX_SPI_configure(PLX_SPI_Handle_t aHandle, PLX_SPI_Unit_t aUnit,
                       uint32_t clk) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  obj->unit = aUnit;
  obj->clk = clk;
}

void PLX_SPI_setupPort(PLX_SPI_Handle_t aHandle, PLX_SPI_Params_t *aParams) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  EALLOW;

  switch (obj->unit) {
#ifdef SPIA_BASE
    case PLX_SPI_SPI_A:
      obj->portHandle = (uint32_t)&SpiaRegs;
      // enable clock to SPIA
      CpuSysRegs.PCLKCR8.bit.SPI_A = 1;
      break;
#endif
#ifdef SPIB_BASE
    case PLX_SPI_SPI_B:
      obj->portHandle = (uint32_t)&SpibRegs;
      // enable clock to SPIB
      CpuSysRegs.PCLKCR8.bit.SPI_B = 1;
      break;
#endif
#ifdef SPIC_BASE
    case PLX_SPI_SPI_C:
      obj->portHandle = (uint32_t)&SpicRegs;
      // enable clock to SPIB
      CpuSysRegs.PCLKCR8.bit.SPI_C = 1;
      break;
#endif
#ifdef SPID_BASE
    case PLX_SPI_SPI_D:
      obj->portHandle = (uint32_t)&SpidRegs;
      // enable clock to SPIB
      CpuSysRegs.PCLKCR8.bit.SPI_D = 1;
      break;
#endif
    default:
      PLX_ASSERT(0);
      break;
  }

  PLX_SPI_REGS_PTR->SPICCR.bit.SPISWRESET = 0;

#ifdef PLX_SPI_USE_CONTROLLER_PERIPHERAL
  PLX_SPI_REGS_PTR->SPICTL.bit.CONTROLLER_PERIPHERAL = aParams->CONTROLLERPERIPHERAL;
#else
  PLX_SPI_REGS_PTR->SPICTL.bit.MASTER_SLAVE = aParams->CONTROLLERPERIPHERAL;
#endif
  PLX_SPI_REGS_PTR->SPICTL.bit.TALK = 1;
  PLX_SPI_REGS_PTR->SPICTL.bit.CLK_PHASE = aParams->CLKPHASE;
  PLX_SPI_REGS_PTR->SPICCR.bit.CLKPOLARITY = aParams->CLKPOLARITY;

  // Calculate BRR = (LSPCLK freq / SPI CLK freq) - 1
  PLX_SPI_REGS_PTR->SPIBRR.all = (Uint16)(obj->clk / aParams->BAUDRATE - 1);
  PLX_SPI_REGS_PTR->SPICCR.bit.SPICHAR = aParams->SPICHAR - 1;
  PLX_SPI_REGS_PTR->SPISTS.all = 0x0000;  // Clear all status bits，
                                          // clear OVERRUN_FLAG and INT_FLAG
  PLX_SPI_REGS_PTR->SPIPRI.all = 0x0010;  // SPI priority high, emulator
                                          // immediate on and free run was
                                          // 0x0020.

  // Initialize SPI FIFO registers
  PLX_SPI_REGS_PTR->SPIFFTX.bit.SPIFFENA = 1;  // Enable FIFO
  PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFFINTCLR = 0;
  PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFINTCLR = 0;
  PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFOVFCLR = 0;
  PLX_SPI_REGS_PTR->SPIFFCT.all = 0x00;
  PLX_SPI_REGS_PTR->SPIFFTX.bit.TXFIFO = 1;
  PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFIFORESET = 1;
  PLX_SPI_REGS_PTR->SPIFFTX.bit.SPIRST = 0;
  PLX_SPI_REGS_PTR->SPIFFTX.bit.SPIRST = 1;

  PLX_SPI_REGS_PTR->SPICCR.bit.SPISWRESET = 1;

  EDIS;

  if (aParams->SPICHAR == 16) {
    obj->rxMask = 0xFFFF;
  } else {
    obj->rxMask = (1 << aParams->SPICHAR) - 1;
  }
  obj->txShift = 16 - aParams->SPICHAR;
}

void PLX_SPI_putGetWords(PLX_SPI_Handle_t aHandle, uint16_t *aOutData,
                         uint16_t *aInData, uint16_t aLen) {
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

  for (int i = 0; i < aLen; i++) {
    PLX_SPI_REGS_PTR->SPITXBUF = aOutData[i];
  }
  while (PLX_SPI_REGS_PTR->SPIFFRX.bit.RXFFST != aLen) {
    continue;
  }
  for (int i = 0; i < aLen; i++) {
    aInData[i] = PLX_SPI_REGS_PTR->SPIRXBUF;
  }
}
