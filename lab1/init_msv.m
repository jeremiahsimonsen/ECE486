function s = init_msv(M,blocksize)
%INIT_MSV Initializes structure for running mean-square value 
%   s = init_msv(M, blocksize) initializes a structure containing all
%   necessary fields to compute the running mean squared value of a block
%   of samples 'blocksize' in length, over the most recent 'M' samples. The
%   field 'prevSquared' is a running sum of the squares of the most recent
%   M samples. 'history' is a circular buffer containing the last M-1
%   elements of the sample, necessary in calculating the running mean
%   squared value for subsequent calls of the calc_msv()function.
%   'histIndex' is the current location in the circular history buffer.
%
%   Example: If M = 4 and blocksize = 10 then
%                s = init_msv(4,10) 
%           returns s = 
%				M: 4
%				blocksize: 10
%				prevSquared: 0
%				history: [0 
%						  0 
%						  0]
% 				histIndex: 1.

% Authors: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% ECE 486 - Laboratory 1
% January 30, 2015

s = struct( 'M',M, ...
			'blocksize',blocksize, ...
			'prevSquared', 0, ...
			'history', zeros(M-1,1), ...
			'samples', 0, ...
			'histIndex',1);

end
