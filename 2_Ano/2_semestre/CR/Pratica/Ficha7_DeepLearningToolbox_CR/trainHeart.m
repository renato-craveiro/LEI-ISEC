
function trainHeart()
    S = readmatrix('heart_train.csv', 'Delimiter', ',', 'DecimalSeparator', '.');
    in = S(:,1:end-1)';
    t = S(:,end)';


    net = feedforwardnet;
    net.trainFcn = 'trainlm'; %backpropagation

    [net,tr] = train(net,in,t);

    y = net(in);
    y=(y>=0.5);

    erro = perform(net,t,y)
    fprintf("Erro: %f\n",erro);
    c=0;
    for i=1:length(t)
       if(t(i) == y(i))
           c=c+1;
       end
    end
    fprintf("FeedForward acertou %.2f %% dos casos\n",c/length(t)*100)
    
