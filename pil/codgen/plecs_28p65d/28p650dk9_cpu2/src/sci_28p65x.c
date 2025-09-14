/*
   Copyright (c) 2019 by Plexim GmbH
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

#include "plx_sci.h"

#pragma diag_suppress 112 // PLX_ASSERT(0) in switch statement

#define PLX_SCI_ONE_STOPBIT 0x00
#define PLX_SCI_TWO_STOPBITS 0x80
#define PLX_SCI_NO_PARITY 0x00
#define PLX_SCI_ODD_PARITY 0x20
#define PLX_SCI_EVEN_PARITY 0x60
#define PLX_SCI_SEVEN_BITS 0x06
#define PLX_SCI_EIGHT_BITS 0x07
#define PLX_SCI_PLX_SCI_LOOPBACK 0x10

PLX_SCI_Handle_t PLX_SCI_init(void *aMemory, const size_t aNumBytes)
{
	if(aNumBytes < sizeof(PLX_SCI_Obj_t))
	{
		return((PLX_SCI_Handle_t)NULL);
	}
	PLX_SCI_Handle_t handle = (PLX_SCI_Handle_t)aMemory;
	return handle;
}

void PLX_SCI_configure(PLX_SCI_Handle_t aHandle, PLX_SCI_Unit_t aUnit, uint32_t aClk)
{
    PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;
    obj->unit = aUnit;
    obj->clk = aClk;
}

bool PLX_SCI_setupPort(PLX_SCI_Handle_t aHandle, uint32_t baudrate)
{
	PLX_SCI_Obj_t *obj = (PLX_SCI_Obj_t *)aHandle;

	EALLOW;

    switch(obj->unit)
    {
        case PLX_SCI_SCI_A:
            obj->portHandle = (uint32_t)&SciaRegs;
            // enable clock to SCIA
            CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
            break;

        case PLX_SCI_SCI_B:
            obj->portHandle = (uint32_t)&ScibRegs;
            // enable clock to SCIB
            CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
            break;

        default:
            PLX_ASSERT(0);
    }

    PLX_SCI_REGS_PTR->SCICTL1.all = 0; // reset SCI
    PLX_SCI_REGS_PTR->SCICCR.all = PLX_SCI_ONE_STOPBIT+ PLX_SCI_NO_PARITY + PLX_SCI_EIGHT_BITS;
    PLX_SCI_REGS_PTR->SCICTL1.all = 0x0013; // enable tx, rx, disable rx err, sleep, txwake

 	uint16_t brr = (uint16_t)(obj->clk /8l /baudrate) - 1;
    PLX_SCI_REGS_PTR->SCIHBAUD.all = 0xFF & (brr>>8);
    PLX_SCI_REGS_PTR->SCILBAUD.all = 0xFF & brr;

    // setup FIFO
    PLX_SCI_REGS_PTR->SCIFFTX.all = 0xC000;
    PLX_SCI_REGS_PTR->SCIFFRX.all = 0x0000;
    PLX_SCI_REGS_PTR->SCIFFCT.all = 0x00;

    PLX_SCI_REGS_PTR->SCICTL1.all = 0x0033;  // relinquish SCI from reset *

    // reset FIFO
    PLX_SCI_REGS_PTR->SCIFFTX.bit.TXFIFORESET = 1;
    PLX_SCI_REGS_PTR->SCIFFRX.bit.RXFIFORESET = 1;

	EDIS;

    return true;
}
