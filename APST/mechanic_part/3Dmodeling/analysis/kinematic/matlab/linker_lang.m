clc
clear
clc
h = 50
j = 50 -15
delta = 150;
theta = pi-(delta/180)*pi;
func =@(x) (1+cos(theta)) *x(1) + (x(2)^2-h^2)^0.5 - (x(2)^2-(h-sin(theta)*x(1))^2)^0.5-54;
x0 = [40, 60];
options = optimset('Algorithm','Levenberg-Marquardt');
A = fsolve(func,x0,options);
res = A
error = func(A)
-res(1) - (res(2)^2 - 50^2)^0,5
(res(2)^2-50^2)^0.5