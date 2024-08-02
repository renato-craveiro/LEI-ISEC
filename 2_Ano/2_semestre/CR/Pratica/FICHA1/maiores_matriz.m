%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.2 - Funcs

% f)


function [resMaior,resPos] = maiores_matriz(mat)
    % resMaior=0;
    % mat
    % size(mat)
    % for n=1:length(mat)
    %     if resMaior<mat(n)
    %         resMaior=mat(n);
    %         resPos=n;
    %     end
    % end
    resMaior=max(mat)
    resPos=find(mat==resMaior)

end