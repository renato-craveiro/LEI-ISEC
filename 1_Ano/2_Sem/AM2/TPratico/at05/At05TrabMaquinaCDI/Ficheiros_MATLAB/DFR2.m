
% Formula das Diferenças regressivas em 2 pontos
% f'(xi)=(f(x(i))-f(x(i-1))/h
% INPUT:  f - função
%         [a, b] - intervalo de deriva玢o
%         n - número de subintervalos
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f 
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function [dydx, x,y]=DFR2(f,a,b,n,y)
h=(b-a)/n;

x=a:h:b;
if nargin==4
    y=f(x);
end
dydx=zeros(1,n);
dydx(1)=(y(2)-y(1))/h;
for i=2:n+1
    dydx(i)=(y(i)-y(i-1))/h;
end