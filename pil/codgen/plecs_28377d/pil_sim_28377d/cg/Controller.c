/*
 * Implementation file for: buck_converter_boostxl/Controller
 * Generated with         : PLECS 4.9.2
 *                          TI2837x 1.10.1
 * Generated on           : 1 Jun 2025 02:49:16
 */
#include "Controller.h"
#ifndef PLECS_HEADER_Controller_h_
#error The wrong header file "Controller.h" was included. Please check your
#error include path to see whether this file name conflicts with the name
#error of another header file.
#endif /* PLECS_HEADER_Controller_h_ */
#if defined(__GNUC__) && (__GNUC__ > 4)
#   define _ALIGNMENT 16
#   define _RESTRICT __restrict
#   define _ALIGN __attribute__((aligned(_ALIGNMENT)))
#   if defined(__clang__)
#      if __has_builtin(__builtin_assume_aligned)
#         define _ASSUME_ALIGNED(a) __builtin_assume_aligned(a, _ALIGNMENT)
#      else
#         define _ASSUME_ALIGNED(a) a
#      endif
#   else
#      define _ASSUME_ALIGNED(a) __builtin_assume_aligned(a, _ALIGNMENT)
#   endif
#else
#   ifndef _RESTRICT
#      define _RESTRICT
#   endif
#   ifndef _ALIGN
#      define _ALIGN
#   endif
#   ifndef _ASSUME_ALIGNED
#      define _ASSUME_ALIGNED(a) a
#   endif
#endif
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "plx_hal.h"
#define PLECSRunTimeError(msg) Controller_errorStatus = msg
#define Controller_UNCONNECTED 0
static float Controller_deriv[2] _ALIGN;
static uint32_t Controller_tickLo[2];
static int32_t Controller_tickHi[2];
Controller_BlockOutputs Controller_B;
#if defined(EXTERNAL_MODE) && EXTERNAL_MODE
const float * const Controller_ExtModeSignals[] = {
   &Controller_B.Load[0],
   &Controller_B.Saturation_2,
   &Controller_B.ADC1[0],
   &Controller_B.ADC1[1],
   &Controller_B.ADC2
};
#endif /* defined(EXTERNAL_MODE) */
Controller_Parameters Controller_P = {
   /* Parameter 'SwitchState' of
    *  Manual Signal Switch : 'Controller/SW2'
    */
   true,
   /* Parameter 'Value' of
    *  Constant : 'Controller/Vout_ref Max: 3'
    */
   2.f
};
Controller_ModelStates Controller_X _ALIGN;
const char * Controller_errorStatus;
const float Controller_sampleTime[2][2] = {
   /* Task "Base task" */
   {1.5e-05f, 0.f},
   /* Task "Blink task" */
   {0.499995f, 0.f}
};
const char * const Controller_checksum =
   "f02f9ca7431a78901a5a0d24f71aafc8761897af";
/* Target declarations */
// tag step function to allow special linking
#pragma CODE_SECTION(Controller_step, "step")
extern void Controller_initHal();

void Controller_initialize(void)
{
   Controller_tickHi[0] = 0;
   Controller_tickLo[0] = 0;
   Controller_tickHi[1] = 0;
   Controller_tickLo[1] = 0;
   memset(&Controller_X, 0, sizeof(Controller_X));

   /* Target pre-initialization */
   Controller_initHal();


   /* Initialization for Integrator : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
   Controller_X.Integrator1_x = 0;
   Controller_X.Integrator1_i1_first = 1;
   Controller_X.Integrator1_i2_prevReset = 0.f;

   /* Initialization for Integrator : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
   Controller_X.Integrator1_1_x = 0;
   Controller_X.Integrator1_1_i1_first = 1;
   Controller_X.Integrator1_1_i2_prevReset = 0.f;

   /* Initialization for Delay : 'Controller/Delay2' */
   Controller_X.Delay2 = false;
}

void Controller_step(int task_id)
{
   if (Controller_errorStatus)
   {
      return;
   }
   switch(task_id)
   {
   case 0: /* Task "Base task" */
   {

      /* CPU Load : 'Controller/Load' */
      Controller_B.Load[0] = PLXHAL_DISPR_getTask0LoadInPercent();
      Controller_B.Load[1] = PLXHAL_DISPR_getTimeStamp0();
      Controller_B.Load[2] = PLXHAL_DISPR_getTimeStampB();
      Controller_B.Load[3] = PLXHAL_DISPR_getTimeStampD();
      Controller_B.Load[4] = PLXHAL_DISPR_getTimeStampP();

      /* ADC : 'Controller/ADC1' */
      Controller_B.ADC1[0] = PLXHAL_ADC_getIn(0, 0);
      Controller_B.ADC1[1] = PLXHAL_ADC_getIn(0, 1);

      /* Saturation : 'Controller/Control/Closed_loop/Saturation'
       * incorporates
       *  Constant : 'Controller/Vout_ref\nMax: 3'
       */
      Controller_B.Saturation = Controller_P.Vout_refMax_3_Value;
      if (Controller_B.Saturation > 3.f)
      {
         Controller_B.Saturation = 3.f;
      }
      else if (Controller_B.Saturation < 0.f)
      {
         Controller_B.Saturation = 0.f;
      }

      /* Sum : 'Controller/Control/Closed_loop/Sum' */
      Controller_B.Sum = -Controller_B.ADC1[0] + Controller_B.Saturation;

      /* Manual Signal Switch : 'Controller/SW2'
       * incorporates
       *  Constant : 'Controller/Constant'
       *  Constant : 'Controller/Constant2'
       */
      Controller_B.SW2 = Controller_P.SW2_SwitchState ? 1.f : 0.f;

      /* Powerstage Protection : 'Controller/Power' */
      {
         if((Controller_B.SW2) > 0)
         {
            PLXHAL_PWR_setEnableRequest(true);
         }
         else
         {
            PLXHAL_PWR_setEnableRequest(false);
         }
      }
      Controller_B.Power = PLXHAL_PWR_isEnabled();

      /* Integrator : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
      if (Controller_X.Integrator1_i1_first ||
          (!Controller_X.Integrator1_i2_prevReset && Controller_B.Power))
      {
         Controller_X.Integrator1_x = 0.f;
      }
      Controller_B.Integrator1 = Controller_X.Integrator1_x;

      /* Sum : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Sum'
       * incorporates
       *  Product : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Product'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Source Select/internal/Constant'
       *  Product : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Product3'
       *  Sum : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Sum1'
       *  Product : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Product2'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Source Select/internal/Constant2'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Kd Integrator/Kd = 0/Constant'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Source Select/internal/Constant3'
       */
      Controller_B.Sum_1 =
         (Controller_B.Sum *
          0.66f) +
         (((Controller_B.Sum * 0.f) - 0.f) * 0.f) + Controller_B.Integrator1;

      /* Saturation : 'Controller/Control/Closed_loop/Voltage \nCompensator/Saturation/internal/Saturation Select/constant/Saturation' */
      Controller_B.Saturation_1 = Controller_B.Sum_1;
      if (Controller_B.Saturation_1 > 10.f)
      {
         Controller_B.Saturation_1 = 10.f;
      }
      else if (Controller_B.Saturation_1 < 0.f)
      {
         Controller_B.Saturation_1 = 0.f;
      }

      /* Sum : 'Controller/Control/Closed_loop/Sum2' */
      Controller_B.Sum2 = Controller_B.Saturation_1 - Controller_B.ADC1[1];

      /* Integrator : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
      if (Controller_X.Integrator1_1_i1_first ||
          (!Controller_X.Integrator1_1_i2_prevReset && Controller_B.Power))
      {
         Controller_X.Integrator1_1_x = 0.222222222f;
      }
      Controller_B.Integrator1_1 = Controller_X.Integrator1_1_x;

      /* Sum : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Sum'
       * incorporates
       *  Product : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Product'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Source Select/internal/Constant'
       *  Product : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Product3'
       *  Sum : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Sum1'
       *  Product : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Product2'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Source Select/internal/Constant2'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Kd Integrator/Kd = 0/Constant'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Source Select/internal/Constant3'
       */
      Controller_B.Sum_2 =
         (Controller_B.Sum2 *
          0.16f) +
         (((Controller_B.Sum2 *
            0.f) - 0.f) * 0.f) + Controller_B.Integrator1_1;

      /* Saturation : 'Controller/Control/Closed_loop/Duty/Saturation'
       * incorporates
       *  Product : 'Controller/Control/Closed_loop/Duty/Divide1'
       *  Constant : 'Controller/Control/Closed_loop/Duty/V4'
       */
      Controller_B.Saturation_2 = Controller_B.Sum_2 / 9.f;
      if (Controller_B.Saturation_2 > 0.45f)
      {
         Controller_B.Saturation_2 = 0.45f;
      }
      else if (Controller_B.Saturation_2 < 0.f)
      {
         Controller_B.Saturation_2 = 0.f;
      }

      /* ADC : 'Controller/ADC2' */
      Controller_B.ADC2 = PLXHAL_ADC_getIn(1, 0);

      /* PWM : 'Controller/PWM' */

      PLXHAL_PWM_setDuty(0, Controller_B.Saturation_2);

      /* Digital Out : 'Controller/PR2 enable' */
      PLXHAL_DIO_set(0, 1.f);
      if (Controller_errorStatus)
      {
         return;
      }

      /* Update for Integrator : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
      Controller_X.Integrator1_i1_first = 0;
      Controller_X.Integrator1_i2_prevReset = !!(Controller_B.Power);

      /* Update for Integrator : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1' */
      Controller_X.Integrator1_1_i1_first = 0;
      Controller_X.Integrator1_1_i2_prevReset = !!(Controller_B.Power);

      /* Update for PWM : 'Controller/PWM' */
      PLXHAL_PWR_syncdPwmEnable();

      /* Derivatives for Integrator : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1'
       * incorporates
       *  Sum : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Sum3'
       *  Product : 'Controller/Control/Closed_loop/Voltage \nCompensator/Continous Time/Product1'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Source Select/internal/Constant1'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Anti-windup\nmethod/Back-Calculation/Constant'
       *  Product : 'Controller/Control/Closed_loop/Voltage \nCompensator/Anti-windup\nmethod/Back-Calculation/Product'
       *  Sum : 'Controller/Control/Closed_loop/Voltage \nCompensator/Anti-windup\nmethod/Back-Calculation/Sum'
       *  Constant : 'Controller/Control/Closed_loop/Voltage \nCompensator/Source Select/internal/Anti-windup\nmethod/Back-Calculation/Constant'
       */
      Controller_deriv[0] =
         (Controller_B.Sum * 550.f *
          1.f) +
         ((-Controller_B.Sum_1 + Controller_B.Saturation_1) * 0.0012f);

      /* Derivatives for Integrator : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1'
       * incorporates
       *  Sum : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Sum3'
       *  Product : 'Controller/Control/Closed_loop/Current \nCompensator/Continous Time/Product1'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Source Select/internal/Constant1'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Anti-windup\nmethod/Back-Calculation/Constant'
       *  Product : 'Controller/Control/Closed_loop/Current \nCompensator/Anti-windup\nmethod/Back-Calculation/Product'
       *  Sum : 'Controller/Control/Closed_loop/Current \nCompensator/Anti-windup\nmethod/Back-Calculation/Sum'
       *  Product : 'Controller/Control/Closed_loop/Duty/Product'
       *  Constant : 'Controller/Control/Closed_loop/Duty/V4'
       *  Constant : 'Controller/Control/Closed_loop/Current \nCompensator/Source Select/internal/Anti-windup\nmethod/Back-Calculation/Constant'
       */
      Controller_deriv[1] =
         (Controller_B.Sum2 * 1350.f *
          1.f) +
         ((-Controller_B.Sum_2 +
           (Controller_B.Saturation_2 * 9.f)) * 0.000118518519f);

      /* Update continuous states */
      Controller_X.Integrator1_x += 1.5e-05f*Controller_deriv[0];
      Controller_X.Integrator1_1_x += 1.5e-05f*Controller_deriv[1];
      break;
   }

   case 1:                                                                                                                               /* Task "Blink task" */
   {

      /* Delay : 'Controller/Delay2' */
      Controller_B.Delay2 = Controller_X.Delay2;

      /* Logical Operator : 'Controller/Logical\nOperator2' */
      Controller_B.LogicalOperator2 = !Controller_B.Delay2;
      /* Digital Out : 'Controller/LED Blinking' */
      PLXHAL_DIO_set(1, Controller_B.LogicalOperator2);
      if (Controller_errorStatus)
      {
         return;
      }

      /* Update for Delay : 'Controller/Delay2' */
      Controller_X.Delay2 = Controller_B.LogicalOperator2;
      break;
   }
   }
}

void Controller_terminate(void)
{
}
