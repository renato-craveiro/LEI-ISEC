function [dydx,x,y] = DFP(f,a,b,n,y)
%DFP - implemta o mét. das Dif. Finita Progressiva
%  f'(xi)=(y(i+1)-y(i))/h
%% INPUT:  f - funcao
%         [a, b] - intervalo de derivacao
%         n - número de subintervalos
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f 
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

    h=(b-a)/n;
    x=a:h:b;
    
    if nargin == 4
        y=f(x);
    end
    
    dydx = zeros(1,n+1);
    
    
    for i=1:n
        dydx(i)=(y(i+1)-y(i))/h;
    end
    dydx(n+1) = dydx(n);
end