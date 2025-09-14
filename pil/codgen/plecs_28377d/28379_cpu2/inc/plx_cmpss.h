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

#ifndef PLX_CMPSS_H_
#define PLX_CMPSS_H_

#include "includes.h"

#include "cmpss.h"
//#include "plx_cmpss_impl.h" // implementation specific

typedef struct PLX_CMPSS_OBJ
{
	uint32_t base;
} PLX_CMPSS_Obj_t;

typedef PLX_CMPSS_Obj_t *PLX_CMPSS_Handle_t;

PLX_CMPSS_Handle_t PLX_CMPSS_init(void *aMemory, const size_t aNumBytes)
{
	PLX_CMPSS_Handle_t handle;

	if(aNumBytes < sizeof(PLX_CMPSS_Obj_t)){
		return((PLX_CMPSS_Handle_t)NULL);
  }

	// set handle
	handle = (PLX_CMPSS_Handle_t)aMemory;
	return handle;
}

void PLX_CMPSS_configure(PLX_CMPSS_Handle_t aHandle, uint32_t aCmpssBase){
  PLX_CMPSS_Obj_t *obj = (PLX_CMPSS_Obj_t *)aHandle;
  obj->base = aCmpssBase;
}

inline void PLX_CMPSS_setDacHigh(PLX_CMPSS_Handle_t aHandle, float aVal){
  PLX_CMPSS_Obj_t *obj = (PLX_CMPSS_Obj_t *)aHandle;
  uint32_t base = obj->base;

  float val = 4096.0 / 3.3 * aVal;
  uint16_t valInt = 0;
  if (val > 0xFFF){
      valInt = 0xFFF;
  } else if (val > 0){
      valInt = (uint16_t)val;
  }
  CMPSS_setDACValueHigh(base, valInt);
}

inline void PLX_CMPSS_setPeak(PLX_CMPSS_Handle_t aHandle, float aPeak){
  PLX_CMPSS_Obj_t *obj = (PLX_CMPSS_Obj_t *)aHandle;
  uint32_t base = obj->base;

  float val = 65536.0 / 3.3 * aPeak;
  uint32_t valInt = 0;
  if (val > 0xFFFFFFFF){
      valInt = 0xFFFFFFFF;
  } else if (val > 0){
      valInt = (uint32_t)val;
  }
  if (valInt > 0xFFFF){
      uint16_t decVal = CMPSS_getRampDecValue(base);
      if(decVal != 0){
          uint32_t rampDelay = (valInt-0xFFFF)/decVal;
          if (rampDelay > 0x1FFF){
              rampDelay = 0x1FFF;
          }
          CMPSS_setRampDelayValue(base, (uint16_t)rampDelay);
      }
      valInt = 0xFFFF;
  } else {
      CMPSS_setRampDelayValue(base, 0x0000);     
  }
  CMPSS_setMaxRampValue(base, ((uint16_t)valInt));
}

#endif /* PLX_CMPSS_H_ */
