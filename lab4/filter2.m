function Hd = filter2
%FILTER2 Returns a discrete-time filter object.
% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
%
% This function generates a low-pass filter for the purpose of filtering
% the complex output of the mixer in the FSK demodulator developed for Lab
% 4. It selects the frequency band of interest, allowing for frequency
% estimation calculations.
%
% The filter is designed for a sample rate of 9.6 ksps and passes up to
% 1700 Hz while rejecting above 4500 Hz. The passband was chosen to select
% the band of frequencies containing the signal of interest and the
% stopband was selected to avoid aliasing with copies of the spectrum.

% MATLAB Code
% Generated by MATLAB(R) 8.2 and the DSP System Toolbox 8.5.
% Generated on: 22-Mar-2015 17:58:38

% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.

% All frequency values are in Hz.
Fs = 9600;  % Sampling Frequency

Fpass = 1700;    % Passband Frequency
Fstop = 4500;    % Stopband Frequency
Apass = 1;       % Passband Ripple (dB)
Astop = 80;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match);

% Get the transfer function values.
[b, a] = tf(Hd);
g = prod(Hd.ScaleValues);				% Calculate gain factor

% Convert to a singleton filter.
Hd = dfilt.df2(b, a);
a = Hd.Denominator; b = Hd.Numerator;	% Grab coefficients
z = roots(b); p = roots(a);				% Calculate poles/zeros
zp2biquad(z,p,g,'filter2_coef.c');		% Dump filter coefs into file


% [EOF]
