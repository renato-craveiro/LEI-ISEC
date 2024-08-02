%
% Worksheet 1 - Revision MatLab
%

% Ex. 3.1

%a)
max = 100;
mat=[2:2:100];

%b)
matTransp=mat';

%c)
matC=[1 2 3;4 5 6;7 8 9];

%d)
matC(1,3) = 33;

%e)
matE=ones(3,3);








%f)
matF=randn(8,8) %neg+pos
matF2=rand(8,8) %pos



%g)
matF2(:,2)=[]


%h)
x=0:pi/100:2*pi
y=sin(x)
plot(x,y,'DisplayName','Sin')

%i)
hold on
xCos=0:pi/100:2*pi
yCos=cos(xCos)
plot(xCos,yCos,Color='g',DisplayName='Cos')

legend
title("Sin vs. Cos")


hold off




%3.2 a)

m1=gera_matriz(10,4,10)






