%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.2 - Funcs

%c)

function maior=maior_num()
    %nMat=1;
    maior=0;
    prompt="Insira um num pf.: ";
    while true
        x=input(prompt);
        if maior<x
            maior=x;
        end
        %matrix
        % mat(nMat)=x;
        %nMat=nMat+1;
        if(x==0)
            break
        end
    end
    %mat;
    %maior=max(mat);
end