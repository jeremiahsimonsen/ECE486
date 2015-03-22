/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 4 Real-time implementation of an FSK detector
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 22, 2015
 * 
 * This file contains the real-time implementation of an FSK detector for a 
 * Bell 202 half-duplex modem. A 1200 Hz tone denotes a binary 1 and a 2200 Hz
 * tone denotes a binary 0. Binary data is transmitted at 1200 bits/second. The
 * detector in this lab would be combined with a UART to form the core of the 
 * Bell 202 Modem.
 *
 * An input on pin AIN1 is sampled at 48 ksps. The signal is lowpass filtered
 * to extract the data signal, which is bandwidth limited to 3.4 kHz. Audio 
 * tones above 3.4 kHz are rejected. The signal is passed through a second
 * to reject any impulses at DC caused by the signal not be centered at 1.5 V.
 * The sample sequence is decimated, effectively reducing the sample rate by a
 * factor of 5, to 9.6 ksps. The signal is mixed with a complex exponential 
 * (frequency of 1700 Hz - halfway between "0" and "1"), shifting the spectrum
 * down by 1.7/9.6 cycles/sample. The signal is lowpass filtered again to
 * reject duplicate bands. The frequency of the signal is then estimated and
 * a voltage proportional to the distance from the center frequency is output
 * to pin AOUT1
 * 
 */

// Define the decimation rate...
#define D1 10


/*
 * Includes, Defines, Globals, Static
 * (Filter coef's etc.)
 */
  /********* Your code here *********/

int main(void)
{
  /*
   * Local Variable Definitions 
   */
  /********* Your code here *********/
  
  
  /*
   * Optional: set the ADC/DAC Block Size to anything down to one sample
   * (Omitting this call uses the default block size of 100 samples per data block)
   * The code below assumes that MY_NSAMP is a multiple of D1... otherwise, 
   * you'll have to modify the decimation code to avoid discontinuities between 
   * blocks of input data.
   */
  setblocksize( MY_NSAMP );

  /* 
   * Set up the required 50 ksps sample rate... Use the ADC to measure 
   * a function generator or a microphone input
   */
  initialize(FS_48K, MONO_IN, STEREO_OUT);       // Set up: ADC input, DAC output
  
  /*
   * Get the actual sampling frequncy 
   * (It can be slightly different from the requested value)
   */
  fs = getsamplingfrequency();
  
  /*
   * Allocate Required Memory, initialize filters, mixers, etc.
   */
  /********* Your code here *********/

  /*
   * Infinite Loop to process the data stream "MY_NASMP" samples at a time
   */
  while(1){
    /*
     * Ask for a block of ADC samples to be put into the working buffer
     *   getblock() will wait until the input buffer is filled...  On return
     *   we work on the new data buffer.
     */
    getblock(input);	// Wait here until the input buffer is filled... 
    
    /*
     * Stage 1:  Complete processing at the incoming sample frequency fs.
     *           (Array size MY_NSAMP samples)
     */
    /********* Your code here *********/
    
    /* 
     * Decimate by D1
     */
    for (i=0; i<MY_NSAMP/D1; i++) 
      stage2_input[i] = stage1_output[i*D1];
    
    /*
     * Stage 2:  Complete processing at the intermediate sample rate fs/D1.
     *           (Array size MY_NSAMP/D1 samples)
     */
    /********* Your code here *********/
    
    
    
    /* 
     * Write output values to the DAC....  NOTE: Be sure to set the output
     * array to values for every INPUT sample (not just samples at the decimated 
     * rates!
     */
    for (i=0; i<MY_NSAMP/D1; i++) {
      // Every stage-3 output should be written to D1 output samples!
      for (j=0; j<D1; j++) {
	output1[i*D1+j] = stage2_output_re[i];
	output2[i*D1+j] = stage2_output_im[i];
      }
    }
    
    /*
     * pass the (length MY_NSAMP) calculated buffers back for DAC output
     */
    putblockstereo(output1, output2);
    
    /*
     * Other processing performed once per input block...
     * (Watch for button presses?  Decide which LEDs should be lit?
     */
    /********* Your code here *********/
      
  }
}
