/*
   Copyright (c) 2014-2019 by Plexim GmbH
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

#include "plx_power.h"
#include "plx_dispatcher.h"
#include "pil.h"

#pragma diag_suppress 179  // enter_PS_FSM_STATE_CRITICAL_FAULT declared
                           // but not referenced

PLX_PWR_Obj_t PLX_PWR_SObj;
PLX_PWR_Handle_t PLX_PWR_SHandle;

static bool PLX_PWR_isSafe();
static void PLX_PWR_disableSwitching();
static bool PLX_PWR_hasTripped();
static void PLX_PWR_reset();

void PLX_PWR_sinit() {
  PLX_PWR_SHandle = (PLX_PWR_Handle_t)&PLX_PWR_SObj;
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  obj->enableDelayInTicks = 0;
}

void PLX_PWR_configure(PLX_DIO_Handle_t aGateDrvEnableHandle,
                       uint16_t aFsmExecRateHz) {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  obj->gdrvEnableHandle = aGateDrvEnableHandle;

  obj->fsmExecRateHz = aFsmExecRateHz;
  obj->enableDelayInTicks = obj->fsmExecRateHz / 1000 * 100;  // 100 ms=default

  obj->numRegisteredPwmChannels = 0;

  obj->pilMode = false;

  PLX_PWR_reset();
}

void PLX_PWR_registerPwmChannel(PLX_PWM_Handle_t aChannel) {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

  PLX_ASSERT(obj->numRegisteredPwmChannels < PLX_PWR_MAX_PWM_CHANNELS);
  obj->pwmChannels[obj->numRegisteredPwmChannels] = aChannel;
  obj->numRegisteredPwmChannels++;
}

void PLX_PWR_reset() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

  obj->gatesActive = false;
  obj->enableSwitchingReq = false;
  obj->state = PLX_PWR_STATE_POWERUP;
  obj->enableReq = false;
}

bool PLX_PWR_isReadyForEnable() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  bool isReadyForEnable;
  {
    uint16_t stat0 = DISPR_disableInt();
    isReadyForEnable = (obj->state == PLX_PWR_STATE_DISABLED);
    DISPR_restoreInt(stat0);
  }
  return isReadyForEnable;
}

void PLX_PWR_setEnableRequest(bool aEnable) {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  {
    uint16_t stat0 = DISPR_disableInt();
    obj->enableReq = aEnable;
    DISPR_restoreInt(stat0);
  }
}

bool PLX_PWR_isEnabled() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  bool isEnabled;
  {
    uint16_t stat0 = DISPR_disableInt();
    isEnabled = (obj->state == PLX_PWR_STATE_ENABLED);
    DISPR_restoreInt(stat0);
  }
  return isEnabled;
}

void PLX_PWR_setPilMode(bool pilMode) {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  {
    uint16_t stat0 = DISPR_disableInt();
    obj->pilMode = pilMode;
    DISPR_restoreInt(stat0);
  }
}

void PLX_PWR_runFsm() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  uint16_t enableReq;
  {
    uint16_t stat0 = DISPR_disableInt();  // disable interrupts to allow
                                          // changing enableReq from highest
                                          // priority task
    enableReq = obj->enableReq;
    DISPR_restoreInt(stat0);
  }
  int16_t newState = obj->state;
  switch (obj->state) {
    // powerup
    case PLX_PWR_STATE_POWERUP:
      if (!enableReq) {
        goto enter_PS_FSM_STATE_DISABLED;
      }
      break;

    // disabled
    enter_PS_FSM_STATE_DISABLED:
      if (obj->gdrvEnableHandle) {
        PLX_DIO_set(obj->gdrvEnableHandle, false);
      }
      newState = PLX_PWR_STATE_DISABLED;
      PLX_PWR_disableSwitching();

      break;
    case PLX_PWR_STATE_DISABLED:
      if (enableReq) {
        goto enter_PS_FSM_STATE_ENABLING;
      }
      break;

    // enabling
    enter_PS_FSM_STATE_ENABLING:
      newState = PLX_PWR_STATE_ENABLING;
      if (obj->pilMode != true) {
        if (obj->gdrvEnableHandle) {
          PLX_DIO_set(obj->gdrvEnableHandle, true);
        }
        obj->timer = obj->enableDelayInTicks;  // for charge-pump to reach
                                               // steady-state
        if (obj->timer < 1) {
          obj->timer = 1;  // enforce minimal delay
        }
      } else {
        obj->timer = 0;
      }
      break;
    case PLX_PWR_STATE_ENABLING:
      if (obj->timer == 0) {
        goto enter_PS_FSM_STATE_ENABLED;
      } else {
        obj->timer--;
      }
      break;

    // enabled
    enter_PS_FSM_STATE_ENABLED:
      // enable powerstage
      newState = PLX_PWR_STATE_ENABLED;
      {
        uint16_t stat0 = DISPR_disableInt();
        obj->enableSwitchingReq = true;
        DISPR_restoreInt(stat0);
      }
      break;
    case PLX_PWR_STATE_ENABLED:
      // check for fault
      if (!enableReq) {
        goto enter_PS_FSM_STATE_DISABLED;
      } else {
        if (PLX_PWR_hasTripped()) {
          goto enter_PS_FSM_STATE_FAULT;
        }
      }
      break;

    // fault
    enter_PS_FSM_STATE_FAULT:
      PLX_PWR_disableSwitching();
      if (obj->gdrvEnableHandle) {
        PLX_DIO_set(obj->gdrvEnableHandle, false);
      }
      newState = PLX_PWR_STATE_FAULT;
      break;
    case PLX_PWR_STATE_FAULT:
      if (!enableReq) {
        goto enter_PS_FSM_STATE_FAULT_ACKN;
      }
      break;

    // critical fault
    enter_PS_FSM_STATE_CRITICAL_FAULT:
      PLX_PWR_disableSwitching();
      if (obj->gdrvEnableHandle) {
        PLX_DIO_set(obj->gdrvEnableHandle, false);
      }
      newState = PLX_PWR_STATE_CRITICAL_FAULT;
      break;
    case PLX_PWR_STATE_CRITICAL_FAULT:
      // no way out
      break;

    // fault acknowledge
    enter_PS_FSM_STATE_FAULT_ACKN:
      newState = PLX_PWR_STATE_FAULT_ACKN;
      break;
    case PLX_PWR_STATE_FAULT_ACKN:
      // reset gate driver here...
      goto enter_PS_FSM_STATE_DISABLED;
  }
  {
    uint16_t stat0 = DISPR_disableInt();  // disable interrupts to allow reading
                                          // state from higher priority tasks
    obj->state = newState;
    DISPR_restoreInt(stat0);
  }
}

void PLX_PWR_emergencyOff() {
  // disable outputs of all registered PWM blocks as quickly as possible
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  if (obj->gdrvEnableHandle) {
    PLX_DIO_set(obj->gdrvEnableHandle, false);
  }
  for (int i = 0; i < obj->numRegisteredPwmChannels; i++) {
    PLX_PWM_disableOut(obj->pwmChannels[i]);
  }
}

static bool PLX_PWR_isSafe() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

  return ((obj->state != PLX_PWR_STATE_ENABLED)
          && (obj->state != PLX_PWR_STATE_ENABLING));
}

static void PLX_PWR_disableSwitching() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  {
    uint16_t stat0 = DISPR_disableInt();
    obj->enableSwitchingReq = false;
    DISPR_restoreInt(stat0);
  }
  // disable actuators
  for (int i = 0; i < obj->numRegisteredPwmChannels; i++) {
    PLX_PWM_disableOut(obj->pwmChannels[i]);
  }
  obj->gatesActive = false;
}

static bool PLX_PWR_hasTripped() {
  PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
  if (obj->pilMode) {
    return false;
  }

  for (int i = 0; i < obj->numRegisteredPwmChannels; i++) {
    if (!PLX_PWM_pwmOutputIsEnabled(obj->pwmChannels[i])) {
      return true;
    }
  }
  return false;
}
