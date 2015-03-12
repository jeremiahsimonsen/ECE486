%% ECE486 Lab 3 - Task 2
% Script to design bandpass filter to pass frequencies within 0.10 <= f <=
% 0.25 and attenuate frequencies f<=0.05 and f>=0.32 by at least 35 dB

%% Generate filter coefficients
pf = [.1 .14 .22 .25 .35];			% pole frequencies
r = [.93 .62 .6 .89 .98];
% bw = (0.25 - 0.1)*2*pi;		% bandwidth
% r = 1 - bw/(2+length(pf));	% radius of poles
zf = [0 .05 .32 .35 0.5];				% zero frequencies
p = [r.*exp(1i*2*pi.*pf) r.*exp(-1i*2*pi.*pf)];	% poles
z = [1*exp(1i*2*pi.*zf) 1*exp(-1i*2*pi.*zf)];	% zeros
a = poly(p);
b = poly(z);


f = 0:.001:.5;
num = zeros(1,length(f));
den = zeros(1,length(f));
for k=1:length(a)
	den = den + a(k)*exp(-1i*2*pi*(k-1).*f);
end
for k=1:length(b)
	num = num + b(k)*exp(-1i*2*pi*(k-1).*f);
end
G1 = num ./ den;
% G1 = (b(1) + b(2)*exp(-1i*2*pi*f) + b(3)*exp(-1i*2*pi*2*f))./ ...
% 	 (a(1) + a(2)*exp(-1i*2*pi*f) + a(3)*exp(-1i*2*pi*2*f));

K = 1/max(abs(G1))-.002;
G1 = K .* G1;

figure(1); hold on;
patch([0.1 0.1 0.25 0.25],[-7 -5 0 -2],'g');
patch([0 0 0.04 0.04],[-45 -40 -40 -45],'r');
patch([0.32 0.32 0.5 0.5],[-45 -35 -35 -45],'r');
plot(f,20*log10(abs(G1))); grid on;
axis([0 0.5 -45 5]);
xlabel('Frequency (cycles/sample)'); ylabel('Filter Gain (dB)');