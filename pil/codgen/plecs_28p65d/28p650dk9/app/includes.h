/*
   Copyright (c) 2021-2023 by Plexim GmbH
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

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#if !defined(_PLEXIM_) || !defined(CPU1)
#error Incorrect project settings!
#endif

#define TARGET_28p65x

#include "f28p65x_device.h"
#include "f28p65x_globalprototypes.h"
#include "f28p65x_pie_defines.h"

#include "driverlib/ipc.h"
#include "driverlib/sysctl.h"
#include "driverlib/cputimer.h"

#include "pil.h"

#include <stdio.h>

#define THIS_TSP_VER 0x0110

#define PLX_ASSERT(x) do {\
   if(!(x)){\
      asm("        ESTOP0");\
      for(;;);\
   }\
} while(0)

#ifndef BUILD_HAL

#if defined(SCI_BAUD_RATE) && defined(SCI_PORT)
#define SCI_CONFIGURED
#endif

#define PARALLEL_COM_BUF_ADDR 0x00CF00
#define PARALLEL_COM_BUF_LEN 0x80

#endif // BUILD_HAL
#endif // _INCLUDES_H_
