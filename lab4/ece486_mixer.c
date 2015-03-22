/*****************************************************************


 file: ece486_mixer.c
 Description:  Subroutines to implement a (real) mixer.
                 Multiple subroutine calls are used to create a signal
                     y(n) = x(n)c(n)
                 where x(n) is the nth input sample, and the values of c(n) 
                 obtained from a (circularly accessed) array of fixed values.
                 (Typically, the array c(n) is initialize to give samples of a 
                 cosine or sine function.)

 Implementation:
   Mixers are implemented using three functions:
     init_mixer() is called once, and is used to initialize the array
                  of mixer samples c(n), allocate any required memory,
                  and perform any other required initialization.
     calc_mixer() is called multiple times -- once for every block of input samples.
                  It returns the mixer output samples y(n).
     destroy_mixer() is called once at the end of the program, and is used
                  to de-allocate any memory.

  Function Prototypes and parameters:

        #include "mixer.h"
        MIXER_T *init_mixer(float *mixer_coefs, int n_coef, int blocksize);

           Inputs:
                mixer_coefs     pointer to the array of values for c(n)
                                The mixer_coefs array is NOT modified by
                                init_mixer(), calc_mixer(), or destroy_mixer().
                n_coef          Number of samples in the mixer_coefs array
                blocksize	Block size for input sample blocks.
           Returned:
                The function returns a pointer to a "MIXER_T" data type
                (which is defined in "mixer.h")
                
        void calc_mixer(MIXER_T *s, float *x, float *y );

           Inputs:
                s       pointer to MIXER_T, as provided by init_mixer()
                x       Array of input sample values
                	
           Returned:
                On return, y[i] is replaced by the value of x[i]*c[n], where
                c(n) is obtained sequentially from the mixer_coefs array of 
                values.  Each output sample uses the next value from the 
                mixer_coefs array until the end of the array is reached.  The
                index into the mixer_coefs array is then reset to the beginning
                of the array.
                
           Note: the array y must be allocated by the caller.
                
        void destroy_mixer(MIXER_T *s);   
           Inputs:
                s       pointer to MIXER_T, as provided by init_mixer()
           No value is returned.
 
*******************************************************************/ 

#include "ece486_mixer.h"
#include <stdlib.h>

MIXER_T *init_mixer(float *mixer_coefs, int n_coef, int blocksize){

    //Allocate the mixer structure
    MIXER_T *s;
    s = (MIXER_T *) malloc(sizeof(MIXER_T));
    if (s == NULL) return NULL;

    //Initialize the structure
    s->blocksize = blocksize;
    s->n_coef = n_coef;
    s->c = mixer_coefs;
    s->m_index = 0;

    return s;

}

void calc_mixer(MIXER_T *s, float *x, float *y){

    int i;

    for(i=0;i<s->blocksize;i++){

        //Calculate the output and update the index
        y[i] = x[i]*(s->c[s->m_index]);
        s->m_index++;

        //Circle back to the beginning of the coefficient array
        if(s->m_index == s->n_coef){
            s->m_index = 0;
        }

    }

}

void destroy_mixer(MIXER_T *s){

    //Free the pointer to the mixer coefficients
    s->c = NULL;

    //Free the mixer structure
    free(s);
    s = NULL;

}


