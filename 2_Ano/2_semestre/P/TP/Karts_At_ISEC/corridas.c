/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
Número de aluno: 21280286

No âmbito da Unidade Curricular de Programação
Licenciatura em Engenharia Informática - ISEC - 2018/2019

-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include "piloto.h"
#include "carros.h"
#include "corridas.h"

#define FPILOTOS "pilotos.txt"
#define FCARROS "carros.txt"

void apNaoSelec(Piloto listaPiloto[], Carro listaCarros[],
                int tamCarro, int tamPiloto, int vetSelec[], int vetCarSelec[], int nPilotos){
//Mostra os carros que não estejam no vetor de carros selecionados e o motivo
    int i,j, encontrou;

    fprintf(stdout, "\n\nCarros/Pilotos que nao estao nesta corrida:\n");

    fprintf(stdout, "\n\nPilotos:\n\n");
    if (tamPiloto ==0){
        fprintf(stdout, "Nao existem pilotos na lista.\n\n");
        return;
    }
    for(i=0; i<tamPiloto; i++){
        encontrou=1;
        for(j=0; j<nPilotos;j++){
            if(vetSelec[j]==listaPiloto[i].id){
                encontrou =0;
            }
        }
        if(encontrou){
            if(listaPiloto[i].impedimento !=0){
                //Caso tenham algum impedimento mostra o número de impedimentos
                fprintf(stdout, "%s\nID: %d -> Razao: Tem %d impedimentos\n\n", listaPiloto[i].nome, listaPiloto[i].id,
                         listaPiloto[i].impedimento);
            }else{
                //Caso contrário a razão foi por não haver espaço para esse mesmo piloto pela escolha aleatória realizada antes
                fprintf(stdout, "%s\nID: %d -> Razao: limite da pista atingido.\n\n", listaPiloto[i].nome, listaPiloto[i].id);
            }
        }
    }

    fprintf(stdout, "---\n\nCarros:\n\n");
    if (tamCarro ==0){
        fprintf(stdout, "Nao existem carros na lista.\n\n");
        return;
    }
    for(i=0; i<tamCarro; i++){
        encontrou=1;
        for(j=0; j<nPilotos;j++){
            if(vetCarSelec[j]==listaCarros[i].id){
                encontrou =0;
            }
        }
        if(encontrou){
            if(listaCarros[i].avariado !=0){
                //Caso tenham alguma avaria mostra o número de voltas que não vao correr
                fprintf(stdout, "\nID: %d -> Razao: Tem %d voltas sem poder correr\n\n", listaCarros[i].id, listaCarros[i].avariado);
            }else{
                //Caso contrário é porque a escolha aleatória não selecionou o carro em questão e não havia espaço para o mesmo
                fprintf(stdout, "\nID: %d -> Razao: limite da pista atingido.\n\n", listaCarros[i].id, listaCarros[i].avariado);
            }
        }
    }


}


void parAleat(parPilCar *parunic, Piloto *listaPiloto, Carro *listaCarros,
               int tamCarro, int tamPiloto, int vetSelec[], int vetCarSelec[], int nPilotos){
//Vai fazer um único par aleatório e acrescentá-lo ao vetor dos pares
    int escolha,avaria,i, encontrado;
    int d,m,a,h,min,s,auxIdade;
    i=0;

    do{
        encontrado=-1;
        escolha=intUniformRnd(0,tamPiloto-1);
        for(i=0; i<nPilotos; i++){
            if (vetSelec[i]==listaPiloto[escolha].id){
                encontrado=1;
                break;
            }
        }
        if(encontrado!=1 && listaPiloto[escolha].impedimento==0 ){
                    encontrado = 0;
        }
    }while(encontrado!=0 );

    i=0;
    while(vetSelec[i]!=0){
        i++;
    };

    vetSelec[i]=listaPiloto[escolha].id;



    (*parunic).parPiloto=listaPiloto[escolha];


    do{
        encontrado=-1;
        escolha=intUniformRnd(0,tamCarro-1);
        for(i=0; i<nPilotos; i++){
            if (vetCarSelec[i]==listaCarros[escolha].id){
                encontrado=1;
                break;
            }
        }
        if(encontrado!=1 && listaCarros[escolha].avariado==0 ){
                    encontrado = 0;
        }
    }while(encontrado!=0 );


    i=0;
    while(vetCarSelec[i]!=0){
        i++;
    };

    vetCarSelec[i]=listaCarros[escolha].id;

    (*parunic).parCarro=listaCarros[escolha];


    //Obtem a data atual e compara-a com a data de nascimanto do piloto, calculando assim a sua idade
    obtemData(&d,&m,&a,&h,&min,&s);
    auxIdade = a - (*parunic).parPiloto.ano;
    if((*parunic).parPiloto.mes>m){
        auxIdade=auxIdade-1;
    }else{
        if((*parunic).parPiloto.mes==m && (*parunic).parPiloto.dia>d){
            auxIdade=auxIdade-1;
        }
    }

    (*parunic).pIdade = auxIdade;
}


 pParPilCar criaListaPC(Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto, parPilCar *listaPC,
                         int *nPilotos, int nvoltas){
//cria a lista dos pares de pilotos/carros
    parPilCar *novoPar, *aux;
    parPilCar parunico;
    int i,cont, contCarros, contPilotos,contPar, auxNPilotosOriginal, opcChar;
    char opc;

     printf("\n\nCapacidade maxima da pista: %d\n", *nPilotos);
    contCarros=contaCarrosDisp(listaCarros, tamCarro);
    printf("\nCarros disponiveis: %d \n", contCarros);
    contPilotos=contaPilotosDisp(listaPiloto, tamPiloto);
    printf("\nPilotos disponiveis: %d \n", contPilotos);

    if(contCarros==0 || contPilotos==0){
        fprintf(stderr, "Nao existem carros/pilotos disponiveis.\n\n");
        return NULL;
    }else{

    if(contCarros>contPilotos){
        contPar=contPilotos;      //Caso existam mais carros que pilotos disponíveis, o número de pares fica igual ao número de pilotos disponiveis
    }else{
        contPar=contCarros;       //Caso contrário o número de pares fica igual ao número de carros disponiveis
    }


    if(*nPilotos>contPar){
        fprintf(stdout,"\nO numero maximo da pista e maior do que os pilotos/carros disponiveis. Serao selecionados %d pares.\n", contPar);

        *nPilotos=contPar;      //Caso o número de pilotos estabelecidos para a pista seja superior ao número de carros/pilotos disponíveis
                                //Apresenta um aviso da situação e informa quantos pares serão gerados
    }



    int jaSelec[*nPilotos], jaCarSelec[*nPilotos]; //Declara dois vetores com o tamanho do número de carros e de pilotos para guardarem os ids
                                                   //dos carros e pilotos já slecionados, iniciando-os a 0
    for(i=0;i<*nPilotos;i++){
        jaSelec[i]=0;
        jaCarSelec[i]=0;
    }


    i=0;
    cont=1;

    printf("\nNumero final de pilotos na corrida: %d\n\n", *nPilotos);

    do{
        novoPar=malloc(sizeof(parPilCar));
        if(novoPar==NULL){
            fprintf(stderr,"ERRO nao foi possivel reservar memoria para o um novo par.\n\n");
            return listaPC;
        }
        parAleat(&parunico, listaPiloto, listaCarros, tamCarro, tamPiloto, jaSelec, jaCarSelec, *nPilotos);

        novoPar->parCarro=parunico.parCarro;
        novoPar->parPiloto=parunico.parPiloto;
        novoPar->pIdade=parunico.pIdade;
        novoPar->parVoltas=malloc(sizeof(parVolt)*nvoltas);
        novoPar->prox=NULL;


        if(listaPC==NULL){
            listaPC=novoPar;
        }else{
            aux=listaPC;
            while(aux->prox!=NULL){
                aux=aux->prox;
            }
            novoPar->prox = aux->prox;
            aux->prox=novoPar;
        }

        i++;
    }while(i<*nPilotos);
    fprintf(stdout,"Deseja ver os carros e pilotos nao selecionados e a razao?[S/N]: ");
    opc = getchar();

    opcChar = 0;
    do{
        switch (opc){
            case 's':
            case'S':
                opcChar =0;
                apNaoSelec(listaPiloto, listaCarros, tamCarro, tamPiloto, jaSelec, jaCarSelec, *nPilotos);
                fflush(stdin);
                fprintf(stdout,"\nPressione [ENTER] para apresentar a corrida.");
                getchar();
                limpaecra();
                break;
            case 'n':
            case 'N':
                opcChar = 0;
                fflush(stdin);
                fprintf(stdout,"\nPressione [ENTER] para apresentar a corrida.");
                getchar();
                limpaecra();
                break;
            default:
                opc = getchar();
                opcChar = 1;
                break;
        }
    }while(opcChar);

    }
    return listaPC;

 }


void mostraPares(parPilCar *listaPC, Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto){
//Apresenta a lista de pares Piloto/Carro
    int cont=1;
    if(!listaPC){
        fprintf(stderr, "A lista de pares da Corrida encontra-se vazia!\n\n");
        return;
    }

    while(listaPC){
        fprintf(stdout,"Par %d:\n", cont);

        mostraPilotoUnico(listaPC->parPiloto.id, listaPiloto, tamPiloto);
        mostraCarroUnico(listaPC->parCarro.id, listaCarros, tamCarro);
        fprintf(stdout, "Idade do Piloto: %d\n\n\n", listaPC->pIdade);

        listaPC=listaPC->prox;

        cont++;
    }
}



corridas corrida(Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto){
//Cria uma corrida
    corridas treino;        //Treino ou corrida são o mesmo, pelo que dei o nome de treino para indicar ser apenas uma corrida a ser criada
    parPilCar *auxTreino;
    treino.listaPares = NULL;
    int *idPosicoes=NULL;
    int voltaAtual, probAcidente, opcCIselec=0, segGanhou=-1, auxganhou=0, i, opcVerVoltas, jaDesistiu, tempTotSeg, auxPos;
    char opcCompInc;

    //Declaração das características da corrida
    fprintf(stdout, "_____________________\n\n"
                    "Insira o numero de voltas da corrida (entre 5 e 10): ");
    do{
        fscanf(stdin, "%d", &treino.nVoltas);
    }while(treino.nVoltas<5 ||treino.nVoltas>10);

    fprintf(stdout,"Insira o comprimento da pista (entre 500 e 1000): ");

    do{
        fscanf(stdin, "%d", &treino.comprimento);
    }while(treino.comprimento<500 ||treino.comprimento>1000);

    fprintf(stdout, "Insira o maximo de concorrentes: ");
    do{
        fscanf(stdin, "%d", &treino.maxConcorrentes);
    }while(treino.maxConcorrentes<1);



    //Criação da lista da corrida para a estrutura treino
    treino.listaPares = criaListaPC(listaPiloto, listaCarros, tamCarro, tamPiloto, treino.listaPares, &treino.maxConcorrentes, treino.nVoltas);

    mostraPares(treino.listaPares, listaPiloto, listaCarros, tamCarro, tamPiloto);
    fflush(stdin);

    //Vai retirar um impedimento/avaria a cada carro/piloto que o/a tenha
    for(i=0;i<tamPiloto;i++){
            if(listaPiloto[i].impedimento>0)
                listaPiloto[i].impedimento--;
        }
    for(i=0;i<tamCarro;i++){
        if(listaCarros[i].avariado>0)
            listaCarros[i].avariado--;
    }

    if(treino.listaPares!=NULL){ //Caso a lista de pares tenha algum par

        //Vai preencher todas as voltas
        for(voltaAtual=0; voltaAtual<treino.nVoltas; voltaAtual++){
            espera(5);
            fprintf(stdout,"---%d.a volta---", voltaAtual+1);

            auxTreino = treino.listaPares;                  //O ponteiro auxiliar aponta para o início da lista de pares
            while(auxTreino){
                auxTreino->parVoltas[voltaAtual].posicao=0; //Coloca todos os pares na volta atual na posição zero
                auxTreino=auxTreino->prox;
            }

            auxTreino = treino.listaPares;
            while(auxTreino){
                if(auxTreino->parPiloto.impedimento==0){    //Se o piloto não tiver nenhum impedimento (o mesmo pode ganhar impedimentos durante a corrida)
                    if(probEvento(0.05)){                   //Se ocorrer um acidente com o par em questão (probabilidade de 5%)
                        auxTreino->parVoltas[voltaAtual].desistiu = 1;  //Indica que foi nesta volta que o par em questão desistiu
                        auxTreino->parVoltas[voltaAtual].exp=-1;        //Guarda a experiência a perder (no caso perde 1 ponto de experiência)
                        auxTreino->parPiloto.impedimento = 2;           //O impedimento do piloto em questão passa a ser 2
                        auxTreino->parCarro.avariado = 1;               //E o carro em questão fica avariado durante 1 corrida
                        fprintf(stdout, "\n\nOcorreu um acidente com o piloto %s (ID do piloto: %d / ID do carro: %d)\n",
                                 auxTreino->parPiloto.nome, auxTreino->parPiloto.id, auxTreino->parCarro.id); //Informa o utilizador do acidente

                    }else{  //Caso não tenha ocorrido um acidente
                        auxTreino->parVoltas[voltaAtual].segundos = calculaSegundos(auxTreino->pIdade,
                                                                   auxTreino->parPiloto.peso, auxTreino->parPiloto.experiencia,
                                                                   auxTreino->parCarro.potencia, treino.comprimento);   //O número de segundos durante esta volta
                                                                                                                        //é calculado e guardado
                        auxTreino->totSeg += auxTreino->parVoltas[voltaAtual].segundos; //O total de segundos é incrementado com o número de segundos na volta atual

                        fprintf(stdout,"\n\nO piloto %s (id: %d) demorou ",
                                 auxTreino->parPiloto.nome, auxTreino->parPiloto.id);
                        for(i=0;i<=voltaAtual;i++){
                            if(i==voltaAtual)
                                fprintf(stdout, "%d =", auxTreino->parVoltas[i].segundos); //Segundos na última volta percorrida
                            else
                                fprintf(stdout,"%d+", auxTreino->parVoltas[i].segundos);   //Segundos das voltas anteriores
                        }
                        fprintf(stdout," %dseg. a percorrer a %d.a volta\n\n",auxTreino->totSeg, voltaAtual+1); //Total de segundos e qual a volta em questão
                    }
                }
                auxTreino=auxTreino->prox;

            }

            for(auxPos=1;auxPos<=treino.maxConcorrentes;auxPos++){     //Vai classificar os Pares de 1 até ao número de pares
                auxTreino = treino.listaPares;
                while(auxTreino){
                    if(!auxganhou && auxTreino->parPiloto.impedimento==0 && auxTreino->parVoltas[voltaAtual].posicao==0){
                        segGanhou=auxTreino->totSeg;    //Arranja o primeiro número de segundos do par que ainda não foi classificado
                        auxganhou=1;                    //Indica que o primeiro numero de segundos encontrado já foi guardado
                    }
                    if(segGanhou>auxTreino->totSeg && auxTreino->parPiloto.impedimento==0 && auxTreino->parVoltas[voltaAtual].posicao==0){
                        segGanhou=auxTreino->totSeg;    //Compara o número de segundos com os restantes e se for menor que ainda não foi classificado
                                                        //esse passa a ser o menor número de segundos
                    }
                    auxTreino= auxTreino->prox;
                }
                auxganhou=0;

                auxTreino = treino.listaPares;
                while(auxTreino){
                    if(segGanhou==auxTreino->totSeg && auxTreino->parVoltas[voltaAtual].posicao==0){
                        auxTreino->parVoltas[voltaAtual].posicao=auxPos;    //Volta a percorrer a lista para encontrar quem teve o menor numero de segundos
                                                                            //Que não tenha sido classificado e dá-lhe a classificação que auxPos tem no momento
                    }
                    auxTreino= auxTreino->prox;
                }

            }


            printf("Posicoes:\n\n");
            //Vai apresentar as posições na volta atual
            for (auxPos=1;auxPos<=treino.maxConcorrentes;auxPos++){
                auxTreino = treino.listaPares;
                while(auxTreino){
                    if(auxPos==auxTreino->parVoltas[voltaAtual].posicao)
                        printf("%s (ID: %d / Carro: %d): %d.o lugar\n", auxTreino->parPiloto.nome, auxTreino->parPiloto.id,auxTreino->parCarro.id,
                               auxTreino->parVoltas[voltaAtual].posicao);

                    auxTreino=auxTreino->prox;
                }
            }

            auxTreino = treino.listaPares;
            while(auxTreino){
            //Vai detetar quem ganhou e apresentar os dados do piloto
                if(auxTreino->parVoltas[voltaAtual].posicao==1){
                    auxTreino->parVoltas[voltaAtual].ganhou=1;
                    auxTreino->parVoltas[voltaAtual].exp=0.5;
                    fprintf(stdout, "\n\n---\n\nO piloto %s (ID: %d) ganhou a %d.a volta!\n\n", auxTreino->parPiloto.nome,
                             auxTreino->parPiloto.id, voltaAtual+1);
                    for(i=0;i<=voltaAtual;i++){
                        if(i==voltaAtual)
                            fprintf(stdout, "%d =", auxTreino->parVoltas[i].segundos);
                        else
                            fprintf(stdout,"%d+ ", auxTreino->parVoltas[i].segundos);
                    }
                    fprintf(stdout," %dseg.\n\n---",auxTreino->totSeg);
                }
                auxTreino= auxTreino->prox;
            }
        }

        auxTreino=treino.listaPares;
        while(auxTreino){
            for(i=0;i<tamPiloto;i++){
                for(voltaAtual=0; voltaAtual<treino.nVoltas; voltaAtual++){
                    if(auxTreino->parPiloto.id==listaPiloto[i].id){
                        if(listaPiloto[i].experiencia<1 && auxTreino->parVoltas[voltaAtual].exp<0){
                            listaPiloto[i].experiencia=0; //Vai atribuir experiência 0 a quem tem menos de 1 de experiência e a sua experiência na corrida tenha sido negativa
                        }else{
                            listaPiloto[i].experiencia+=auxTreino->parVoltas[voltaAtual].exp; //Vai atribuir a experiência obtida na corrida
                        }
                    }
                }
            }
            auxTreino= auxTreino->prox;
        }

        auxTreino=treino.listaPares;
        while(auxTreino){
            for(i=0;i<tamPiloto;i++){
                if(auxTreino->parPiloto.impedimento && auxTreino->parPiloto.id == listaPiloto[i].id){
                    listaPiloto[i].impedimento+=auxTreino->parPiloto.impedimento;  //Vai colocar os impedimentos obtidos na corrida na lista de pilotos
                }
            }
            for(i=0;i<tamCarro;i++){
                if(auxTreino->parCarro.avariado && auxTreino->parCarro.id == listaCarros[i].id){
                    listaCarros[i].avariado+=auxTreino->parCarro.avariado;  //Vai colocar as avarias obtidas na corrida na lista de carros
                }
            }

            auxTreino= auxTreino->prox;
        }


        do{
            fprintf(stdout,"\n\n----Fim da Corrida----\n\n"
                    "1- Rever volta a volta\n"
                    "2- Rever uma Volta especifica\n"
                    "3- Sair da corrida\n\n"
                    "Escolha uma das opcoes acima: ");
            fscanf(stdin, "%d", &opcVerVoltas);
            fflush(stdin);
            //Vai receber a opção a realizar e iniciar a selecionada
            switch(opcVerVoltas){
                case 1:
                    limpaecra();
                    for(voltaAtual=0;voltaAtual<treino.nVoltas;voltaAtual++){
                        fprintf(stdout, "--%d.a volta--\n\n", voltaAtual+1);
                        auxTreino=treino.listaPares;
                        while(auxTreino){
                            jaDesistiu=0;
                            tempTotSeg=0;
                            if(auxTreino->parVoltas[voltaAtual].desistiu==1){
                                fprintf(stdout,"O piloto %s (ID: %d / Carro: %d) desistiu nesta volta.\n\n",
                                        auxTreino->parPiloto.nome, auxTreino->parPiloto.id, auxTreino->parCarro.id);
                            }else{
                                for(i=0;i<=voltaAtual;i++){
                                    if(auxTreino->parVoltas[i].desistiu==1){
                                        jaDesistiu=1;
                                        break;
                                    }
                                }
                                if(!jaDesistiu){
                                    fprintf(stdout,"%s (ID: %d / Carro: %d): ",
                                        auxTreino->parPiloto.nome, auxTreino->parPiloto.id, auxTreino->parCarro.id);
                                    for(i=0;i<=voltaAtual;i++){
                                        tempTotSeg += auxTreino->parVoltas[i].segundos;
                                        if(i==voltaAtual)
                                                fprintf(stdout,"%d = ",auxTreino->parVoltas[i].segundos);
                                        else
                                            fprintf(stdout,"%d+",auxTreino->parVoltas[i].segundos);
                                    }
                                    fprintf(stdout,"%d segundos.\n\n", tempTotSeg);
                                }
                            }
                            auxTreino = auxTreino->prox;
                        }
                        printf("Posicoes:\n\n");
                        for (auxPos=1;auxPos<=treino.maxConcorrentes;auxPos++){
                            auxTreino = treino.listaPares;
                            while(auxTreino){
                                if(auxPos==auxTreino->parVoltas[voltaAtual].posicao)
                                    printf("%s (ID: %d / Carro: %d): %d.o lugar\n", auxTreino->parPiloto.nome, auxTreino->parPiloto.id,
                                           auxTreino->parCarro.id, auxTreino->parVoltas[voltaAtual].posicao);

                                auxTreino=auxTreino->prox;
                            }
                        }

                        fprintf(stdout,"\n\n\nPressione [ENTER] para continuar.");
                        getchar();
                        limpaecra();
                        fflush(stdin);
                        //Apresenta todas as voltas até ao fim, esperando pelo ENTER no final de cada volta
                    }
                    fprintf(stdout,"\n\n\nCorrida terminada!\nPressione [ENTER] para continuar.");
                    getchar();
                    limpaecra();
                    fflush(stdin);

                    break;
                case 2:
                    fprintf(stdout, "Insira a volta que deseja ver: ");
                    fscanf(stdin,"%d",&voltaAtual);
                    fflush(stdin);
                    voltaAtual--; //O vetor começa em zero
                    fprintf(stdout, "--%d.a volta--\n\n", voltaAtual+1);
                    auxTreino=treino.listaPares;
                    while(auxTreino){
                        jaDesistiu=0;
                        tempTotSeg=0;
                        if(auxTreino->parVoltas[voltaAtual].desistiu==1){   //Se for a volta selecionada apresenta os dados dessa volta
                            fprintf(stdout,"O piloto %s (ID: %d / Carro: %d) desistiu nesta volta.\n\n",
                                    auxTreino->parPiloto.nome, auxTreino->parPiloto.id, auxTreino->parCarro.id);
                        }else{
                            for(i=0;i<=voltaAtual;i++){
                                if(auxTreino->parVoltas[i].desistiu==1){
                                    jaDesistiu=1;
                                    break;
                                }
                            }
                            if(!jaDesistiu){
                                fprintf(stdout,"%s (ID: %d / Carro: %d): ",
                                    auxTreino->parPiloto.nome, auxTreino->parPiloto.id, auxTreino->parCarro.id);
                                for(i=0;i<=voltaAtual;i++){
                                    tempTotSeg += auxTreino->parVoltas[i].segundos;
                                    if(i==voltaAtual)
                                            fprintf(stdout,"%d = ",auxTreino->parVoltas[i].segundos);
                                    else
                                        fprintf(stdout,"%d+",auxTreino->parVoltas[i].segundos);
                                }
                                fprintf(stdout,"%d segundos.\n\n", tempTotSeg);

                            }
                        }
                        auxTreino = auxTreino->prox;
                    }

                    printf("Posicoes:\n\n");
                    for (auxPos=1;auxPos<=treino.maxConcorrentes;auxPos++){
                        auxTreino = treino.listaPares;
                        while(auxTreino){
                            if(auxPos==auxTreino->parVoltas[voltaAtual].posicao)
                                printf("%s (ID: %d / Carro: %d): %d.o lugar\n", auxTreino->parPiloto.nome,
                                       auxTreino->parPiloto.id, auxTreino->parCarro.id, auxTreino->parVoltas[voltaAtual].posicao);

                            auxTreino=auxTreino->prox;
                        }
                    }
                    fprintf(stdout,"\n\n\nPressione [ENTER] para continuar.");
                    getchar();
                    limpaecra();
                    fflush(stdin);
                    break;
                case 3:

                    break;
            }
        //Repete o menu de opções até ter  sido selecionada a opção de sair
        }while(opcVerVoltas!=3);


    }
    fflush(stdin);
    return treino;
}

