function Hd = filter1_dc
%FILTER1_DC Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.2 and the Signal Processing Toolbox 6.20.
% Generated on: 23-Mar-2015 23:03:35

% Elliptic Bandpass filter designed using FDESIGN.BANDPASS.

% All frequency values are in Hz.
Fs = 48000;  % Sampling Frequency

Fstop1 = 60;      % First Stopband Frequency
Fpass1 = 200;     % First Passband Frequency
Fpass2 = 3400;    % Second Passband Frequency
Fstop2 = 6200;    % Second Stopband Frequency
Astop1 = 80;      % First Stopband Attenuation (dB)
Apass  = 1;       % Passband Ripple (dB)
Astop2 = 80;      % Second Stopband Attenuation (dB)
match  = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.bandpass(Fstop1, Fpass1, Fpass2, Fstop2, Astop1, Apass, ...
                      Astop2, Fs);
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
