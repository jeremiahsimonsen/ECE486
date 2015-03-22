n = linspace(0,100,101);
c = cos(2*pi*1700/9600.*n);
s = -sin(2*pi*1700/9600.*n);
x = linspace(0,100,101);

y_re = x.*c;
y_im = x.*s;

y_re = y_re(:);
y_im = y_im(:);

table(y_re)
table(y_im)