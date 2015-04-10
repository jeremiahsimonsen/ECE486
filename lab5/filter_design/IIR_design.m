% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% Date: 04/10/15
%
% This script gives a basic outline to design an elliptical filter.
% Elliptical filters give lowest order, so they are used by default.
% 
% This script is currently setup to design a bandpass filter that
% that has less than 0.1 dB ripple in the pass band of 12 kHz to 15 kHz.
% A stop band of 70 dB is achieved beyond 11.2 kHz and under 16 kHz.
%
% The values can easily be modified to design other band pass and low pass
% filters.

Fs = 48e3; % sampling frequency
Wp = [12e3 15e3]; % enter the pass band frequencies (2 for BP, 1 for LP/HP)
Ws = [11.2e3 16e3]; % enter the stop band frequencies (same as above)
Rp = 0.1; % enter pass band ripple in dB
Rs = 70; % enter stop band ripple
[N, Wp] = ellipord(Wp/Fs*2, Ws/Fs*2, Rp, Rs); % find filter order
[b,a] = ellip(N, Rp, Rs, Wp); % find filter
freqz(b,a,1024*4,Fs) % plot it

