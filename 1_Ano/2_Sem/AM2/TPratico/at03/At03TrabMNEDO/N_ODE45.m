%N_ODE45  Método ODE45 (Método disponibilizado pelo MATLAB).
%   y = N_ODE45(f,a,b,n,y0) Método numérico para a resolução de um PVI
%   y'= f(t,y) com t=[a, b] e y(a)=y0 condição inicial  
%
%INPUT:
%   f - função do 2.º membro da Equação Diferencial
%   [a, b] - extremos do intervalo da variável independente t
%   n - número de subintervalos ou iterações do método
%   y0 - condição inicial t=a -> y=y0
%OUTPUT: 
%   y - vector das soluções aproximações
%   y(i+1) = y(i)+h*f(t(i),y(i)) , i =0,1,...,n-1
%
%   16/04/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23

function y = N_ODE45(f,a,b,n,y0)
    h = (b-a)/n;                        % h = tamanho de cada passo (subintervalo)
    
    t = a:h:b;                          % Alocação de memória e preenchimento de vetor t (abcissas) (de a a b com intervalos de tamanho h)
    
    [~,y] = ode45(f, t, y0);            % Preenchimento do vetor y com aproximações utilizando a função ODE45
    y = y';                             % Mudança da orientação do vetor
end


