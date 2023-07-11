%N_RK2  Método de Runge-Kutta de ordem 2
%   [t, u, v] = NRK2SED(f, g, a, b, n, u0, v0) Método numérico para a
%   resolução de um SED
%   u'= f(t,u,v), v'=g(t,u,v), t=[a, b], u(a)=u0 e v(a)=v0
%
%INPUT:
%   f - função do 2.º membro da ED
%   [a, b] - t inicial e t final
%   n - número de subintervalos
%   u0 - condição inicial u(0)
%   v0 - condição inicial v(0)
%OUTPUT: 
%   [t,u,v] - vector das soluções aproximações e da discretização de t
%   u(i+1) = u(i)+h*f(t(i),u(i),v(i))
%   v(i+1) = v(i)+h*g(t(i),u(i),v(i)) , i =0,1,...,n-1
%
%   30/04/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23


function [t, u, v] = NRK2SED(f, g, a, b, n, u0, v0)
    h=(b-a)/n;
    t=a:h:b;
    u = zeros(1, n+1);
    v = zeros(1, n+1);
    u(1)=u0;
    v(1)=v0;
    
    for i=1:n
        k1u = h * f(t(i), u(i), v(i));
        k1v = h * g(t(i), u(i), v(i));
        
        k2u = h * f(t(i+1), u(i) + k1u, v(i) + k1v);
        k2v = h * g(t(i+1), u(i) + k1u, v(i) + k1v);
        
        u(i+1) = u(i) + (1/2)*(k1u + k2u);
        v(i+1) = v(i) + (1/2)*(k1v + k2v);
    end
end