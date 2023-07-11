%NRK4  Método de Runge-Kutte de Ordem 4.
%   y = N_RK2(f,a,b,n,y0) Método numérico para a resolução de um PVI
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


function y = NRK4(f,a,b,n,y0)
    h = (b-a)/n;                                        % h = tamanho de cada passo (subintervalo)
    
    t = a:h:b;                                          % Alocação de memória e preenchimento de vetor t (abcissas) (de a a b com intervalos de tamanho h)
    y = zeros(1, n+1);                                  %Alocação de memória a zero criando um vetor limpo com tamanho n+1
    
    y(1) = y0;                                          % Insersão da condição inicial no primeiro elemento do vetor y

    for i=1:n                                           % n iterações
        k1 = h*f(t(i), y(i));                           % Inclinação inicial do intervalo
        k2 = h*f(t(i) + h/2, y(i) + 0.5*k1);            % 1ª Inclinação média (ponto médio) do intervalo
        k3 = h*f(t(i) + h/2, y(i) + 0.5*k2);            % 2ª Inclinação média (ponto médio) do intervalo
        k4 = h*f(t(i+1), y(i) + k3);                    % Inclinação final do intervalo
        
        y(i + 1) = y(i) + (k1 + 2*k2 + 2*k3 + k4)/6;    % Aproximação do próximo valor da solução do problema original
    end
end

