function [fis] = gorjeta()

%PASSO 1 criar sistema fis
fis = mamfis;

%PASSO 2 VARIÁVEIS
fis = addInput(fis,[0,10],'Name',"servico");
       %COMPLETAR
fis = addInput(fis,[0,10],'Name',"comida");
fis = addOutput(fis,[0,30],'Name',"gorjeta");

%PASSO 3 FUNÇÕES DE PERTENÇA
fis = addMF(fis,"servico", "gaussmf",[1.5 0],'Name', "fraco");
	%COMPLETAR para as três variáveis
fis = addMF(fis,"servico", "gaussmf",[1.5 5],'Name', "bom");
fis = addMF(fis,"servico", "gaussmf",[1.5 10],'Name', "excelente");

fis = addMF(fis,"comida", "trapmf",[0 0 1 3],'Name', "ma");
fis = addMF(fis,"comida", "trapmf",[7 9 11 19],'Name', "deliciosa");

fis = addMF(fis,"gorjeta", "trimf",[0 5 10],'Name', "fraca");
fis = addMF(fis,"gorjeta", "trimf",[10 15 20],'Name', "media");
fis = addMF(fis,"gorjeta", "trimf",[20 25 30],'Name', "generosa");
	%PASSO 4 REGRAS
regra1 = "servico==fraco | comida==ma => gorjeta=fraca";
regra2 = "servico==bom => gorjeta=media";
regra3 = "servico==excelente | comida==deliciosa => gorjeta=generosa";
regras=[regra1 regra2 regra3];
fis = addRule(fis,regras);

%PASSO 5: avaliar para vários valores de servico e comida com evalfis
for servico=0:10
  	  for comida=0:10
           entrada=[servico comida];
           out = evalfis(fis, entrada);
            fprintf('serviço = %d\nComida = %d\nGorjeta = %f\n\n',servico, comida, out);
      end
 end
end

