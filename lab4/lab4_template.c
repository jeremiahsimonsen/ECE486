/*
 * Pseudo-code template for ECE-486 Lab 4
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
