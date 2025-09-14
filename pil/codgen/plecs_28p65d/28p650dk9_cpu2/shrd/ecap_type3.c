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

#include "inc/hw_memmap.h"
#include "plx_cap.h"

PLX_CAP_Handle_t PLX_CAP_init(void *aMemory, const size_t aNumBytes) {
  PLX_CAP_Handle_t handle;

  if (aNumBytes < sizeof(PLX_CAP_Obj_t)) {
    return ((PLX_CAP_Handle_t)NULL);
  }
  // set handle
  handle = (PLX_CAP_Handle_t)aMemory;

  return handle;
}

void PLX_CAP_setDefaultParams(uint16_t aNumEvents, bool aSingleShotOperation,
                              PLX_CAP_Params_t *aParams) {
  PLX_ASSERT(aSingleShotOperation);

  aParams->reg.ECCTL1.all = 0;       // init to reset value
  aParams->reg.ECCTL2.all = 0x000E;  // init to reset value

  aParams->reg.ECCTL2.bit.STOP_WRAP = aNumEvents - 1;

  aParams->reg.ECCTL2.bit.CONT_ONESHT = 1;  // one-shot
  aParams->reg.ECCTL2.bit.SYNCI_EN = 0;     // disable sync in
  aParams->reg.ECCTL2.bit.SYNCO_SEL = 0;    // pass through
}

void PLX_CAP_configure(PLX_CAP_Handle_t aHandle, uint16_t aUnit, uint16_t aGpio,
                       const PLX_CAP_Params_t *aParams) {
  PLX_CAP_Obj_t *obj = (PLX_CAP_Obj_t *)aHandle;

  EALLOW;
  switch (aUnit) {
    default:
      PLX_ASSERT(0);
      break;
#ifdef ECAP1_BASE
    case 1:
      obj->ecap = &ECap1Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
      ECap1Regs.ECCTL0.bit.INPUTSEL = 6;  // cross-bar 7
      break;
#endif
#ifdef ECAP2_BASE
    case 2:
      obj->ecap = &ECap2Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
      ECap2Regs.ECCTL0.bit.INPUTSEL = 7;
      break;
#endif
#ifdef ECAP3_BASE
    case 3:
      obj->ecap = &ECap3Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;
      ECap3Regs.ECCTL0.bit.INPUTSEL = 8;
      break;
#endif
#ifdef ECAP4_BASE
    case 4:
      obj->ecap = &ECap4Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP4 = 1;
      ECap4Regs.ECCTL0.bit.INPUTSEL = 9;
      break;
#endif
#ifdef ECAP5_BASE
    case 5:
      obj->ecap = &ECap5Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP5 = 1;
      ECap5Regs.ECCTL0.bit.INPUTSEL = 10;
      break;
#endif
#ifdef ECAP6_BASE
    case 6:
      obj->ecap = &ECap6Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP6 = 1;
      ECap6Regs.ECCTL0.bit.INPUTSEL = 11;
      break;
#endif
#ifdef ECAP7_BASE
    case 7:
      obj->ecap = &ECap7Regs;
      CpuSysRegs.PCLKCR3.bit.ECAP7 = 1;
      ECap7Regs.ECCTL0.bit.INPUTSEL = 12;
      break;
#endif
  }
  EDIS;

  obj->ecap->ECCTL1.bit.CAPLDEN = 0;    // disable CAP1-CAP4 register loads
  obj->ecap->ECCTL2.bit.TSCTRSTOP = 0;  // stop counter

  obj->ecap->ECEINT.all = 0x0000;  // disable interrupts
  obj->ecap->ECCLR.all = 0xFFFF;   // clear flags

  obj->ecap->ECCTL1.all = aParams->reg.ECCTL1.all;
  obj->ecap->ECCTL2.all = aParams->reg.ECCTL2.all;

  obj->ecap->ECCTL2.bit.STOP_WRAP =
      aParams->reg.ECCTL2.bit.STOP_WRAP;  // when to stop
  obj->ecap->ECCTL1.bit.CAPLDEN = 1;      // enable unit

  obj->ecap->ECCTL2.bit.TSCTRSTOP = 1;  // start counter
  obj->ecap->ECCTL2.bit.REARM = 1;      // arm one-shot

  obj->numEvents = aParams->reg.ECCTL2.bit.STOP_WRAP + 1;
  obj->eventMask = (1 << (obj->numEvents + 1)) - 2;
}

bool PLX_CAP_getNewValues(PLX_CAP_Handle_t aHandle, uint16_t aNewPrescale,
                          uint32_t *aValues, bool *aOverflowFlag) {
  PLX_CAP_Obj_t *obj = (PLX_CAP_Obj_t *)aHandle;

  *aOverflowFlag = obj->ecap->ECFLG.bit.CTROVF;
  obj->ecap->ECCLR.bit.CTROVF = 1;

  if ((obj->ecap->ECFLG.all & obj->eventMask) == obj->eventMask) {
    volatile uint32_t *cap = &obj->ecap->CAP1;
    for (int i = 0; i < obj->numEvents; i++) {
      aValues[i] = cap[i];
    }
    obj->ecap->ECCTL1.bit.PRESCALE = (aNewPrescale >> 1) & 0x1F;
    obj->ecap->ECCLR.all = obj->eventMask;
    obj->ecap->ECCTL2.bit.REARM = 1;  // effective in single-shot and
                                      // continuous mode
    return true;
  }
  return false;
}
