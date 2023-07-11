%NRK2  Método de Heun.
%   y = NHeun(f,a,b,n,y0) Método numérico para a resolução de um PVI
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
%   28/04/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function y = NHeun(f,a,b,n,y0)
    h = (b-a)/n;                     % Tamanho de cada subintervalo (passo)
    
    t = a:h:b;                       % Alocação de memória - vetor das abcissas
    y = zeros(1, n+1);               % Alocação de memória - vetor das ordenadas
    
   
    y(1) = y0;                       % O primeiro valor de y é sempre y0 (condição inicial do pvi)
    
    for i=1:n                        % O número de iterações vai ser igual a n
        k1 = f(t(i),y(i));           % Inclinação no início do intervalo
        k2 = f(t(i+1), y(i) + k1*h); % Inclinação no fim do intervalo
        k = 0.5*(k1+k2);             % Cálculo da média das inclinações
        y(i+1)=y(i)+h*k;             % Próximo valor aproximado da solução do problema original
    end
end