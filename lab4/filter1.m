function [Hd,g] = filter1
%FILTER1_1 Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.2 and the DSP System Toolbox 8.5.
% Generated on: 22-Mar-2015 16:58:18

% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.

% All frequency values are in Hz.
Fs = 48000;  % Sampling Frequency

Fpass = 3400;    % Passband Frequency
Fstop = 6200;    % Stopband Frequency
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
a = Hd.Denominator; b = Hd.Numerator;
z = roots(b); p = roots(a);
zp2biquad(z,p,1,'filter1_coef.c');



% [EOF]
