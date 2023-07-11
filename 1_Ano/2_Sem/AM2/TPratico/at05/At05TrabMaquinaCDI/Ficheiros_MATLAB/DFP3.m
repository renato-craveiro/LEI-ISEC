% Formula das Diferençs progressivas em 3 pontos
% f'(xi)=(-3*f(x(i)) + 4*f(x(i+1)) - f(x(i+2))/(2*h)
% INPUT:  f - funcao
%         [a, b] - intervalo de derivacao
%         n - número de subintervalos
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f 
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23


function [dydx,x,y]=DFP3(f,a,b,n,y)

h=(b-a)/n;
x=a:h:b;

if nargin==4
    y=f(x);
end

dydx=zeros(1,n);

for i=1:n-1
    dydx(i)=( (-3)*y(i) + 4*y(i+1) - y(i+2) ) / (2*h)
end

dydx(n)=( y(n-3) - 4*y(n-2) + 3*y(n-1) )/(2*h);
dydx(n+1)=( y(n-2) - 4*y(n-1) + 3*y(n) )/(2*h);