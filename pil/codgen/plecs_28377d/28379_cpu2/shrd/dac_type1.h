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

#ifndef PLX_DAC_TYPE1_H_
#define PLX_DAC_TYPE1_H_

typedef enum PLX_DAC_UNIT {
  PLX_DAC_A = 0,
  PLX_DAC_B,
  PLX_DAC_C
} PLX_DAC_Unit_t;

typedef struct PLX_DAC_OBJ {
  volatile struct DAC_REGS *dac;
  float scale;
  float offset;
  uint16_t min;
  uint16_t max;
  float voltsPerDacBit;
} PLX_DAC_Obj_t;

typedef PLX_DAC_Obj_t *PLX_DAC_Handle_t;

inline void PLX_DAC_setVal(PLX_DAC_Handle_t aHandle, uint16_t aVal) {
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;
  if (aVal > 0xFFF) {
    aVal = 0xFFF;
  }
  obj->dac->DACVALS.all = aVal;
}

#endif /* PLX_DAC_TYPE1_H_ */
