%%FIR_FILTER_DESIGN Generates the FIR filter coefficients, or impulse
% response, to meet given design specifications
%
% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen

%%
N = 16*1024;
Fs = 48e3;
M = 75;
% f = (0:N-1)/N;
% f(N/2+2:end) = f(N/2+2:end)-1;
f = linspace(-0.5,0.5,N);
F = f*Fs;
Hr = (abs(f)<.0955)*1.01;
hr = ifft(Hr);
figure(1); clf;
stem(0:N-1,abs(hr),'.');
Hd = Hr.*exp(-1i*2*pi*f*(M-1)/2);
hd = real(ifft(Hd));
h = hd(1:M).*kaiser(M,8)';
hold on;
stem(1:M,h,'r.');
figure(2); clf; hold on;
patch([-.07 -.07 .07 .07],[-.1 .1 .1 -.1],'g');
patch([-.5 -.5 -.13 -.13],[-150 -80 -80 -150],'r');
patch([.5 .5 .13 .13],[-150 -80 -80 -150],'r');
plot(f,20*log10(abs(fft(h,N))));
axis([-.5 .5 -150 10]);