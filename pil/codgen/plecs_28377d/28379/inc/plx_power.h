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



#ifndef PLX_PWR_H_
#define PLX_PWR_H_

#include "includes.h"
#include "pil.h"
#include "plx_pwm.h"
#include "plx_dio.h"
#include "plx_power_impl.h"

extern void PLX_PWR_sinit();

extern void PLX_PWR_configure(PLX_DIO_Handle_t aHandle, uint16_t aFsmExecRateHz);
extern void PLX_PWR_setEnableDelay(int16_t aDelayInMs);
extern void PLX_PWR_registerPwmChannel(PLX_PWM_Handle_t aChannel);
extern void PLX_PWR_configureTZGpio(uint16_t aTzId, uint16_t aGpio);

extern void PLX_PWR_setEnableRequest(bool aEnable);
extern void PLX_PWR_runFsm();
extern void PLX_PWR_syncdSwitchingEnable(); // call from control thread

extern void PLX_PWR_setPilMode(bool pilMode);  // OK to call from any thread
extern bool PLX_PWR_isReadyForEnable(); // OK to call from any thread
extern bool PLX_PWR_isEnabled(); // OK to call from any thread
extern void PLX_PWR_emergencyOff();

#endif /* PLX_PWR_H_ */
