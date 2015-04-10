% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen

% elliptical filters give lowest order, so they are used by default

Fs = 48e3; % sampling frequency
Wp = [12e3 15e3]; % enter the pass band frequencies (2 for BP, 1 for LP/HP)
Ws = [11.2e3 16e3]; % enter the stop band frequencies (same as above)
Rp = 0.1; % enter pass band ripple in dB
Rs = 70; % enter stop band ripple
[N, Wp] = ellipord(Wp/Fs*2, Ws/Fs*2, Rp, Rs); % find filter order
[b,a] = ellip(N, Rp, Rs, Wp); % find filter
freqz(b,a,1024*4,Fs) % plot it

