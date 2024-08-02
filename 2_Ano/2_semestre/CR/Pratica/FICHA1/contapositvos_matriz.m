%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.2 - Funcs

%e)



function res=contapositvos_matriz(mat)
    % nRes=0;
    % for n=1:length(mat)
    %     if mat(n)>0
    %         %tmp(nRes)=mat(n);
    %         nRes=nRes+1;
    %     end
    % end
    % res=nRes
    %res=numel( ...
       res= numel(find(mat>0))
end
