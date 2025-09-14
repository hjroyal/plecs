/*
   Copyright (c) 2014-2023 by Plexim GmbH
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

#ifndef PLX_EPWM_TYPE4_H_
#define PLX_EPWM_TYPE4_H_

#include "inc/hw_memmap.h"

typedef struct PLX_PWM_REG_PARAMS {
  Uint16 TBPRD;
  union TBPHS_REG TBPHS;
  union TBCTL_REG TBCTL;
  union CMPCTL_REG CMPCTL;
  union AQCTLA_REG AQCTLA;
  union DBCTL_REG DBCTL;
  union DBFED_REG DBFED;
  union DBRED_REG DBRED;
  union TZCTL_REG TZCTL;
  union ETSEL_REG ETSEL;
  union TZSEL_REG TZSEL;
  union ETSOCPS_REG ETSOCPS;
} PLX_PWM_RegParams_t;

typedef enum PLX_PWM_OUTPUT_MODE {
  PLX_PWM_OUTPUT_MODE_DUAL = 0,
  PLX_PWM_OUTPUT_MODE_SINGLE,
  PLX_PWM_OUTPUT_MODE_DISABLED
} PLX_PWM_OutputMode_t;

typedef struct PLX_PWM_PARAMS {
  PLX_PWM_RegParams_t reg;
  PLX_PWM_OutputMode_t outMode;
} PLX_PWM_Params_t;

typedef struct PLX_PWM_OBJ {
  uint16_t *cmprProbe;
  volatile struct EPWM_REGS *pwm;
  uint16_t nomTBPRD;
  uint16_t sequence;
} PLX_PWM_Obj_t;

typedef PLX_PWM_Obj_t *PLX_PWM_Handle_t;

extern void PLX_PWM_getRegisterBase(PLX_PWM_Unit_t aPwmChannel,
                                    volatile struct EPWM_REGS **aReg);

inline uint32_t PLX_PWM_getFullDutyCompare(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  if (obj->pwm->TBCTL.bit.CTRMODE == TB_COUNT_UPDOWN) {
    // up-down
    return obj->pwm->TBPRD;
  } else {
    // sawtooth
    return (uint32_t)obj->pwm->TBPRD + 1;
  }
}

inline uint32_t PLX_PWM_getCounter(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  return obj->pwm->TBCTR;
}

inline bool PLX_PWM_getCountDirection(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  return obj->pwm->TBSTS.bit.CTRDIR;
}

inline void PLX_PWM_enableOut(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  EALLOW;
  obj->pwm->TZCLR.bit.OST = 1;
  EDIS;
}

inline bool PLX_PWM_pwmOutputIsEnabled(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  return (obj->pwm->TZFLG.bit.OST == 0);
}

inline void PLX_PWM_disableOut(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  EALLOW;
  obj->pwm->TZFRC.bit.OST = 1;
  // For whatever reason this disable fails sometimes unless called twice in a
  // row Specific to F28P65x target...
#ifdef TARGET_28p65x
  obj->pwm->TZFRC.bit.OST = 1;
#endif
  EDIS;
}

inline void PLX_PWM_setTZSafe(PLX_PWM_Handle_t aHandle, uint16_t aSafe) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->TZCTL.bit.TZA = aSafe;
  obj->pwm->TZCTL.bit.TZB = aSafe;
}

inline void PLX_PWM_setDeadTimeCounts(PLX_PWM_Handle_t aHandle,
                                      uint16_t aRisingEdgeDelay,
                                      uint16_t aFallingEdgeDelay) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->DBFED.all = aFallingEdgeDelay & 0x3FFF;
  obj->pwm->DBRED.all = aRisingEdgeDelay & 0x3FFF;
}

inline void PLX_PWM_setDeadTimeCountsRising(PLX_PWM_Handle_t aHandle,
                                            uint16_t aRisingEdgeDelay) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->DBRED.all = aRisingEdgeDelay & 0x3FFF;
}

inline void PLX_PWM_setDeadTimeCountsFalling(PLX_PWM_Handle_t aHandle,
                                             uint16_t aFallingEdgeDelay) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->DBFED.all = aFallingEdgeDelay & 0x3FFF;
}

inline void PLX_PWM_setOutToPassive(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  // force 'EPWMxA in' and 'EPWMxB in'
  if (obj->pwm->DBCTL.bit.POLSEL == 1) {
    // DB_ACTV_LOC
    obj->pwm->AQCSFRC.bit.CSFA = 2;  // force high
    obj->pwm->AQCSFRC.bit.CSFB = 2;  // force high
  } else {
    // DB_ACTV_HIC
    obj->pwm->AQCSFRC.bit.CSFA = 1;  // force low
    obj->pwm->AQCSFRC.bit.CSFB = 1;  // force low
  }
  // bypass dead-band submodule
  obj->pwm->DBCTL.bit.OUT_MODE = 0x0;
}

inline void PLX_PWM_setOutToOperational(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->DBCTL.bit.OUT_MODE = 0x3;  // enable dead-band submodule
  // disable forcing of 'EPWMxA in' and 'EPWMxB in'
  obj->pwm->AQCSFRC.bit.CSFA = 0;
  obj->pwm->AQCSFRC.bit.CSFB = 0;
}

inline void PLX_PWM_prepareSetOutToXTransition(PLX_PWM_Handle_t aHandle) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  // Configure shadowing to allow synchronized disabling of leg
  // through force and deadband control.
  if (obj->pwm->TBCTL.bit.CTRMODE == TB_COUNT_UP) {  // Sawtooth carrier
    obj->pwm->AQSFRC.bit.RLDCSF = 1;                 // load at period
    obj->pwm->DBCTL2.bit.LOADDBCTLMODE = 0;          // load at zero
  } else {                                           // Symmetrical carrier
    // Check polarity and sequence (odd=negative, even=positive)
    // with xor operation
    if ((obj->pwm->DBCTL.bit.POLSEL == 2) != ((obj->sequence & 0x01) == 0)) {
      // positive polarity & positive sequence
      // OR negative polarity & negative sequence
      obj->pwm->AQSFRC.bit.RLDCSF = 1;         // load at period
      obj->pwm->DBCTL2.bit.LOADDBCTLMODE = 1;  // load at period
    } else {
      // positive polarity & negative sequence
      // OR negative polarity & positive sequence
      obj->pwm->AQSFRC.bit.RLDCSF = 0;         // load at zero
      obj->pwm->DBCTL2.bit.LOADDBCTLMODE = 0;  // load at zero
    }
  }
}

inline void PLX_PWM_setSequenceAq(PLX_PWM_Handle_t aHandle,
                                  uint32_t aSequenceAq) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  // this function allows setting the AQ actions directly from a sequence mask -
  // note that AQCTLA/A2 and AQCTLB/B2 are configured identically, with the
  // exception of the CMP events - for those, we assign the CAs to AQCTLA and
  // the CBs to AQCTLB
  obj->pwm->AQCTLA.all = aSequenceAq & 0xFF;           // [CAD CAU PRD ZRO]
  obj->pwm->AQCTLA2.all = (aSequenceAq >> 16) & 0xFF;  // [T2D T2U T1D T1U]

  obj->pwm->AQCTLB.all = (aSequenceAq & 0x0F)
                         | (aSequenceAq & 0xF00);  // [CBD CBU X X PRD ZRO]
  obj->pwm->AQCTLB2.all = obj->pwm->AQCTLA2.all;   // [T2D T2U T1D T1U]

  obj->sequence = 1;  // ensures that the duty cycle is not inverted
}

inline void PLX_PWM_setSequence(PLX_PWM_Handle_t aHandle, uint16_t aSequence) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  obj->sequence = aSequence & 1;
  uint16_t zeroTransition, firstCompareTransition;

  if (obj->sequence == 0) {
    // negative sequence
    zeroTransition = 1;          // clear
    firstCompareTransition = 2;  // set
  } else {
    // positive sequence
    zeroTransition = 2;          // set
    firstCompareTransition = 1;  // clear
  }

  // configure channel A compare events
  obj->pwm->AQCTLA.bit.CAU = firstCompareTransition;
  obj->pwm->AQCTLA.bit.CAD = zeroTransition;  // only relevant for
                                              // symmetrical carrier

  // configure channel A zero and period events
  obj->pwm->AQCTLA.bit.ZRO = zeroTransition;
  if (obj->pwm->TBCTL.bit.CTRMODE != TB_COUNT_UP) {
    // only force period for symmetrical carrier
    obj->pwm->AQCTLA.bit.PRD = firstCompareTransition;
  } else {
    obj->pwm->AQCTLA.bit.PRD = 0;
  }

  // channel B has equivalent actions as channel A
  obj->pwm->AQCTLB.bit.CBU = obj->pwm->AQCTLA.bit.CAU;
  obj->pwm->AQCTLB.bit.CBD = obj->pwm->AQCTLA.bit.CAD;
  obj->pwm->AQCTLB.bit.ZRO = obj->pwm->AQCTLA.bit.ZRO;
  obj->pwm->AQCTLB.bit.PRD = obj->pwm->AQCTLA.bit.PRD;
}

inline uint16_t PLX_PWM_calcCompareValue(PLX_PWM_Handle_t aHandle,
                                         float aDuty) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  float duty = aDuty;

  if (duty > 1.0) {
    duty = 1.0;
  } else if (duty < 0.0) {
    duty = 0.0;
  }

  if (obj->sequence == 0) {
    duty = 1.0 - duty;
  }

  float cmpF = duty * (float)PLX_PWM_getFullDutyCompare(aHandle);
  if (cmpF > 65535.0) {
    cmpF = 65535.0;
  }
  return (uint16_t)cmpF;
}

inline void PLX_PWM_setPwmDutyCSmallerThanFull(PLX_PWM_Handle_t aHandle,
                                               float aDuty) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  uint16_t cmp = PLX_PWM_calcCompareValue(aHandle, aDuty);
  if (cmp >= PLX_PWM_getFullDutyCompare(aHandle)) {
    cmp = PLX_PWM_getFullDutyCompare(aHandle) - 1;
  }
  obj->pwm->CMPC = cmp;
}

inline void PLX_PWM_setPwmDutyCGreaterThanZero(PLX_PWM_Handle_t aHandle,
                                               float aDuty) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  uint16_t cmp = PLX_PWM_calcCompareValue(aHandle, aDuty);
  if (cmp < 1) {
    cmp = 1;
  }
  obj->pwm->CMPC = cmp;
}

inline void PLX_PWM_setPwmDutyB(PLX_PWM_Handle_t aHandle, float aDuty) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->CMPB.bit.CMPB = PLX_PWM_calcCompareValue(aHandle, aDuty);
}

inline void PLX_PWM_setPwmDuty(PLX_PWM_Handle_t aHandle, float aDuty) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;
  obj->pwm->CMPA.bit.CMPA = PLX_PWM_calcCompareValue(aHandle, aDuty);
}

inline void PLX_PWM_enableAllClocks() {
  EALLOW;
#ifdef EPWM1_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
#endif
#ifdef EPWM2_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
#endif
#ifdef EPWM3_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
#endif
#ifdef EPWM4_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
#endif
#ifdef EPWM5_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
#endif
#ifdef EPWM6_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
#endif
#ifdef EPWM7_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
#endif
#ifdef EPWM8_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
#endif
#ifdef EPWM9_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
#endif
#ifdef EPWM10_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
#endif
#ifdef EPWM11_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
#endif
#ifdef EPWM12_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;
#endif
#ifdef EPWM13_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM13 = 1;
#endif
#ifdef EPWM14_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM14 = 1;
#endif
#ifdef EPWM15_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM15 = 1;
#endif
#ifdef EPWM16_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM16 = 1;
#endif
#ifdef EPWM17_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM17 = 1;
#endif
#ifdef EPWM18_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM18 = 1;
#endif
  EDIS;
}

inline void PLX_PWM_disableAllClocks() {
  EALLOW;
#ifdef EPWM1_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM1 = 0;
#endif
#ifdef EPWM2_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM2 = 0;
#endif
#ifdef EPWM3_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM3 = 0;
#endif
#ifdef EPWM4_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM4 = 0;
#endif
#ifdef EPWM5_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM5 = 0;
#endif
#ifdef EPWM6_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM6 = 0;
#endif
#ifdef EPWM7_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM7 = 0;
#endif
#ifdef EPWM8_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM8 = 0;
#endif
#ifdef EPWM9_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM9 = 0;
#endif
#ifdef EPWM10_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM10 = 0;
#endif
#ifdef EPWM11_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM11 = 0;
#endif
#ifdef EPWM12_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM12 = 0;
#endif
#ifdef EPWM13_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM13 = 0;
#endif
#ifdef EPWM14_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM14 = 0;
#endif
#ifdef EPWM15_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM15 = 0;
#endif
#ifdef EPWM16_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM16 = 0;
#endif
#ifdef EPWM17_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM17 = 0;
#endif
#ifdef EPWM18_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM18 = 0;
#endif
  EDIS;
}

#endif /* PLX_EPWM_TYPE4_H_ */
