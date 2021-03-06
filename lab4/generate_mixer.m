function y = generate_mixer(Fs,F0)
%GENERATE_MIXER Generates look-up table for a mixer, storing it in
%'mixer_coef.c'
% Author: ECE486 Spring 2015 Lab Group 2
% Author: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
%
% This function generates a look-up table for a mixer given a sampling
% frequency and mixer frequency. The table will be of the minimum length
% required to represent an integer number of periods.

% Generate the look-up table values
f0 = F0 / Fs;			% Normalized mixer frequency
[~,N] = rat(f0);		% Periodicity
n = 0:N-1;				% values of n to evaluate at
y = cos(2*pi*f0.*n);	% calculate cosine values

% Dump the mixer look-up table into 'mixer_coef.c'
fd = fopen('mixer_coef.c','w');

% Define Doxygen header
header = {'/*!';
		  ' * @file';
		  ' * ';
		  ' * @brief ECE 486 Spring 2015 Lab 4 - Source file containing ';
		  ' * mixer look-up table.';
		  ' * ';
		  ' * @author ECE486 Lab Group 2';
		  ' * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen';
		  ' *';
		  ' * @date Mar 26, 2015';
		  ' *';
		  ' * This file contains the mixer look-up table values for use';
		  ' * externally. The table is automatically generated by the';
		  ' * "generate_mixer()" function defined in "generate_mixer.m"';
		  ' *';
		  ' * This allows for the automatic testing of a new mixer';
		  ' * by re-running the generate_mixer() function and re-compiling';
		  ' *';
		  ' */'};
% Print Doxygen header to the file
for row=1:size(header,1)
	fprintf(fd,'%s\n',header{row,:});	% print each line
end

% Now add the necessary include and start of the mixer_coef array
fprintf(fd,'\n#include "mixer_coef.h" \n\nint n_mixer = %d;',N);
fprintf(fd,'\n\nfloat mixer_coef[%d] = {', N);
s = '';

% Add all the look-up table values to our output string
for k=1:N
	if k ~= N
		s = strcat(s, sprintf('\n\t%g,',y(k)));
	else
		s = strcat(s, sprintf('\n\t%g',y(k)));
	end
end
s = strcat(s, '};');

fprintf(fd,'%s',s);		% Print the string to the file
fclose(fd);				% Close the file