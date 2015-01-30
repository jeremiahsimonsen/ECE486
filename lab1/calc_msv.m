function [y,s] = calc_msv(x,s)
%CALC_MSV Calculates the "running mean-square value" of a sample vector
%	[y,s] = calc_msv(x,s) calculates the running mean square value(msv) of
%	the sample vector 'x', storing the msv in vector 'y'. Information for
%	the calculation is stored in the structure 's', such as the number of
%	samples to average over, 'M', the last M-1 samples in any previous
%	blocks of 'blocksize' samples are stored in 'history', the current
%	index of history is stored in 'histIndex', and a running sum of the
%	squares of the previous M-1 samples is stored in 'prevSquared'.
%
%	Example: If s = 
%					M: 3, 
%					blocksize: 7,
%					prevSquared: 0, 
%					history: [0
%							  0]
% 					histIndex: 1
%	and x = [1 2 3 4 5 6 7]' then
%					[y,s] = calc_msv(x,s)
%				returns y = [1
%							 2.5
% 							 4.6667
% 							 9.6667
% 							 16.6667
% 							 25.6667
% 							 36.6667]
% 						
% 						s = 
% 							M: 3,
% 							blocksize: 7,
% 							prevSquared: 85,
% 							history: [7
% 									  6]
% 							histIndex: 2. 

% Authors: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% ECE 486 - Laboratory 1
% January 30, 2015

x = x(:);	% Force the input sample vector to be a column vector

for n=1:size(x,1) % index through the input array
	% Calculate y(n)	
	y(n,1) = (s.prevSquared + x(n)^2) / s.M;				% average over M samples
	
	s.prevSquared = s.prevSquared - s.history(s.histIndex)^2;	% subtract (the oldest value)^2
	s.prevSquared = s.prevSquared + x(n)^2;						% add (the new value)^2
	s.history(s.histIndex) = x(n);								% update value in history buffer
	s.histIndex = s.histIndex + 1;								% next spot in the buffer is now the oldest
	
	% Handle looping back around to the beginning of the history buffer.
	% Avoids using modulus just in case it is less efficient on the
	% processor
	if (s.histIndex == s.M)										% reached the end
		s.histIndex = s.histIndex - (s.M - 1);					% go to beginning
	end
	
end

end