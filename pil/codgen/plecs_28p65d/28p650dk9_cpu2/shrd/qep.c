/*
   Copyright (c) 2014-2022 by Plexim GmbH
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

#include "plx_qep.h"

PLX_QEP_Handle_t PLX_QEP_init(void *aMemory, const size_t aNumBytes) {
  PLX_QEP_Handle_t handle;

  if (aNumBytes < sizeof(PLX_QEP_Obj_t)) {
    return ((PLX_QEP_Handle_t)NULL);
  }

  // set handle
  handle = (PLX_QEP_Handle_t)aMemory;

  return handle;
}

void PLX_QEP_setDefaultParams(PLX_QEP_Params_t *aParams) {
  aParams->QPOSINIT = 0;          // set counter initialization
  aParams->QPOSMAX = 0xFFFFFFFF;  // maximum count value

  aParams->QEPCTL.all = 0;
  aParams->QEPCTL.bit.FREE_SOFT =
      0;                         // all QEP timers stop on emulation suspend
  aParams->QEPCTL.bit.PCRM = 1;  // operate QEP in Reset on Max counter mode
  aParams->QEPCTL.bit.SEI = 0;   // disable strobe init
  aParams->QEPCTL.bit.IEI = 0;   // disable index init
  aParams->QEPCTL.bit.SWI = 0;   // don't allow software to initialize counter
  aParams->QEPCTL.bit.SEL = 0;   // disable strobe event
  aParams->QEPCTL.bit.IEL = 3;   // latch on index event marker
                                 //   (only applicable for QEPCTL[PCRM]=01)
  aParams->QEPCTL.bit.QPEN = 1;  // QEP enable
  aParams->QEPCTL.bit.QCLM = 0;  // disable EQEP capture latch
  aParams->QEPCTL.bit.UTE = 0;   // disable unit timer
  aParams->QEPCTL.bit.WDE = 0;   // disable QEP watchdog
}

#ifdef PLX_QEP_USES_DRIVERLIB
#include "inc/hw_memmap.h"  // defines EQEP1_BASE

void PLX_QEP_configure(PLX_QEP_Handle_t aHandle, uint16_t aUnit,
                       const PLX_QEP_Params_t *aParams) {
  PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;

  EALLOW;
  switch (aUnit) {
#ifdef EQEP1_BASE
    case 1:
      obj->qep = &EQep1Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP1 = 1;
      break;
#endif
#ifdef EQEP2_BASE
    case 2:
      obj->qep = &EQep2Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP2 = 1;
      break;
#endif
#ifdef EQEP3_BASE
    case 3:
      obj->qep = &EQep3Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP3 = 1;
      break;
#endif
#ifdef EQEP4_BASE
    case 4:
      obj->qep = &EQep4Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP4 = 1;
      break;
#endif
#ifdef EQEP5_BASE
    case 5:
      obj->qep = &EQep5Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP5 = 1;
      break;
#endif
#ifdef EQEP6_BASE
    case 6:
      obj->qep = &EQep6Regs;
      CpuSysRegs.PCLKCR4.bit.EQEP6 = 1;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }

#else  // not PLX_QEP_USES_DRIVERLIB

void PLX_QEP_configureWithPins(PLX_QEP_Handle_t aHandle, uint16_t aUnit,
                               uint16_t aPin_a, uint16_t aPin_b,
                               uint16_t aPin_i,
                               const PLX_QEP_Params_t *aParams) {
  PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;

  // omitted code for "internal pull-up enable" (e.g.,
  // GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0) & "synchronize inputs to SYSCLKOUT"
  // (e.g., GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0) if the selected pins are
  // already set to the required value by default.
  EALLOW;
  switch (aUnit) {
    default:
      PLX_ASSERT(0);
    case 1:
      obj->qep = &EQep1Regs;
      SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;

      // set GPIO Muxes for EQEP1
      switch (aPin_a) {
        case 20:
          GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;
          break;
        case 50:
          GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 1;
          break;
      }
      switch (aPin_b) {
        case 21:
          GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;
          break;
        case 51:
          GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 1;
          break;
      }
      switch (aPin_i) {
        case 23:
          GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;
          break;
        case 53:
          GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 1;
          break;
      }
      break;
    case 2:
      obj->qep = &EQep2Regs;
      SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;

#ifdef TARGET_2806x
      // set GPIO Muxes for EQEP2
      switch (aPin_a) {
        case 24:
          GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;
          break;
        case 54:
          GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 2;
          break;
      }
      switch (aPin_b) {
        case 25:
          GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;
          break;
        case 55:
          GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 2;
          break;
      }
      switch (aPin_i) {
        case 26:
          GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;
          break;
        case 30:
          GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 2;
          break;
        case 56:
          GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 2;
          break;
      }
#endif
#ifdef TARGET_2833x
      // set GPIO Muxes for EQEP2
      switch (aPin_a) {
        case 24:
          GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;
          break;
      }
      switch (aPin_b) {
        case 25:
          GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;
          break;
      }
      switch (aPin_i) {
        case 26:
          GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;
          break;
      }
#endif
      break;  // case 2
  }
#endif  // PLX_QEP_USES_DRIVERLIB

  obj->qep->QDECCTL.all = 0;

  obj->qep->QPOSINIT = aParams->QPOSINIT;  // set counter initialization to 0
  obj->qep->QPOSMAX = aParams->QPOSMAX;  //<<<< set maximum count value to 1999
  obj->qep->QEPCTL.all = aParams->QEPCTL.all;

  obj->qep->QPOSCTL.all =
      0;  // set eQEP position-compare control unit to the default mode

  obj->qep->QEINT.bit.QDC = 0;  // disable quadrature direction change interrupt
  obj->qep->QEINT.bit.PCO = 0;  // disable position counter overflow interrupt
  obj->qep->QEINT.bit.PCU = 0;  // disable position counter underflow interrupt
  obj->qep->QEINT.bit.PCM = 0;  // disable position compare match interrupt
  obj->qep->QEINT.bit.UTO = 0;  // disable unit time out event interrupt

  EDIS;
}
