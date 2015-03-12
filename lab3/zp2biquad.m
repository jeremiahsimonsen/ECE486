function zp2biquad(z,p,k)
[sos, g] = zp2sos(z,p,k);
b = sos(:,1:3);
a = sos(:,4:6);

% float a[2][3] = {{1.0, -0.6088, 0.9702},
%            {1.0, -0.3500, -0.4250}};
s = sprintf('float b[%d][3] = {',length(b(:,1)));
for n=1:length(b(:,1))
	if n ~= length(b(:,1))
		s = strcat(s, sprintf('{%g, %g, %g},\n',b(n,1),b(n,2),b(n,3)));
	else
		s = strcat(s, sprintf('{%g, %g, %g}};\n',b(n,1),b(n,2),b(n,3)));
	end
end
s = strcat(s, sprintf('\nfloat a[%d][3] = {',length(a(:,1))));
for n=1:length(a(:,1))
	if n ~= length(a(:,1))
		s = strcat(s, sprintf('{%g, %g, %g},\n',b(n,1),b(n,2),b(n,3)));
	else
		s = strcat(s, sprintf('{%g, %g, %g}};\n',b(n,1),b(n,2),b(n,3)));
	end
end
s