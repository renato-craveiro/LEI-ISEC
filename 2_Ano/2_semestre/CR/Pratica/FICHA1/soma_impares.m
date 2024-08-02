%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.2 - Funcs

%d)



function res = soma_impares(mat)
    res=0;
    % for n=1:length(mat)
    %     if(mat(n)~=0)
    %         res=res+mat(n)
    %     end
    % end
    tmp=mat(find(mod(mat,2)~=0)');
    res = sum(tmp);

end
