
% Formula das Diferenças centradas em 3 pontos
% f'(xi)=(f(x(i+1))-f(x(i-1))/(2*h)
% INPUT:  f - funcao
%         [a, b] - intervalo de derivacao
%         n - número de subintervalos
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f 
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function [dydx, x,y]=DFC3(f,a,b,n,y)

h=(b-a)/n;
x=a:h:b;

if nargin==4
    y=f(x);
end
dydx=zeros(1,n);
dydx(1)=(y(2)-y(1))/h;
for i=2:n
    dydx(i)=(y(i+1)-y(i-1))/(2*h);
end
dydx(n+1)=(y(n)-y(n-1))/h;