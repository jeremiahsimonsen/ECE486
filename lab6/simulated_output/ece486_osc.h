// Complex Oscillator

#ifndef ECE486_IQ_OSC_
#define ECE486_IQ_MIXER_

typedef struct osc_struct {
    float    re;        //!< Current real component of oscillator output
    float    im;        //!< Current imaginary imaginary of oscillator output
    float    re_factor; //!< Real component of exp(j*2*PI*f0)
    float    im_factor; //!< Imaginary component of exp(j*2*PI*f0)
    } OSCILLATOR_T;
    
OSCILLATOR_T *init_oscillator(float f0);

void step_oscillator(OSCILLATOR_T *osc);

void change_oscillator( OSCILLATOR_T *osc, float new_f0);

void destroy_oscillator(OSCILLATOR_T *osc);

static inline float osc_re(OSCILLATOR_T *osc) {
  return osc->re;
}

static inline float osc_im(OSCILLATOR_T *osc) {
  return osc->im;
}

#endif