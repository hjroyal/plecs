/*
   Copyright (c) 2023 by Plexim GmbH
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
#include "ipc.h"
#include "gpio.h"

// Function prototypes
void ISR_ILLEGAL(void);

void PieCntlInit(void);
static void PieVectTableInit(void);

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

	// Initialize interrupt controller and Vector Table
	// to defaults for now. Application ISR mapping done later.
	PieCntlInit();
	PieVectTableInit();

//    PLX_ASSERT(SysCtl_getClock(DEVICE_OSCSRC_FREQ) == DEVICE_SYSCLK_FREQ);
//    PLX_ASSERT(SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ) == DEVICE_LSPCLK_FREQ);

#ifdef CPU1
    //
    // Disable pin locks.
    //
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_C, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_D, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_E, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_F, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);

    SysCtl_setClock(aDeviceClkConf);
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);
#endif // CPU1
}

#ifdef CPU1
//*****************************************************************************
//
// Function to boot CPU2
// Available bootmodes :
//      - BOOTMODE_BOOT_TO_FLASH_BANK0_SECTOR0
//      - BOOTMODE_BOOT_TO_FLASH_BANK0_SECTOR127_END
//      - BOOTMODE_BOOT_TO_FLASH_BANK1_SECTOR0
//      - BOOTMODE_BOOT_TO_FLASH_BANK2_SECTOR0
//      - BOOTMODE_BOOT_TO_FLASH_BANK3_SECTOR0
//      - BOOTMODE_BOOT_TO_FLASH_BANK4_SECTOR0
//      - BOOTMODE_BOOT_TO_FLASH_BANK4_SECTOR127_END
//      - BOOTMODE_BOOT_TO_SECURE_FLASH_BANK0_SECTOR0
//      - BOOTMODE_BOOT_TO_SECURE_FLASH_BANK1_SECTOR0
//      - BOOTMODE_BOOT_TO_SECURE_FLASH_BANK2_SECTOR0
//      - BOOTMODE_BOOT_TO_SECURE_FLASH_BANK3_SECTOR0
//      - BOOTMODE_BOOT_TO_SECURE_FLASH_BANK4_SECTOR0
//      - BOOTMODE_IPC_MSGRAM_COPY_BOOT_TO_M1RAM
//      - BOOTMODE_BOOT_TO_M0RAM
//      - BOOTMODE_BOOT_TO_FWU_FLASH
//      - BOOTMODE_BOOT_TO_FWU_FLASH_ALT1
//      - BOOTMODE_BOOT_TO_FWU_FLASH_ALT2
//      - BOOTMODE_BOOT_TO_FWU_FLASH_ALT3
//
// Note that while using BOOTMODE_IPC_MSGRAM_COPY_BOOT_TO_M1RAM,
// BOOTMODE_IPC_MSGRAM_COPY_LENGTH_xxxW must be ORed with the bootmode parameter
//
// This function must be called after Device_init function
//
//*****************************************************************************
void Device_bootCPU2(uint32_t bootmode){
    //
    // Configure the CPU1TOCPU2IPCBOOTMODE register
    //
    IPC_setBootMode(IPC_CPU1_L_CPU2_R,
                    (0x5A000000UL | bootmode));

    //
    // Set IPC Flag 0
    //
    IPC_setFlagLtoR(IPC_CPU1_L_CPU2_R, IPC_FLAG0);

    //
    // Bring CPU2 out of reset. Wait for CPU2 to go out of reset.
    //
    SysCtl_controlCPU2Reset(SYSCTL_CORE_DEACTIVE);
    while(SysCtl_isCPU2Reset() == 0x1U);
}
#endif


// This function initializes the PIE control registers to a known state.
//
void PieCntlInit(void)
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
	uint16_t  i;
	uint32_t  *Source  =  (void  *)  &ISR_ILLEGAL;
	uint32_t  *Dest  =  (void  *)  &PieVectTable;

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
    if(clkMHz > 206){
        PLX_ASSERT(0);
    } else if(clkMHz > 160){
        waitstates = 0x4;
    } else if(clkMHz > 120){
        waitstates = 0x3;
    } else if(clkMHz > 80){
        waitstates = 0x2;
    } else {
        waitstates = 0x1;
    }
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, waitstates);
}

void MemCopy(uint16_t *SourceAddr, uint16_t* SourceEndAddr, uint16_t* DestAddr)
{
	while(SourceAddr < SourceEndAddr)
	{
		*DestAddr++ = *SourceAddr++;
	}
	return;
}

