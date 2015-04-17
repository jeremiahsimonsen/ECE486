/*!
 * @file
 * @brief Create Bell 202 modulated test signal on STM32F4-Discovery Board 
 * @author Don Hummels
 * @date March 17, 2015
 * 
 * 
 * Modulate a fixed bit sequence using and audio FSK modulation format
 *    1200 bits/second
 *    "1": 1200 Hz
 *    "0": 2200 Hz
 * 
 * Bit sequence is hard-coded, and is output on PC5
 * Each time the bit sequence repeats, PC4 is set high for one bit period.
 * 
 * Note that there's a one-bit delay between the digital outputs PC4 and PC5
 * and the actual analog modulated DAC output due to buffering of the DAC
 * output samples.
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
#include <stdio.h>
#include "arm_math.h"
#include "numtostr.h"

#define NBITS 20
#define NSAMP 80

int main (void) {
  
  static int block_count = 0;
  static float block_dt = 0.8333333e-3f;  // 0.833 msec/block
  char num1[23], num2[23], outstr[100];
  
  float *input, *output;
  
  static int bit_sequence[NBITS] = {1,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0};
  static int bit_index=0;
  static int bit_save=0;	// Used to record the bit currently be streamed to the DAC
  
  static float lo_re = 1.0, lo_im = 0.0; // "Local Oscillator" LO: 1700 Hz 
  static float lo_step_re = 0.993816462056378, lo_step_im = 0.111035308554278;
  
  static float y_re=1.0, y_im=0.0;	// Complex rep of output
  
  static float df_re=0.999464587476366, df_im;
  static float df_im_save=0.032719082821776;
  
  static float output_amp = 0.9;
  
  float temp;	// Temp variable for complex multiplies
  float normalize_factor;
  GPIO_InitTypeDef  my_io_pin;
  int i;	
  
  setblocksize( NSAMP );	
  
  initialize(FS_96K, MONO_IN, MONO_OUT);
  
  // Set up PC5 as a digital io pin
  __GPIOC_CLK_ENABLE();			   // Needed for PC5 digital io
  my_io_pin.Mode = GPIO_MODE_OUTPUT_PP;    // Push/Pull digital output
  my_io_pin.Pull = GPIO_NOPULL;            // No pullup or pulldown resistor
  my_io_pin.Speed = GPIO_SPEED_HIGH;       // LOW, MEDIUM, FAST, or HIGH

  my_io_pin.Pin = GPIO_PIN_5;              // Set up PC5
  HAL_GPIO_Init(GPIOC, &my_io_pin);


  input = (float *)malloc(sizeof(float)*NSAMP);		// Unused input array
  output = (float *)malloc(sizeof(float)*NSAMP);	// AFSK Output

  while (1) {
    getblock(input);	//ask for a block of (unused) ADC samples
    
    block_count++;	// Print status every 0.5 seconds
    if (block_count%600 == 0) {
      sprintf(outstr,"Block # %s, Time = %s sec\n",
	             inttostr(block_count, num1), 
	             floattostr(block_count*block_dt, num2, 3) );
      UART_putstr(outstr);
    }
    
    /*
     * Note that the DAC is currently streaming out the PREVIOUS modulated
     * bit (from the last data block).  Here, we set the digital io bits
     * to indicate what is being streamed out.  PC4 should indicate the start
     * of the bit sequence. (When bit_index==1 here we know the DAC is streaming
     * out our bit_index==0 modulated result, and we need to set PC4.)
     * Similarly, PC5 should indicate the bit that was sent to the DAC in the
     * previous block.
     */
    if (bit_index==1) {
      DIGITAL_IO_SET(); 	// PC4 indicates start of the bit sequence
    } else {
      DIGITAL_IO_RESET();
    }
    
    if (bit_save == 0) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    } else {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    }
    bit_save = bit_sequence[bit_index];
    
    /*
     * Set the frequency offset depending on the modulated bit
     */
    if (bit_sequence[bit_index]==0) {
      df_im = df_im_save;	// df = +500 Hz offset
    } else {
      df_im = -df_im_save;	// df = -500 Hz offset
    }
      
    for (i=0; i<NSAMP; i++) {
      temp = y_re*df_re - y_im*df_im;
      y_im = y_re*df_im + y_im*df_re;	// Complex rep of output
      y_re = temp;			// exp(j*2*pi*df*n)
      
      temp = lo_re*lo_step_re - lo_im*lo_step_im;
      lo_im = lo_re*lo_step_im + lo_im*lo_step_re;
      lo_re = temp;			// LO: exp(j*2*pi*f0*n)
      
      output[i] = output_amp*(y_re*lo_re - y_im*lo_im);
    }
    
    bit_index += 1;
    if (bit_index==NBITS) bit_index = 0;
    
    /*
     * Make sure our complex exponentials have magnitude 1.0
     * Do this once per block to avoid slow growth/decay due to rounding errors
     */
    normalize_factor = 1.0f/sqrtf(y_re * y_re + y_im * y_im);
    y_re *= normalize_factor;
    y_im *= normalize_factor;
    normalize_factor = 1.0f/sqrtf(lo_re * lo_re + lo_im * lo_im);
    lo_re *= normalize_factor;
    lo_im *= normalize_factor;

 
    putblock(output);
  }
  free(input);
  free(output);
}


	
	


