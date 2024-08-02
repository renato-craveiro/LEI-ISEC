function paridade_par()


in = [0 1 0 0 1 0 1 0 1 1;
      0 0 1 0 1 1 1 1 0 1;
      0 0 0 1 0 1 1 1 1 1;
      0 0 0 0 1 0 0 1 1 1]

t =  [1 0 0 0 0 1 0 0 0 1];

net = perceptron;
net.trainParam.epochs = 500;
net = train(net, in,t);

y = sim(net,in);
y= y>=0.5;
fprintf('Outupt:');
disp(y);
erro = perform(net,t,y);
fprintf("Erro %f\n", erro)

c=0;
for i=1:length(t)
   if(t(i) == y(i))
       c=c+1;
   end
end
fprintf("Perceptron acertou %.2f %% dos casos\n",c/length(t)*100)


fprintf("---------------------------------");
n = feedforwardnet([10 10]);
n.divideFcn='';
n = train(n,in,t);
y_feedf=sim(n,in);
y_feedf= y_feedf>=0.5;
disp(y_feedf);
erro = perform(n,t,y_feedf);
fprintf("Erro fforwrd %f\n",erro);
c=0;
for i=1:length(t)
   if(t(i) == y_feedf(i))
       c=c+1;
   end
end
fprintf("FeedForward acertou %.2f %% dos casos\n",c/length(t)*100)