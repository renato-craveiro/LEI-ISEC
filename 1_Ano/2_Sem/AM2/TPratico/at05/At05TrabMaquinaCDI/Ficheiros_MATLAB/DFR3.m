
% Formula das Diferenças regressivas em 3 pontos
% f'(xi)=(f(x(i-2))-4*f(x(i-1)+3*f(x(i)))/(2*h)
% INPUT:  f - funcao
%         [a, b] - intervalo de deriva玢o
%         n - número de subintervalos
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f 
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function [dydx, x,y]=DFR3(f,a,b,n,y)

h=(b-a)/n;
x=a:h:b;

if nargin==4
    y=f(x);
end
dydx=zeros(1,n);
dydx(1)=( (-3)*y(1) + 4*y(2) - y(3) )/(2*h);
dydx(2)=( (-3)*y(2) + 4*y(3) - y(4) )/(2*h);
for i=3:n+1
	dydx(i)=( y(i-2) - 4*y(i-1) + 3*y(i) )/(2*h);
end