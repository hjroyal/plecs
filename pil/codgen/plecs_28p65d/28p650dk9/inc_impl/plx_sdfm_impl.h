/*
   Copyright (c) 2024 by Plexim GmbH
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

#ifndef PLX_SDFM_IMPL_H_
#define PLX_SDFM_IMPL_H_

#include "sdfm.h"

typedef struct PLX_SDFM_OBJ {
  uint32_t baseAddr;
  SDFM_FilterNumber filterNumber;
  SDFM_FilterType filterType;
  SDFM_ClockSource clkSource;
  SDFM_OutputDataFormat dataFormat;
  uint16_t osr;
  uint16_t shift;
} PLX_SDFM_Obj_t;

typedef PLX_SDFM_Obj_t *PLX_SDFM_Handle_t;

inline void PLX_SDFM_configClockSource(PLX_SDFM_Handle_t aHandle, 
                                       SDFM_ClockSource clkSource) {
  aHandle->clkSource = clkSource;
  SDFM_selectClockSource(aHandle->baseAddr, aHandle->filterNumber, clkSource);
}

#endif // PLX_SDFM_IMPL_H_