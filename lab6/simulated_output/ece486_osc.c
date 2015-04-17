
#include <stdlib.h>
#include "arm_math.h"
#include "math.h"
#include "ece486.h"
#include "ece486_osc.h"

OSCILLATOR_T *init_oscillator(float f0)
{
  OSCILLATOR_T *osc;
  
  osc = (OSCILLATOR_T *) malloc( sizeof(OSCILLATOR_T) );
  if (osc==NULL) {
    flagerror(MEMORY_ALLOCATION_ERROR);
    while(1);
  }
  
  osc->re = 1.0f;
  osc->im = 0.0f;
  
  osc->re_factor = arm_cos_f32(2*M_PI*f0);
  osc->im_factor = arm_sin_f32(2*M_PI*f0);
  
  return(osc);
}

void change_oscillator( OSCILLATOR_T *osc, float new_f0) //!< Update the osc freq
{
  float normalize_factor;
  
  osc->re_factor = arm_cos_f32(2*M_PI*new_f0);  // New frequency
  osc->im_factor = arm_sin_f32(2*M_PI*new_f0);
  
  /*
   * Periodically  make sure the magnitude of 
   * the oscillator is not growing or decaying due to rounding errors.  
   * In theory, our stored value of exp(j*2*pi*f0*n) should be 1.  Here, we make
   * the magnitude 1 to avoid long-term accumulation of any rounding error.
   */
  normalize_factor = 1.0f/sqrtf(osc->re * osc->re + osc->im * osc->im);
  osc->re *= normalize_factor;
  osc->im *= normalize_factor;

}

void step_oscillator(OSCILLATOR_T *osc)
{
  float temp;
  temp = osc->re;
  osc->re = osc->re * osc->re_factor - osc->im * osc->im_factor;
  osc->im = temp * osc->im_factor + osc->im * osc->re_factor;
}
 

void destroy_oscillator(OSCILLATOR_T *osc)
{
  free(osc);
}
