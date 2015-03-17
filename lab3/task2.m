%% ECE486 Lab 3 - Task 2
% Script to design bandpass filter to pass frequencies within 0.10 <= f <=
% 0.25 and attenuate frequencies f<=0.05 and f>=0.32 by at least 35 dB.
%
% Authors: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% ECE 486 - Laboratory 3
% March 10, 2015

%% Generate filter coefficients
pf = [.2 .25 .3 .36 .4];			% pole frequencies
r = [.91 .5 .5 .4 .93];			% pole radii/magnitude
zf = [0 .1 0.48 0.5];			% zero frequencies
p = [r.*exp(1i*2*pi.*pf) r.*exp(-1i*2*pi.*pf)];	% generate actual poles
z = [1*exp(1i*2*pi.*zf) 1*exp(-1i*2*pi.*zf)];	% generate actual zeros
a = poly(p);					% denominator polynomial coefficients
b = poly(z);					% numerator polynomial coefficients

%% Evaluate the transfer function
% Using loops allows for any number of poles and zeros in the calculation.
% There really should be some error checking (poles > zeros, etc.)
f = 0:.001:.5;				% Frequency span
num = zeros(1,length(f));	% Numerator - to be evaluated at each f
den = zeros(1,length(f));	% Denominator - to be evaluated at each f
% Calculate denominator
for k=1:length(a)
	den = den + a(k)*exp(-1i*2*pi*(k-1).*f);
end
% Calculate numerator
for k=1:length(b)
	num = num + b(k)*exp(-1i*2*pi*(k-1).*f);
end
G1 = num ./ den;

% For reference, 2nd order looks like:
% G1 = (b(1) + b(2)*exp(-1i*2*pi*f) + b(3)*exp(-1i*2*pi*2*f))./ ...
% 	 (a(1) + a(2)*exp(-1i*2*pi*f) + a(3)*exp(-1i*2*pi*2*f));

% Adjust with a gain factor
K = 1/max(abs(G1))-.0078;
G1 = K .* G1;

%% Generate the transfer function plot
figure(1); hold on;
% Patches to verify specifications
patch([0.2 0.2 0.4 0.4],[-4 -2 -7 -9],'g');
patch([0 0 0.1 0.1],[-45 -40 -40 -45],'r');
patch([0.45 0.45 0.5 0.5],[-45 -40 -40 -45],'r');
% Plot the filter transfer function
plot(f,20*log10(abs(G1))); grid on;
axis([0 0.5 -45 5]);
xlabel('Frequency (cycles/sample)'); ylabel('Filter Gain (dB)');