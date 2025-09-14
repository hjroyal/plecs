/*
   Copyright (c) 2021 by Plexim GmbH
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

#ifndef PLX_HAL_H_
#define PLX_HAL_H_

#include "pil.h"

// FIXME: Consider renaming aHandle, or aChannel?

extern bool PLXHAL_DIO_get(uint16_t aHandle);
extern void PLXHAL_DIO_set(uint16_t aHandle, bool aVal);

extern void PLXHAL_PWM_setDutyFreqPhase(uint16_t aChannel, float aDuty, float aFreqScaling, float aPhase);
extern void PLXHAL_PWM_setDutyPhase(uint16_t aChannel, float aDuty, float aPhase);
extern void PLXHAL_PWM_setDutyFreq(uint16_t aChannel, float aDuty, float aFreqScaling);
extern void PLXHAL_PWM_setDuty(uint16_t aHandle, float aDuty);
extern void PLXHAL_PWM_setDutyAB(uint16_t aHandle, float aDutyA, float aDutyB);
extern void PLXHAL_PWM_setToPassive(uint16_t aChannel);
extern void PLXHAL_PWM_setToOperational(uint16_t aChannel);
extern void PLXHAL_PWM_setSequence(uint16_t aChannel, uint16_t aSequence);
extern void PLXHAL_PWM_setSequenceAq(uint16_t aChannel, uint32_t aSequenceAq);
extern void PLXHAL_PWM_setRisingDelay(uint16_t aChannel, float delay);
extern void PLXHAL_PWM_setFallingDelay(uint16_t aChannel, float delay);
extern void PLXHAL_PWM_setDutyAndPeak(uint16_t aHandle, float aDuty, float aPeak);
extern void PLXHAL_PWM_setDutyFreqPhaseAndPeak(uint16_t aHandle, float aDuty, float aFreqScaling, float aPhase, float aPeak);
extern void PLXHAL_PWM_enableAllOutputs();
extern void PLXHAL_PWM_setOverflowSocDelay(uint16_t aHandle, float aPhase);
extern void PLXHAL_PWM_setUnderflowSocDelay(uint16_t aHandle, float aPhase);

extern void PLXHAL_CMPSS_setPeak(uint16_t aHandle, float aPeak);
extern void PLXHAL_CMPSS_setDacHigh(uint16_t aHandle, float aVal);

extern void PLXHAL_PWR_setEnableRequest(bool aEnable);
extern void PLXHAL_PWR_syncdPwmEnable();
extern bool PLXHAL_PWR_isEnabled();

extern float PLXHAL_ADC_getIn(uint16_t aHandle, uint16_t aChannel);

extern int16_t PLXHAL_SDFM_getFilterData16(uint16_t aHandle);
extern int32_t PLXHAL_SDFM_getFilterData32(uint16_t aHandle);

extern void  PLXHAL_StackMon_paintStack();
extern float PLXHAL_StackMon_getStackUseagePercent();

extern void PLXHAL_DAC_set(uint16_t aHandle, float aValue);

extern uint32_t PLXHAL_QEP_getCounter(uint16_t aChannel);
extern bool PLXHAL_QEP_getAndCearIndexFlag(uint16_t aChannel);
extern uint32_t PLXHAL_QEP_getIndexLatchCounter(uint16_t aChannel);

bool PLXHAL_CAP_getNewValues(uint16_t aChannel, uint16_t aNewPrescale, uint32_t *aValues, bool *aOverflowFlag);

typedef struct PLXHAL_EST_INPUTS {
  float ia;
  float ib;
  float va;
  float vb;
  float rs;
  float ld;
  float lq;
  bool enable;
  // deprecated
  int foreAngleDir;
} PLXHAL_EST_Inputs_t;

typedef struct PLXHAL_EST_OUTPUTS {
  float angle_rad;
  float fe_rps;
  int state;
  float flux_wb;
  float rs_ohm;
  float ld_henry;
  float lq_henry;
  // deprecated
  float fm_rps;
} PLXHAL_EST_Outputs_t;

extern void PLXHAL_EST_update(int16_t aChannel, const PLXHAL_EST_Inputs_t *aInputs, PLXHAL_EST_Outputs_t *aOutputs);

extern bool PLXHAL_CAN_getMessage(uint16_t aChannel, uint16_t aMailBox, unsigned char data[], unsigned char lenMax);
extern void PLXHAL_CAN_putMessage(uint16_t aChannel, uint16_t aMailBox, unsigned char data[], unsigned char len);
extern void PLXHAL_CAN_setBusOn(uint16_t aChannel, bool aBusOn);
extern bool PLXHAL_CAN_getIsBusOn(uint16_t aChannel);
extern bool PLXHAL_CAN_getIsErrorActive(uint16_t aChannel);

extern bool PLXHAL_MCAN_getMessage(uint16_t aChannel, uint16_t aMailBox, unsigned char data[], unsigned char lenMax, uint16_t *aFlags);
extern void PLXHAL_MCAN_putMessage(uint16_t aChannel, uint16_t aMailBox, unsigned char data[], unsigned char len);
extern void PLXHAL_MCAN_setBusOn(uint16_t aChannel, bool aBusOn);
extern bool PLXHAL_MCAN_getIsBusOn(uint16_t aChannel);
extern bool PLXHAL_MCAN_getIsErrorActive(uint16_t aChannel);

uint16_t PLXHAL_SCI_getChar(int16_t aChannel);
void PLXHAL_SCI_putChar(int16_t aChannel, uint16_t data);
bool PLXHAL_SCI_rxIsReady(int16_t aChannel);
bool PLXHAL_SCI_txIsBusy(int16_t aChannel);
void PLXHAL_SCI_handleBreak(int16_t aChannel);

void PLXHAL_CSV_captureInput(int16_t aChannel, void* input, uint16_t index, size_t size);
void PLXHAL_CSV_setDataFresh(int16_t aChannel);
void PLXHAL_CSV_swapBuffers(int16_t aChannel);
void PLXHAL_CSV_convertToString(int16_t aChannel);
void PLXHAL_CSV_transmitOutput(int16_t csvChannel, int16_t sciChannel);
void PLXHAL_CSV_configSignal(int16_t aChannel, uint16_t inputNum, char* formatter, uint16_t type);

uint16_t PLXHAL_SPI_getRxFifoLevel(int16_t aChannel);
bool PLXHAL_SPI_putWords(int16_t aChannel, uint16_t *aData, uint16_t aLen);
bool PLXHAL_SPI_getWords(int16_t aChannel, uint16_t *aData, uint16_t aLen);
bool PLXHAL_SPI_getAndResetRxOverrunFlag(int16_t aChannel);

extern void PLXHAL_IPC_sendDataDbBuf(uint16_t aChannel, void* data);
extern void PLXHAL_IPC_getDataDbBuf(uint16_t aChannel, void* data);

extern float PLXHAL_DISPR_getTask0LoadInPercent();

extern uint32_t PLXHAL_DISPR_getTimeStamp0();
extern uint32_t PLXHAL_DISPR_getTimeStamp1();
extern uint32_t PLXHAL_DISPR_getTimeStamp2();
extern uint32_t PLXHAL_DISPR_getTimeStamp3();
extern uint32_t PLXHAL_DISPR_getTimeStampB();
extern uint32_t PLXHAL_DISPR_getTimeStampD();
extern uint32_t PLXHAL_DISPR_getTimeStampP();

#endif // PLX_HAL_H_
