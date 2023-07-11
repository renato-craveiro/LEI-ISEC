%NAdamsMoulton Método de Adams-Bashford.
%   y = NAdamsMoulton(f, a, b , N, y0) Método numérico para a resolução de um PVI
%   y'= f(t,y) com t=[a, b] e y(a)=y0 condição inicial  
%
%INPUT:
%   f - função do 2.º membro da Equação Diferencial
%   [a, b] - extremos do intervalo da variável independente t
%   n - número de subintervalos ou iterações do método
%   y0 - condição inicial t=a -> y=y0
%OUTPUT: 
%   y - vector das soluções aproximações
%
%   16/04/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function y = NAdamsBashford(f, a, b , N, y0)

% Define o tamanho h

h = (b - a)/N;

t= a:h:b; %define os valores de 7

y = zeros(1, N+1); %inicializa y
y(1) = y0; %CI

% Usa o método de RK2 para obter valor seguinte
 k1 = h * f(t(1), y(1));             % Inclinação inicial do intervalo
 k2 = h * f(t(2), y(1) + k1);    % Inclinação final do intervalo
 y(2)=y(1) + (k1 + k2)/2;

%y(2) = y(1) + h*f(t(1), y(1));

% Usa o método de Adams-Moulton para os passos subsequentes
    for i = 2:N
        y(i+1) = y(i) + (h/2)*(3*f(t(i), y(i)) + f(t(i-1), y(i-1)));
    end
end


