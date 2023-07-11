% DI_DFDerivada2 Derivação Numérica - fórmula das diferenças finitas em 3 pontos para a 2º derivada
% Formúla das Diferenças finitas em 3 pontos para a 2º derivada
% f''(xi)=(f(x(i+1)) - 2*f(x(i)) - f(x(i-1))/(h^2)
% INPUT:  f - função
%         [a, b] - intervalo de derivação
%         n - número de subintervalos 
%         y - imagens x vs y
% OUTPUT: [x, y] - malha de pontos
%         dydx - derivada de f de 2ªordem
%
%   03/06/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function [dydx,x,y]=DF2D3(f,a,b,n,y)

h=(b-a)/n;
x=a:h:b;

if nargin==4
    y=f(x);
end

dydx=zeros(1,n);

dydx(1)=(y(3)-2*y(2)+y(1))/(h^2);

for i=2:n
    dydx(i)=(y(i+1)-2*y(i)+y(i-1))/(h^2);
end
dydx(n+1)=(y(n)-2*y(n-1)+y(n-2))/(h^2);