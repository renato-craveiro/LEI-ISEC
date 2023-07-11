%NEULER  M�todo de Euler para ED/PVI.
%   y = NEuler(f,a,b,n,y0) M�todo num�rico para a resolu��o de um PVI
%   y'= f(t,y) com t=[a, b] e y(a)=y0 condi��o inicial  
%
%INPUT:
%   f - fun��o do 2.� membro da Equa��o Diferencial
%   [a, b] - extremos do intervalo da vari�vel independente t
%   n - n�mero de subintervalos ou itera��es do m�todo
%   y0 - condi��o inicial t=a -> y=y0
%OUTPUT: 
%   y - vector das solu��es aproxima��es
%   y(i+1) = y(i)+h*f(t(i),y(i)) , i =0,1,...,n-1
%
%   03/03/2020 - Arm�nioCorreia .: armenioc@isec.pt 

function y = NEuler(f,a,b,n,y0)
    h    = (b-a)/n;
    
    t = a:h:b;   %Modificado a 16/04/2023 por Renato Craveiro para aloca��o de mem�ria 
                 % e preenchimento de vetor t (abcissas) (de a a b com intervalos de tamanho h)
    y = zeros(1, n+1);  % Modificado a 16/04/2023 por Renato Craveiro para aloca��o de mem�ria a zero
                        %criando um vetor limpo com tamanho n+1
    
    t(1) = a;
    y(1) = y0;
    for i=1:n
        y(i+1)=y(i)+h*f(t(i),y(i));
        t(i+1)=t(i)+h;
    end
end
