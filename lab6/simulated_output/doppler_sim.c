/*!
 * @file
 * @brief Generate a Doppler radar test signal 
 * @author Don Hummels
 * @date March 17, 2015
 * 
 * 
 * Step through a fixed sequence of frequencies and amplitudes to simulate
 * changing targets.  N_TONES=4 separate tones are generated at various amplitudes.
 * Frequencies and amplitudes change every second.
 * 
 * The sequence is hard-coded.  Four frequencies are generated, with amplitudes
 * of 0.0, 0.05, 0.10, 0.15.  The frequencies remain fixed, but the amplitudes
 * are rotated every second.
 * 
 * Frequencies correspond to +41, +7, -11, and -37 meters/second, giving 
 *   F = 1.5853e+03   2.7067e+02  -4.2533e+02  -1.4307e+03 Hz. 
 * For a sample rate of 96 ksps, the frequencies are coded below as
 *   f = 1.6514e-02   2.8194e-03  -4.4306e-03  -1.4903e-02  cycles/sample
 * 
 * @addtogroup ece486_fir
 */


#include "stm32f4xx_hal.h"

#ifdef STM32F407xx
#include "stm32f4_discovery.h"
#endif /* STM32F407xx */

#ifdef STM32F429xx
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#endif /* STM32F429xx */

#include "ece486.h"
#include <stdlib.h>
#include "arm_math.h"
#include "ece486_osc.h"

#define N_TONES 4

#define FS FS_48K
#define NSAMP 80
#define BUFFERS_PER_SECOND 1200

int main (void) {
  float *input, *output1, *output2;
  
  static OSCILLATOR_T *osc[N_TONES];
  static float amp[N_TONES] = {0.0, 0.05, 0.10, 0.15};
//  static float amp[N_TONES] = {0.0, 0.0, 0.0, 0.15};
  static float f0[N_TONES] = { 1.6514e-02,   2.8194e-03,  -4.4306e-03,  -1.4903e-02};
  static float dc_offset_re = 0.3, dc_offset_im = -0.3;
  
  float temp;
  int i,j;	
  static int count=0;	// Number of blocks processed... used to change output every second

  setblocksize( NSAMP );	
  
  initialize(FS_96K, MONO_IN, STEREO_OUT);


  input = (float *)malloc(sizeof(float)*NSAMP);		// Unused input array
  output1 = (float *)malloc(sizeof(float)*NSAMP);	// Real part of output
  output2 = (float *)malloc(sizeof(float)*NSAMP);	// Imaginary part of output
  
  for (i=0; i<N_TONES; i++) {
    osc[i] = init_oscillator( f0[i]);
  }

  while (1) {
    getblock(input);	//ask for a block of (unused) ADC samples
    
    if ( ++count == BUFFERS_PER_SECOND ) { // starting a new second... 
      count = 0;
      // Rotate the amplitudes
      temp = amp[0];
      for (i=0; i<N_TONES-1; i++) amp[i] = amp[i+1];
      amp[N_TONES-1] = temp;
      
      // The frequencies are left the same... but we update anyway just to avoid
      // a slow decay in the amplitudes due to rounding.
      for (i=0; i<N_TONES; i++)
	change_oscillator( osc[i], f0[i] );
    }
    
    for (i=0; i<NSAMP; i++) {		
      output1[i] = dc_offset_re;		// Start with the DC offset
      output2[i] = dc_offset_im;
      
      for (j=0; j<N_TONES; j++) {
	output1[i] += amp[j] * osc_re(osc[j]);  // Add in each oscillator output
	output2[i] += amp[j] * osc_im(osc[j]);
	step_oscillator(osc[j]);	// Increment the oscillators for the next sample
      }
    }
     
    putblockstereo(output1, output2);
  }
  
  // Should never get here.
  for (i=0; i<N_TONES; i++) destroy_oscillator(osc[j]);
  free(input);
  free(output1);
  free(output2);
}


	
	


