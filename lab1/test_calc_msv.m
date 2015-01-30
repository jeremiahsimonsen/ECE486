%% Test Script for the calc_msv function
% This Matlab script calls the calc_msv function with arguments of various
% dimensions to make sure that things are working as expected.
%
% Authors: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% ECE486 Group 2, Spring 2015
% January 30, 2015
clear all;

%% Test when M < blocksize
% Should match:
% y =
%     0.3333
%     1.6667
%     4.6667
%     9.6667
%    16.6667
%    25.6667
%    36.6667
% 
% s = 
% 
%               M: 3
%       blocksize: 7
%     prevSquared: 85
%         history: [2x1 double]
%         samples: 0
%       histIndex: 2

x1 = [1 2 3 4 5 6 7];
s = init_msv(3,7);
[y,s] = calc_msv(x1,s);
y,s

%% Test when M = blocksize
% Should match:
% y =
% 
%     0.3333
%     1.6667
%     4.6667
%     9.6667
%    16.6667
%    25.6667
%    36.6667
%    49.6667
%    64.6667
   
x1 = [1 2 3];
x2 = [4 5 6];
x3 = [7 8 9];
s = init_msv(3,3);
[y1,s] = calc_msv(x1,s);
[y2,s] = calc_msv(x2,s);
[y3,s] = calc_msv(x3,s);
y = [y1;y2;y3]

%% Test when M > blocksize

x1 = [1 2 3];
x2 = [4 5 6];
x3 = [7 8 9];

s = init_msv(6, 3);
[y1,s] = calc_msv(x1,s);
[y2,s] = calc_msv(x2,s);
[y3,s] = calc_msv(x3,s);
y = [y1; y2; y3]