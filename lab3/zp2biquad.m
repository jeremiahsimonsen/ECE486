function zp2biquad(z,p,k)
[sos, g] = zp2sos(z,p,k);
b = sos(:,1:3);
a = sos(:,4:6);

s = sprintf('#include "biquad_coef.h"\nfloat b_coef[%d][3] = {', ...
	length(b(:,1)));
for n=1:length(b(:,1))
	if n ~= length(b(:,1))
		s = strcat(s, sprintf('{%g, %g, %g},\n',b(n,1),b(n,2),b(n,3)));
	else
		s = strcat(s, sprintf('{%g, %g, %g}};\n',b(n,1),b(n,2),b(n,3)));
	end
end
s = strcat(s, sprintf('\nfloat a_coef[%d][3] = {',length(a(:,1))));
for n=1:length(a(:,1))
	if n ~= length(a(:,1))
		s = strcat(s, sprintf('{%g, %g, %g},\n',a(n,1),a(n,2),a(n,3)));
	else
		s = strcat(s, sprintf('{%g, %g, %g}};\n',a(n,1),a(n,2),a(n,3)));
	end
end
s = strcat(s, sprintf('\nfloat g = %g;\n',g));

fd = fopen('biquad_coef.c','w');
fprintf(fd,'%s',s);
fclose(fd);