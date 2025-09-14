/*
   Copyright (c) 2014-2021 by Plexim GmbH
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

#include "plx_dispatcher.h"

// singleton
DISPR_Obj_t DisprObj;
DISPR_Handle_t DisprHandle;

void DISPR_background();

void PIL_SCOPE_sample(PIL_Handle_t aPilHandle);

void DISPR_sinit() {
  DisprHandle = (DISPR_Handle_t)&DisprObj;

  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  // obj->powerupDelayIntTask1Ticks = 0;
  obj->numTasks = 0;
  obj->idleTask = (DISPR_IdleTaskPtr_t)0;
  obj->syncCallback = (DISPR_SyncCallbackPtr_t)0;
  obj->panicCallback = (DISPR_PanicCallbackPtr_t)0;
}

void DISPR_configure(uint32_t aBasePeriodInTimerTicks, PIL_Handle_t aPilHandle,
                     DISPR_TaskObj_t *aTskMemory, uint16_t aNumTasks) {
  PLX_ASSERT(aNumTasks >= 1);
  PLX_ASSERT(aNumTasks <= 16);  // number of tasks limited by size of
                                // tasksReadyFlags
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  obj->basePeriodInTimerTicks = aBasePeriodInTimerTicks;
  obj->pilHandle = aPilHandle;
  obj->tskMemory = aTskMemory;
  obj->numTasks = aNumTasks;

  CpuTimer1Regs.TCR.bit.TSS = 1;
  CpuTimer1Regs.TPRH.all = 0;

  obj->timeStampPeriod = 2 * obj->basePeriodInTimerTicks;

  // configure timer for timing diagnostics
  CpuTimer1Regs.PRD.all = obj->timeStampPeriod - 1;
  CpuTimer1Regs.TCR.bit.TRB = 1;  // reload timer (sets to period)
}

void DISPR_setPowerupDelay(uint16_t aTask1Ticks) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  obj->powerupDelayIntTask1Ticks = aTask1Ticks;
}

void DISPR_registerSyncCallback(DISPR_SyncCallbackPtr_t aCallback) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  obj->syncCallback = aCallback;
}

void DISPR_registerPanicCallback(DISPR_PanicCallbackPtr_t aCallback) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  obj->panicCallback = aCallback;
}

void DISPR_registerIdleTask(DISPR_IdleTaskPtr_t aTsk) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  obj->idleTask = aTsk;
}

void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk,
                        uint32_t aPeriodInTimerTicks, void *const aParameters) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

  PLX_ASSERT(aTaskId < obj->numTasks);
  if (aTaskId == 0) {
    // task 0 always called at dispatcher rate
    PLX_ASSERT(aPeriodInTimerTicks == obj->basePeriodInTimerTicks);
  }
  obj->tskMemory[aTaskId].periodInSysClkTicks = aPeriodInTimerTicks;
  obj->tskMemory[aTaskId].tsk = aTsk;
  obj->tskMemory[aTaskId].mask = (1 << aTaskId);
  obj->tskMemory[aTaskId].params = aParameters;
  obj->tskMemory[aTaskId].periodInDisprTicks =
      (uint16_t)(aPeriodInTimerTicks / obj->basePeriodInTimerTicks);
  // only exact multiples allowed
  PLX_ASSERT(((uint32_t)obj->tskMemory[aTaskId].periodInDisprTicks
              * obj->basePeriodInTimerTicks)
             == aPeriodInTimerTicks);
}

void DISPR_reset() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

  for (uint16_t i = 0; i < obj->numTasks; i++) {
    obj->tskMemory[i].timer = 0;
  }
  obj->tasksReadyFlags = 0;
  obj->tasksRunningFlags = 0;
}

void DISPR_start() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

  PLX_ASSERT(obj->numTasks > 0);

  obj->interruptNesting = 0;
  obj->panicOccurred = false;
  DISPR_reset();

  obj->powerupCountdown = obj->powerupDelayIntTask1Ticks;

  // roundabout way to enable interrupts (FreeRTOS compatible)
  obj->tskMemory[0].tsk(true, obj->tskMemory[0].params);

  EALLOW;
  if (obj->syncCallback) {
    obj->syncCallback();
  }
  CpuTimer1Regs.TCR.bit.TSS = 0;  // start diagnostics timer
  EDIS;

  // power-up delay
  while (obj->powerupCountdown > 0) {
    // wait to, e.g., allow gatedrive charge-pump to stabilize
    continue;
  }

  for (;;) {
    if (obj->pilHandle != 0) {
      PIL_backgroundCall(obj->pilHandle);
    }
    DISPR_background();
    if (obj->idleTask) {
      obj->idleTask();
    }
  }
}

#pragma CODE_SECTION(DISPR_dispatch, "dispatch")
void DISPR_dispatch() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

  obj->timeStamp0 = CpuTimer1Regs.TIM.all;

  // we return immediately if power-up delay has not yet expired
  if (obj->powerupCountdown > 0) {
    obj->powerupCountdown--;
    return;
  }

  obj->interruptNesting++;
  DISPR_assert(obj->interruptNesting <= obj->numTasks, "Model execution halted due to base task overrun. \nPlease increase the sample time.\n");

  if (obj->pilHandle != 0) {
#ifndef PARALLEL_COM_PROTOCOL
    PIL_BEGIN_INT_CALL(obj->pilHandle);
#else
    PIL_beginInterruptCall(obj->pilHandle);
#endif
  }

  if (obj->panicOccurred) {
    obj->interruptNesting++;
    return;
  }

  int i;
  // determine which tasks should be dispatched
  for (i = 0; i < obj->numTasks; i++) {
    if (obj->tskMemory[i].timer == 0) {
      if (i == 0) {                               // highest priority task
        obj->timeStamp1 = obj->timeStamp3;        // last start of period
        obj->timeStamp2 = obj->timeStamp2Last;    // last end of task timestamp
        obj->timeStamp3 = CpuTimer1Regs.TIM.all;  // start of new period

        // we give highest priority task special treatment (to minimize latency)
        obj->eradStamp0 = 0;
        obj->tskMemory[0].tsk(false, obj->tskMemory[0].params);
        obj->eradStamp1 = 0;
        // this is not really thread-safe, as we could be sampling a variable
        // while lower priority task is in the process of modifying its value
        if (obj->pilHandle != 0) {
          PIL_SCOPE_sample(obj->pilHandle);
        }
        obj->eradStamp2 = 0;
        obj->timeStamp2Last = CpuTimer1Regs.TIM.all;  // end of task

      } else {
        // schedule dispatching of lower priority tasks
        DISPR_assert(!(obj->tasksRunningFlags  // check for overrun
                       & obj->tskMemory[i].mask), "Model execution halted due to a task overrun. \nPlease increase the sample time.\n");
        obj->tasksReadyFlags |= obj->tskMemory[i].mask;
      }
      obj->tskMemory[i].timer = obj->tskMemory[i].periodInDisprTicks;
    }
    obj->tskMemory[i].timer--;
  }

  // run scheduled lower priority tasks
  i = 1;
  uint16_t lpMask = 0xFFFE;
  while (obj->tasksReadyFlags & lpMask) {
    if (obj->tasksRunningFlags & obj->tskMemory[i].mask) {
      // we hit a task that is already running, break to avoid
      // double-dispatching, tasks with priorities lower than the currently
      // running task will be dispatched immediately after running task returns
      break;
    }
    if (obj->tasksReadyFlags & obj->tskMemory[i].mask) {
      obj->tasksRunningFlags |= obj->tskMemory[i].mask;
      obj->tasksReadyFlags &= (~obj->tskMemory[i].mask);
      // !!!!
      // This is the only place, other than in main(),
      // where calling EINT is allowed
      // !!!!
      EINT;  // re-enable interrupt to allow nesting
      obj->tskMemory[i].tsk(false, obj->tskMemory[i].params);
      DINT;
      obj->tasksRunningFlags &= (~obj->tskMemory[i].mask);
      // now see if lower priority task is ready
    }
    lpMask <<= 1;
    i++;
  }
  DINT;  // TI expects interrupts to be disabled before entering I$$REST
  obj->interruptNesting--;
}

void DISPR_background() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  // acquire consistent set of time stamps
  int32_t ts0, ts1, ts2, ts3;
  {
    uint16_t stat0 = DISPR_disableInt();
    ts0 = (int32_t)obj->timeStamp0;
    ts1 = (int32_t)obj->timeStamp1;
    ts2 = (int32_t)obj->timeStamp2;
    ts3 = (int32_t)obj->timeStamp3;
    DISPR_restoreInt(stat0);
  }

  // Note: CPU timer counts down
  if (ts1 <= ts2) {
    ts1 += obj->timeStampPeriod;
  }
  if (ts2 <= ts3) {
    ts3 -= obj->timeStampPeriod;
  }

  int32_t tsD = ts1 - ts2;  // base task duration
  int32_t tsP = ts1 - ts3;  // base task period

  float load = (float)(100 * tsD) / (float)(tsP);

  // start of base task
  int32_t tsB = ts1;
  if (tsB >= obj->timeStampPeriod) {
    tsB -= obj->timeStampPeriod;
  }
  tsB = obj->timeStampPeriod - tsB;

  if (tsB >= (int32_t)obj->basePeriodInTimerTicks) {
    tsB -= obj->basePeriodInTimerTicks;
  }

  {
    uint16_t stat0 = DISPR_disableInt();
    obj->timeStamp0Latched = ts0;
    obj->timeStamp1Latched = ts1;
    obj->timeStamp2Latched = ts2;
    obj->timeStamp3Latched = ts3;
    obj->timeStampPLatched = tsP;
    obj->timeStampDLatched = tsD;
    obj->timeStampBLatched = tsB;
    obj->task0LoadInPercent = load;
    DISPR_restoreInt(stat0);
  }
}
