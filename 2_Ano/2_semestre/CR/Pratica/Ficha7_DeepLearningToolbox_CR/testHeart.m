function testHeart()
    S = readmatrix('heart_test.csv', 'Delimiter', ',', 'DecimalSeparator', '.');
    in =S'
    t = [1 1 1 0 0 0]
    y = net(in);
    y = (y>=0.5);
    erro = perform(net,t,y);
    