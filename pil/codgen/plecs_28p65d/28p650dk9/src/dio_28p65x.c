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

#include "f28p65x_gpio_defines.h"
#include "plx_dio.h"

#pragma diag_suppress 112 // PLX_ASSERT(0) in switch statements

typedef struct PLX_DIO_SOBJ
{
	volatile uint32_t dummyWriteVar;
	volatile uint32_t dummyReadAllwaysHighVar;
	volatile uint32_t dummyReadAllwaysLowVar;

	PLX_DIO_Obj_t dummyWriteObj;
	PLX_DIO_Obj_t dummyReadAllwaysHighObj;
	PLX_DIO_Obj_t dummyReadAllwaysLowObj;
} PLX_DIO_SObj_t;

static PLX_DIO_SObj_t PLX_DIO_SObj;

void PLX_DIO_sinit()
{
#ifdef CPU1
	EALLOW;
	// disable pin locks
	GpioCtrlRegs.GPALOCK.all = 0x00000000;
	GpioCtrlRegs.GPBLOCK.all = 0x00000000;
	GpioCtrlRegs.GPCLOCK.all = 0x00000000;
	GpioCtrlRegs.GPDLOCK.all = 0x00000000;
	GpioCtrlRegs.GPELOCK.all = 0x00000000;
	GpioCtrlRegs.GPFLOCK.all = 0x00000000;
	EDIS;
#endif

	PLX_DIO_SObj.dummyWriteObj.dat = &PLX_DIO_SObj.dummyWriteVar;
	PLX_DIO_SObj.dummyWriteObj.mask = 0;

	PLX_DIO_SObj.dummyReadAllwaysHighObj.dat = &PLX_DIO_SObj.dummyReadAllwaysHighVar;
	PLX_DIO_SObj.dummyReadAllwaysHighObj.mask = 1;

	PLX_DIO_SObj.dummyReadAllwaysLowObj.dat = &PLX_DIO_SObj.dummyReadAllwaysLowVar;
	PLX_DIO_SObj.dummyReadAllwaysLowObj.mask = 1;

	PLX_DIO_SObj.dummyReadAllwaysHighVar = 1;
	PLX_DIO_SObj.dummyReadAllwaysLowVar = 0;
}

PLX_DIO_Handle_t PLX_DIO_obtainDummyWrite()
{
	return (PLX_DIO_Handle_t)&PLX_DIO_SObj.dummyWriteObj;
}

PLX_DIO_Handle_t PLX_DIO_obtainDummyRead(bool aReadValue)
{
	if(aReadValue)
	{
		return (PLX_DIO_Handle_t)&PLX_DIO_SObj.dummyReadAllwaysHighObj;
	}
	else
	{
		return (PLX_DIO_Handle_t)&PLX_DIO_SObj.dummyReadAllwaysLowObj;
	}
}

PLX_DIO_Handle_t PLX_DIO_init(void *aMemory, const size_t aNumBytes)
{
	PLX_DIO_Handle_t handle;

	if(aNumBytes < sizeof(PLX_DIO_Obj_t))
		return((PLX_DIO_Handle_t)NULL);

	// set handle
	handle = (PLX_DIO_Handle_t)aMemory;

	return handle;
}

void PLX_DIO_configureIn(PLX_DIO_Handle_t aHandle, uint16_t aChannel, PLX_DIO_InputProperties_t * const aProperties)
{
    PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;
    PLX_ASSERT((aChannel <= 168) || ((aChannel >= 198) && (aChannel <= 242)));

    obj->activeHigh = !aProperties->enableInvert;

    uint16_t group = aChannel / 32;
    uint16_t groupIndex = aChannel % 32;
    volatile uint32_t *dataRegsBaseAddr = (volatile uint32_t *)&GpioDataRegs + group*GPY_DATA_OFFSET;

    obj->mask = 1L << groupIndex;
    obj->dat = &dataRegsBaseAddr[GPYDAT];
    obj->toggle = &dataRegsBaseAddr[GPYTOGGLE];
}

void PLX_DIO_configureOut(PLX_DIO_Handle_t aHandle, uint16_t aChannel, PLX_DIO_OutputProperties_t * const aProperties)
{
    PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

    obj->activeHigh = !aProperties->enableInvert;

    uint16_t group = aChannel / 32;
    uint16_t groupIndex = aChannel % 32;
    volatile uint32_t *dataRegsBaseAddr = (volatile uint32_t *)&GpioDataRegs + group*GPY_DATA_OFFSET;

    obj->mask = 1L << groupIndex;
    obj->dat = &dataRegsBaseAddr[GPYDAT];
    obj->toggle = &dataRegsBaseAddr[GPYTOGGLE];

    // set to passive
    if(obj->activeHigh)
    {
        *obj->dat &= ~obj->mask;
    }
    else
    {
        *obj->dat |= obj->mask;
    }
}
