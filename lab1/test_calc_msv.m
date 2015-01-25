clear all;

%% Test when M < blocksize
x1 = [1 2 3 4 5 6 7];
s = init_msv(3,7);
[y,s] = calc_msv(x1,s);
y,s

%% Test when M > blocksize

x1 = [1 2 3];
x2 = [4 5 6];
x3 = [7 8 9];

s = init_msv(6, 3);
[y1,s] = calc_msv(x1,s);
[y2,s] = calc_msv(x2,s);
[y3,s] = calc_msv(x3,s);
y = [y1; y2; y3]