/*20192014-2016 by Plexim GmbH
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

#ifndef PLX_DAC_H_
#define PLX_DAC_H_

#include "includes.h"
#include "plx_dac_impl.h" // implementation specific

extern void PLX_DAC_sinit();

extern PLX_DAC_Handle_t PLX_DAC_init(void *aMemory, const size_t aNumBytes);

extern void PLX_DAC_configure(PLX_DAC_Handle_t aHandle, PLX_DAC_Unit_t aUnit, bool useExternalVref, float aVref);

extern void PLX_DAC_configureScaling(PLX_DAC_Handle_t aHandle, float aScale, float aOffset, float aMin, float aMax);

extern void PLX_DAC_setValF(PLX_DAC_Handle_t aHandle, float aVal);

extern void PLX_DAC_setVal(PLX_DAC_Handle_t aHandle, uint16_t aVal);

#endif /* PLX_DAC_H_ */
