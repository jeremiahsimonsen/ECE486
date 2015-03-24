function Hd = filter1_dc
%FILTER1_DC Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.2 and the Signal Processing Toolbox 6.20.
% Generated on: 24-Mar-2015 00:22:31

% Elliptic Highpass filter designed using FDESIGN.HIGHPASS.

% All frequency values are in Hz.
Fs = 48000;  % Sampling Frequency

Fstop = 60;      % Stopband Frequency
Fpass = 300;     % Passband Frequency
Astop = 80;      % Stopband Attenuation (dB)
Apass = 1;       % Passband Ripple (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.highpass(Fstop, Fpass, Astop, Apass, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match);

% Get the transfer function values.
[b, a] = tf(Hd);
g = prod(Hd.ScaleValues);

% Convert to a singleton filter.
Hd = dfilt.df2(b, a);
a = Hd.Denominator; b = Hd.Numerator;
z = roots(b); p = roots(a);
zp2biquad(z,p,g,'filter1_coef.c');



% [EOF]
