clc; clear; close all
t = 0:0.001:1;
f2 = csvread('hm2_function02.csv');
plot(t,f2); hold on

x = linspace(0,1,100);
t = linspace(0,1,100);
sol = pdepe(0,@pdefun,@icfun,@bcfun,x,t);
u = sol(:,:,1);
plot(x,u(end,:))
xlabel('x'); ylabel('u(x)')
legend('result by Jacobi iteration','exact solution')
title('r(x) = -x, f(x) = cos(x)')

function [c,f,s] = pdefun(x,~,u,DuDx)
    r = -x;
    c = 1;
    f = DuDx+cos(x);
    s = -r.*u;
end
function u0 = icfun(~)
    u0 = 0;
end
function [pl,ql,pr,qr] = bcfun(~,ul,~,ur,~)
    pl = ul;
    ql = 0;
    pr = ur;
    qr = 0;
end