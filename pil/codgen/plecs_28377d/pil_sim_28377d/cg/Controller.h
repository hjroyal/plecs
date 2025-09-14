/*
 * Header file for: buck_converter_boostxl/Controller
 * Generated with : PLECS 4.9.2
 *                  TI2837x 1.10.1
 * Generated on   : 1 Jun 2025 02:49:16
 */
#ifndef PLECS_HEADER_Controller_h_
#define PLECS_HEADER_Controller_h_

#include <stdbool.h>
#include <stdint.h>

/* Target typedefs */
typedef int_fast8_t int8_t;
typedef uint_fast8_t uint8_t;


/* Model floating point type */
typedef float Controller_FloatType;

/* Target declarations */
extern void Controller_background(void);

/* Model checksum */
extern const char * const Controller_checksum;

/* Model error status */
extern const char * Controller_errorStatus;


/* Model sample time */
extern const float Controller_sampleTime[2][2];


/*
 * Model states */
typedef struct
{
   float Integrator1_x;             /* Controller/Control/Closed_loop/Voltage  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   bool Integrator1_i1_first;       /* Controller/Control/Closed_loop/Voltage  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   bool Integrator1_i2_prevReset;   /* Controller/Control/Closed_loop/Voltage  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   float Integrator1_1_x;           /* Controller/Control/Closed_loop/Current  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   bool Integrator1_1_i1_first;     /* Controller/Control/Closed_loop/Current  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   bool Integrator1_1_i2_prevReset; /* Controller/Control/Closed_loop/Current  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   bool Delay2;                     /* Controller/Delay2 */
} Controller_ModelStates;
extern Controller_ModelStates Controller_X;


/* Block outputs */
typedef struct
{
   float Saturation;                /* Controller/Control/Closed_loop/Saturation */
   float SW2;                       /* Controller/SW2 */
   float Saturation_1;              /* Controller/Control/Closed_loop/Voltage  Compensator/Saturation/internal/Saturation Select/constant/Saturation */
   float Saturation_2;              /* Controller/Control/Closed_loop/Duty/Saturation */
   float Load[5];                   /* Controller/Load */
   float ADC1[2];                   /* Controller/ADC1 */
   float Sum;                       /* Controller/Control/Closed_loop/Sum */
   float Power;                     /* Controller/Power */
   float Integrator1;               /* Controller/Control/Closed_loop/Voltage  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   float Sum_1;                     /* Controller/Control/Closed_loop/Voltage  Compensator/Continous Time/Sum */
   float Sum2;                      /* Controller/Control/Closed_loop/Sum2 */
   float Integrator1_1;             /* Controller/Control/Closed_loop/Current  Compensator/Continous Time/Ki Integrator/Ki ~= 0/Integrator/edge_triggered/Integrator1 */
   float Sum_2;                     /* Controller/Control/Closed_loop/Current  Compensator/Continous Time/Sum */
   float ADC2;                      /* Controller/ADC2 */
   bool Delay2;                     /* Controller/Delay2 */
   bool LogicalOperator2;           /* Controller/Logical Operator2 */
} Controller_BlockOutputs;
extern Controller_BlockOutputs Controller_B;

#if defined(EXTERNAL_MODE) && EXTERNAL_MODE
/* External mode signals */
#define Controller_NumExtModeSignals 5
extern const float * const Controller_ExtModeSignals[];
/* Tunable parameters */
#define Controller_NumTunableParameters 2
#endif /* defined(EXTERNAL_MODE) */


/* Block parameters */
typedef struct
{
   /* Parameter 'SwitchState' of
    *  Manual Signal Switch : 'Controller/SW2'
    */
   bool SW2_SwitchState;
   /* Parameter 'Value' of
    *  Constant : 'Controller/Vout_ref Max: 3'
    */
   float Vout_refMax_3_Value;
} Controller_Parameters;
extern Controller_Parameters Controller_P;

/* Entry point functions */
void Controller_initialize(void);
void Controller_step(int task_id);
void Controller_terminate(void);

#endif /* PLECS_HEADER_Controller_h_ */
