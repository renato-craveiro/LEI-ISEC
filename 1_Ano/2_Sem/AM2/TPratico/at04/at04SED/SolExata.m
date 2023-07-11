%   30/04/2023 - Renato Craveiro | 2018011392 | Lic. Eng. Informática ISEC
%   Ano Letivo 2022/23


function [t, exata] = SolExata(ODE, a, b, n, u0, v0)
    syms y(t);
    tempExata = dsolve(ODE, ['y(0)=',num2str(u0)], ...
        ['Dy(0)=',num2str(v0)], 't');
    if(~isempty(tempExata))
        ext=@(t) eval(vectorize(char(tempExata)));
        h=(b-a)/n;
        t=a:h:b;
        exata=ext(t);
    else
        exata=[];
end