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

#ifndef DISPATCHER_IMPL_H_
#define DISPATCHER_IMPL_H_

typedef struct DISPR_TASK_OBJ {
  DISPR_TaskPtr_t tsk;
  void *params;
  uint32_t periodInSysClkTicks;
  uint16_t periodInDisprTicks;
  uint16_t timer;
  uint16_t mask;
} DISPR_TaskObj_t;

typedef struct DISPR_OBJ {
  uint32_t basePeriodInTimerTicks;

  DISPR_TaskObj_t *tskMemory;
  uint16_t numTasks;
  volatile uint16_t tasksReadyFlags;
  volatile uint16_t tasksRunningFlags;
  DISPR_IdleTaskPtr_t idleTask;
  DISPR_SyncCallbackPtr_t syncCallback;
  uint16_t powerupDelayIntTask1Ticks;
  volatile uint16_t powerupCountdown;

  PIL_Handle_t pilHandle;

  // diagnostics
  bool panicOccurred;
  DISPR_PanicCallbackPtr_t panicCallback;
  int16_t interruptNesting;
  volatile uint16_t eradStamp0;
  volatile uint16_t eradStamp1;
  volatile uint16_t eradStamp2;

  volatile float task0LoadInPercent;
  uint32_t timeStampPeriod;
  uint32_t timeStamp0;
  uint32_t timeStamp1;
  uint32_t timeStamp2;
  uint32_t timeStamp2Last;
  uint32_t timeStamp3;
  uint32_t timeStamp0Latched;
  uint32_t timeStamp1Latched;
  uint32_t timeStamp2Latched;
  uint32_t timeStamp3Latched;
  uint32_t timeStampPLatched;
  uint32_t timeStampBLatched;
  uint32_t timeStampDLatched;
} DISPR_Obj_t;

typedef DISPR_Obj_t *DISPR_Handle_t;

extern DISPR_Handle_t DisprHandle;

void PIL_setErrorMessage(PIL_Handle_t aPilHandle, const char* aMessage);

inline void DISPR_assert(bool aCondition, const char *aErrorMessage) {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  if (!aCondition && !obj->panicOccurred) {
    obj->panicOccurred = true;
    if (obj->panicCallback) {
      obj->panicCallback();
    }
    PIL_setErrorMessage(obj->pilHandle, aErrorMessage);
  }
}

inline float DISPR_getTask0LoadInPercent() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->task0LoadInPercent;
}

inline uint32_t DISPR_getTimeStamp0() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStamp0Latched;
}

inline uint32_t DISPR_getTimeStamp1() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStamp1Latched;
}

inline uint32_t DISPR_getTimeStamp2() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStamp2Latched;
}

inline uint32_t DISPR_getTimeStamp3() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStamp3Latched;
}

inline uint32_t DISPR_getTimeStampP() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStampPLatched;
}

inline uint32_t DISPR_getTimeStampB() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStampBLatched;
}

inline uint32_t DISPR_getTimeStampD() {
  DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
  return obj->timeStampDLatched;
}

#endif /* DISPATCHER_IMPL_H_ */
