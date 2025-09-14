/*
   Copyright (c) 2014-2020 by Plexim GmbH
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

/*
 * TODO:
 */



#ifndef PLX_PWM_H_
#define PLX_PWM_H_

#include "includes.h"
#include "pil.h"
#include "plx_pwm_impl.h" // implementation specific

extern void PLX_PWM_sinit();

extern PLX_PWM_Handle_t PLX_PWM_init(void *aMemory, const size_t aNumBytes);

extern void PLX_PWM_setDefaultParams(PLX_PWM_Params_t *aParams);

extern void PLX_PWM_configure(PLX_PWM_Handle_t aHandle, uint16_t aModulator, const PLX_PWM_Params_t *aParams);

extern void PLX_PWM_scalePeriod(PLX_PWM_Handle_t aHandle, float aScalingFactor);

extern void PLX_PWM_setPhase(PLX_PWM_Handle_t aHandle, float aPhase);

extern void PLX_PWM_setCompare(PLX_PWM_Handle_t aHandle, uint16_t aCompare);

extern void PLX_PWM_setOutToPassive(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_setOutToOperational(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_prepareSetOutToXTransition(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_setSequenceAq(PLX_PWM_Handle_t aHandle, uint32_t aSequenceAq);

extern void PLX_PWM_setSequence(PLX_PWM_Handle_t aHandle, uint16_t aSequence);

extern uint32_t PLX_PWM_getFullDutyCompare(PLX_PWM_Handle_t aHandle);

extern uint32_t PLX_PWM_getCounter(PLX_PWM_Handle_t aHandle);

extern bool PLX_PWM_getCountDirection(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_enableOut(PLX_PWM_Handle_t aHandle);

extern bool PLX_PWM_pwmOutputIsEnabled(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_disableOut(PLX_PWM_Handle_t aHandle);

extern void PLX_PWM_setDeadTimeCounts(PLX_PWM_Handle_t aHandle, uint16_t aRisingEdgeDelay, uint16_t aFallingEdgeDelay);

extern void PLX_PWM_setDeadTimeCountsRising(PLX_PWM_Handle_t aHandle, uint16_t aRisingEdgeDelay);

extern void PLX_PWM_setDeadTimeCountsFalling(PLX_PWM_Handle_t aHandle, uint16_t aFallingEdgeDelay);

extern void PLX_PWM_setPwmDuty(PLX_PWM_Handle_t aHandle, float aDuty);

extern void PLX_PWM_enableAllClocks();

extern void PLX_PWM_disableAllClocks();

#endif /* PLX_PWM_H_ */
