clc
clear
clc
h = 60;
j = 60 -15;
ins_deg = 0;
delta = 60;
deg2rid =@(deg) deg.*pi./180;
red2dig =@(rid) rid.*180./pi;
theta1 = deg2rid(ins_deg);
theta2 = theta1 + deg2rid(delta);
func =@(x) (x(1)*cos(theta1) - (x(2)^2 - (h - x(1)*sin(theta1))^2)^0.5)...
    - (x(1)*cos(theta2) - (x(2)^2 - (h - x(1)*sin(theta2))^2)^0.5) - 54;
x0 = [35, 65.9];
options = optimset('Algorithm','Levenberg-Marquardt');
A = fsolve(func,x0,options);

[h, ins_deg, delta]

res = A
error1 = func(res)
servo_spot = -(res(1)*cos(theta2) - (res(2)^2 - (h - res(1)*sin(theta2))^2)^0.5)+11

upperbound = 60.008; %deg
lowerbound = 0.013 ;%deg

[h, lowerbound, upperbound]
k = [round(res(1)*100)*0.01, round(res(2)*100)*0.01]
error2 = func(k)
Odot = round((servo_spot-11)*100)*0.01

sp_c = 0.3; %N/mm
torque = @(th) 2*sp_c.*(Odot + k(1).*cos(deg2rid(th)) - (k(2)^2 - (h -k(1).*sin(deg2rid(th))).^2).^0.5)... %kx
    .*(k(2) ./ ((k(2)^2 - (h - k(1).*sin(deg2rid(th))).^2).^0.5))...
    .* cos(deg2rid(90) - deg2rid(th) - acos((k(2).^2 - (h - k(1).*sin(deg2rid(th))).^2).^0.5 ./ k(2)))...
    .*(k(1).*0.1)./9.8; %kgf*cm
    
th = 0:0.001:60.008;

max_torque = @(p) -torque(p);
[min_x, min] = fminbnd(torque ,lowerbound, upperbound);
[max_x, max] = fminbnd(max_torque,lowerbound, upperbound);
min_dot = [min_x, min]
max_dot = [max_x, -max]

f1 = figure;
plot(th, torque(th),'b',max_x, -max, 'rx',min_x, min, 'ro');

title('torque')
xlabel('deg')
ylabel('Kgf * cm')

%기동토크
theta_dotdot = deg2rid(0.6*100); % rid/sec^2
tqr = @(thp) -(1.*(-(0.001.*k(1)).*theta_dotdot.*sin(deg2rid(thp)) - (2.*0.001.*k(1).*theta_dotdot.*cos(deg2rid(thp))...
   - ((0.001.*k(1)).^2).*sin(2.*deg2rid(thp)).*theta_dotdot) / ((0.001.*k(2)).^2 - (0.001.*h - 0.001.*k(1).*sin(deg2rid(thp))).^2).^0.5))...
    .*(k(2) ./ ((k(2)^2 - (h - k(1).*sin(deg2rid(thp))).^2).^0.5))...
    .* cos(deg2rid(90) - deg2rid(thp) - acos((k(2).^2 - (h - k(1).*sin(deg2rid(thp))).^2).^0.5 ./ k(2)))...
    .*(k(1).*0.1)./9.8
max_tqr = @(p) -tqr(p);
[min_x2, min2] = fminbnd(tqr ,lowerbound, upperbound);
[max_x2, max2] = fminbnd(max_tqr,lowerbound, upperbound);

min_dot2 = [min_x2, min2]
max_dot2 = [max_x2, -max2]
for n = 1:60009;
    y(n) = tqr((n-1)/1000);
end
f2 = figure;
plot(th,y,'b',min_x2,min2,'ro',max_x2,-max2,'rx');
title('순간 기동 토크')
xlabel('deg')
ylabel('Kgf * cm')
