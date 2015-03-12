function zp2biquad(z,p,k)
[sos, g] = zp2sos(z,p,k);
b = sos(:,1:3);
a = sos(:,4:6);

dec = sprintf('extern float a_coef[][3];\nextern float b_coef[][3];\nextern float g;');
fd = fopen('biquad_coef.h','w');
fprintf(fd,'%s',dec);
fclose(fd);

def = sprintf('#include "biquad_coef.h"\nfloat b_coef[%d][3] = {', ...
	length(b(:,1)));
for n=1:length(b(:,1))
	if n ~= length(b(:,1))
		def = strcat(def, sprintf('{%g, %g, %g},\n',b(n,1),b(n,2),b(n,3)));
	else
		def = strcat(def, sprintf('{%g, %g, %g}};\n',b(n,1),b(n,2),b(n,3)));
	end
end
def = strcat(def, sprintf('\nfloat a_coef[%d][3] = {',length(a(:,1))));
for n=1:length(a(:,1))
	if n ~= length(a(:,1))
		def = strcat(def, sprintf('{%g, %g, %g},\n',a(n,1),a(n,2),a(n,3)));
	else
		def = strcat(def, sprintf('{%g, %g, %g}};\n',a(n,1),a(n,2),a(n,3)));
	end
end
def = strcat(def, sprintf('\nfloat g = %g;\n',g));

fd = fopen('biquad_coef.c','w');
fprintf(fd,'%s',def);
fclose(fd);