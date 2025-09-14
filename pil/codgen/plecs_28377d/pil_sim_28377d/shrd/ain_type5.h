/*
   Copyright (c) 2014-2016 by Plexim GmbH
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

#ifndef PLX_AIN_TYPE5_H_
#define PLX_AIN_TYPE5_H_

typedef struct PLX_AIN_ADC_PARAMS {
  union ADCINTSEL1N2_REG ADCINTSEL1N2;
  union ADCCTL1_REG ADCCTL1;
  union ADCCTL2_REG ADCCTL2;
  union ADCSOCPRICTL_REG ADCSOCPRICTL;
  bool useExternalVref;
  float vref;
} PLX_AIN_AdcParams_t;

typedef struct PLX_AIN_CHANNEL_PARAMS {
  union ADCSOC0CTL_REG ADCSOCxCTL;
  float scale;
  float offset;
} PLX_AIN_ChannelParams_t;

typedef struct PLX_AIN_OBJ {
  volatile struct ADC_REGS *adc;
  union ADCSOC0CTL_REG *socCtrl;
  volatile uint16_t *results;
  float scale[PLX_AIN_NUM_CHANNELS];
  float offset[PLX_AIN_NUM_CHANNELS];
  float voltsPerAdcBit;
} PLX_AIN_Obj_t;

typedef PLX_AIN_Obj_t *PLX_AIN_Handle_t;

inline float PLX_AIN_getInF(PLX_AIN_Handle_t aHandle, uint16_t aChannel) {
  PLX_AIN_Obj_t *obj = (PLX_AIN_Obj_t *)aHandle;
  return ((float)obj->results[aChannel] * obj->scale[aChannel]
          + obj->offset[aChannel]);
}

extern uint32_t PLX_AIN_SysClkHz;

#endif /* PLX_AIN_TYPE5_H_ */
