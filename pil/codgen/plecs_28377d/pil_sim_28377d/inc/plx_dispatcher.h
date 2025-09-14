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

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "includes.h"
#include "pil.h"

typedef void(*DISPR_TaskPtr_t)(bool, void * const);
typedef void(*DISPR_IdleTaskPtr_t)();
typedef void(*DISPR_SyncCallbackPtr_t)();
typedef void(*DISPR_PanicCallbackPtr_t)();

#include "plx_dispatcher_impl.h"

extern void DISPR_sinit();

extern void DISPR_configure(uint32_t aBasePeriodInTimerTicks, PIL_Handle_t aPilHandle, DISPR_TaskObj_t *aTskMemory, uint16_t aNumTasks);
extern void DISPR_setPowerupDelay(uint16_t aDelayInBaseTaskTicks);
extern void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk, uint32_t aPeriodInTimerTicks, void * const aParameters);
extern void DISPR_registerIdleTask(DISPR_IdleTaskPtr_t aTsk);
extern void DISPR_registerSyncCallback(DISPR_SyncCallbackPtr_t aCallback);
extern void DISPR_registerPanicCallback(DISPR_PanicCallbackPtr_t aCallback);

extern void DISPR_start();
extern void DISPR_dispatch();

extern uint16_t DISPR_disableInt(void);
extern void DISPR_restoreInt(uint16_t Stat0);

extern float DISPR_getTask0LoadInPercent();
extern uint32_t DISPR_getTimeStamp0();
extern uint32_t DISPR_getTimeStamp1();
extern uint32_t DISPR_getTimeStamp2();
extern uint32_t DISPR_getTimeStamp3();
extern uint32_t DISPR_getTimeStampB();
extern uint32_t DISPR_getTimeStampD();
extern uint32_t DISPR_getTimeStampP();

#endif /* DISPATCHER_H_ */
