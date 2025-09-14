/*
   Copyright (c) 2022 by Plexim GmbH
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

#include "includes.h"
#include "debug.h"

#include "flash.h"
#include "sysctl.h"


// Function prototypes
void ISR_ILLEGAL(void);

static void PieCntlInit(void);
static void PieVectTableInit(void);

#ifdef CPU1
static void EnableUnbondedIOPullups();
#endif

#pragma diag_suppress 112 // PLX_ASSERT(0) in switch statements

#ifdef CPU1
void DevInit(uint32_t aDeviceClkConf){
#else
void DevInit(void){
#endif
    SysCtl_disableWatchdog();

	DINT;			// Global Disable all Interrupts
	IER = 0x0000;	// Disable CPU interrupts
	IFR = 0x0000;	// Clear all CPU interrupt flags

	// Initialise interrupt controller and Vector Table
	// to defaults for now. Application ISR mapping done later.
	PieCntlInit();
	PieVectTableInit();

#ifdef CPU1
	//
	//      *IMPORTANT*
	//
	// The Device_cal function, which copies the ADC & oscillator calibration
	// values from TI reserved OTP into the appropriate trim registers, occurs
	// automatically in the Boot ROM. If the boot ROM code is bypassed during
	// the debug process, the following function MUST be called for the ADC and
	// oscillators to function according to specification. The clocks to the
	// ADC MUST be enabled before calling this function.
	//
	// See the device data manual and/or the ADC Reference Manual for more
	// information.
	//
	EALLOW;

	//
	// Enable pull-ups on unbonded IOs as soon as possible to reduce power
	// consumption.
	//
	EnableUnbondedIOPullups();

	CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

	// this does not seem to work!
	//(*Device_cal)();

	//
	// Check if device is trimmed
	//
	if(*((Uint16 *)0x5D1B6) == 0x0000){
		//
		// Device is not trimmed--apply static calibration values
		//
		AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
		AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
		AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
		AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
	}

	CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
	EDIS;

	SysCtl_setClock(aDeviceClkConf);
#endif // CPU1
}

// This function initializes the PIE control registers to a known state.
//
static void PieCntlInit(void)
{
	//
	// Disable Interrupts at the CPU level:
	//
	DINT;

	//
	// Disable the PIE
	//
	PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	//
	// Clear all PIEIER registers:
	//
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	//
	// Clear all PIEIFR registers:
	//
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;
}


static void PieVectTableInit(void)
{
	Uint16  i;
	Uint32  *Source  =  (void  *)  &ISR_ILLEGAL;
	Uint32  *Dest  =  (void  *)  &PieVectTable;

	//
	// Do not write over first 3 32-bit locations (these locations are
	// initialized by Boot ROM with boot variables)
	//
	Dest  =  Dest  +  3;

	EALLOW;
	for(i  =  0;  i  <  221;  i++)
	{
		*Dest++  =  *Source;
	}
	EDIS;

	//
	// Enable the PIE Vector Table
	//
	PieCtrlRegs.PIECTRL.bit.ENPIE  =  1;
}

interrupt void ISR_ILLEGAL(void)   // Illegal operation TRAP
{
	PLX_ASSERT(0);
}

void InitFlashHz(uint32_t clkHz)
{
    uint16_t waitstates;
    uint16_t clkMHz = (uint16_t)(clkHz / 1000000L);
    if(clkMHz > 200){
        PLX_ASSERT(0);
    } else if(clkMHz > 150){
        waitstates = 0x3;
    } else if(clkMHz > 100){
        waitstates = 0x2;
    } else if(clkMHz > 50){
        waitstates = 0x1;
    } else {
        waitstates = 0x0;
    }

    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, waitstates);
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
	while(SourceAddr < SourceEndAddr)
	{
		*DestAddr++ = *SourceAddr++;
	}
	return;
}

#ifdef CPU1
static void EnableUnbondedIOPullupsFor176Pin()
{
	EALLOW;
	GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
	GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
	GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
	GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
	EDIS;
}

static void EnableUnbondedIOPullupsFor100Pin()
{
	EALLOW;
	GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  //GPIOs 0-1, 5-9, 22-31
	GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  //GPIOs 32-40, 44-57
	GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  //GPIOs 67-68, 74-77, 79-83, 93-95
	GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
	GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  //GPIOs 128-159
	GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
	EDIS;
}

//
// GPIO_EnableUnbondedIOPullups - Enable IO pullups for specific packages
//
static void EnableUnbondedIOPullups()
{
	unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ;

	//
	// 5 = 100 pin
	// 6 = 176 pin
	// 7 = 337 pin
	//
	if(pin_count == 5)
	{
		EnableUnbondedIOPullupsFor100Pin();
	}
	else if (pin_count == 6)
	{
		EnableUnbondedIOPullupsFor176Pin();
	}
	else
	{
		//
		//do nothing - this is 337 pin package
		//
	}
}
#endif // CPU1
