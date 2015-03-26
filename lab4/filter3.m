function Hd = filter3
%FILTER3 Returns a discrete-time filter object.
% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
%
% This function generates a low-pass filter designed to filter the
% "frequency reference signal" which is the output of the FSK demodulator
% developed for Lab 4. The frequency reference signal is used to denote
% whether a "1" or a "0" were detected. A "0" will correspond to a +500 Hz
% signal, while a "1" will correspond to a -500 Hz signal. The DAC output
% generated by the frequency estimation block will change by approximately
% 1V for every 1 KHz deviation from 1700 Hz (+ or -). For example: 1700 Hz
% input corresponds to 1.5 V out, 1200 Hz in to 1 V out, 2200 Hz in to 2 V
% out.
%
% This filter was added because it was observed that for inputs below 1700
% Hz, the output signal oscillated. The filter is designed for a sample
% rate of 9600 sps and passes frequencies below 1200 Hz and rejects above
% 2400 Hz. The passband was selected because it is known that the
% instantaneous frequency of the input signal may be changing 1200 times
% per second.

% MATLAB Code
% Generated by MATLAB(R) 8.2 and the Signal Processing Toolbox 6.20.
% Generated on: 25-Mar-2015 22:36:13

% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.

% All frequency values are in Hz.
Fs = 9600;  % Sampling Frequency

Fpass = 1200;    % Passband Frequency
Fstop = 2400;    % Stopband Frequency
Apass = 1;       % Passband Ripple (dB)
Astop = 80;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match);

% Get the transfer function values.
[b, a] = tf(Hd);
g = prod(Hd.ScaleValues);

% Convert to a singleton filter.
Hd = dfilt.df2(b, a);
a = Hd.Denominator; b = Hd.Numerator;	% grab coefficients
z = roots(b); p = roots(a);				% calculate poles/zeros
zp2biquad(z,p,g,'filter3_coef.c');		% dump filter coefficients into file



% [EOF]
