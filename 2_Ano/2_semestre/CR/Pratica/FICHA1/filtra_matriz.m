%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.2 - Funcs

%b)


function res = filtra_matriz(mat)
    %iRes=1;
    %for n=1:length(mat)
    %    if mod(mat(n),2)==0
    %        res(iRes)=mat(n)
    %        iRes=iRes+1;
    %    end
    %end
    res=mat(find(mod(mat,2)==0)');

end

