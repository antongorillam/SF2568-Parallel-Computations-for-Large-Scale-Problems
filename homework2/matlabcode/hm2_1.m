clc; clear; close all
t = 0:0.001:1;
f1 = csvread('hm2_function01.csv');
plot(t,f1); hold on

x = linspace(0,1,100);
t = linspace(0,1,100);
sol = pdepe(0,@pdefun,@icfun,@bcfun,x,t);
u = sol(:,:,1);
plot(x,u(end,:))
xlabel('x'); ylabel('u(x)')
legend('result by Jacobi iteration','exact solution')
title('r(x) = -x^{2}, f(x) = sin(x)')

function [c,f,s] = pdefun(x,~,u,DuDx)
    r = -x.^2;
    c = 1;
    f = DuDx-sin(x);
    s = r.*u;
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