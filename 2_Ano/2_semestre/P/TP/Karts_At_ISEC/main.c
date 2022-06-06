/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
N�mero de aluno: 21280286

No �mbito da Unidade Curricular de Programa��o
Licenciatura em Engenharia Inform�tica - ISEC - 2018/2019

-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include "piloto.h"
#include "carros.h"
#include "corridas.h"

#define FPILOTOS "pilotos.txt"
#define FCARROS "carros.txt"


void limpaecra(void){
    //Fun��o que apaga o que foi escrito no cmd/terminal
    #ifdef _WIN32 //Caso estejamos a trabalhar em ambiente Windows
        system("cls"); //O programa envia o nome do comando "cls" para o Sistema Operativo para o mesmo o executar
    #elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))//Caso estejamos a trabalhar em ambiente MacOS ou UNIX (e vertentes como Linux)
        system("clear");//O programa envia o nome do comando "clear" para o Sistema Operativo para o mesmo o executar
    #else
        #error "\n\nO seu Sistema Operativo nao suporta a limpeza de ecran utilizada neste programa.\n\n" //Caso n�o seja nenhum dos acima referidos a fun��o n�o ir� limpar o ecran, mostrando esta mensagem de erro.
    #endif
}

int menuPrincipal(Piloto *listaPiloto, Carro *listaCarros, parPilCar *listaParesCorrida, int tamPiloto, int tamCarro){
/*Inicia a apresenta��o do menu principal do programa, em que o utilizador escolhe uma das op��es aprsentadas
e essa mesma op��o d� lugar a um conjunto de instru��es, desde chamar a fun��o pretendida at� limpar o ecran*/
    int opc;

    corridas treino;
    parPilCar *auxTreino;

    do{
        fprintf(stdout,"Seja Bem-Vindo/a ao Karts@ISEC\n"
               "______________________________\n\n"
               "1. Mostrar pilotos\n"
               "2. Mostrar carros\n"
               "3. Penalizar piloto por mau comportamento\n"
               "4. Iniciar Corrida/Treino\n"
               "5. Sair\n\n"

               "Selecione a opcao pretendida (escrevendo o numero da opcao e clicando em [ENTER]): ");
        fscanf(stdin, "%d", &opc);
        fflush(stdin);
        switch(opc){
        case 1:
            limpaecra();
            if(listaPiloto!=NULL){
                fprintf(stdout,("Pilotos:\n"));
                mostraPilotos(listaPiloto, tamPiloto);
            }else{
                fprintf(stderr,"ERRO: A lista de pilotos encontra-se vazia.\n");
            }
            fprintf(stdout,"\nPressione [ENTER] para voltar ao menu");
            getchar();
            limpaecra();
            break;

        case 2:
            limpaecra();
            if(listaCarros!=NULL){
                fprintf(stdout,("Carros:\n"));
                mostraCarros(listaCarros, tamCarro);
            }else{
                fprintf(stderr,"ERRO: A lista de carros encontra-se vazia.\n");
            }
            fprintf(stdout,"\nPressione [ENTER] para voltar ao menu");
            getchar();
            limpaecra();
            break;
        case 3:
            limpaecra();
            if(listaPiloto!=NULL){
                penaliza(listaPiloto, tamPiloto);
            }else{
                fprintf(stderr,"ERRO: A lista de pilotos encontra-se vazia.\n");
            }
            fprintf(stdout,"\nPressione [ENTER] para voltar ao menu");
            getchar();
            limpaecra();
            break;
        case 4:
            limpaecra();
            fprintf(stdout, "Nova Corrida/Treino\n");
            treino = corrida(listaPiloto, listaCarros, tamCarro,tamPiloto);
            fprintf(stdout,"\nPressione [ENTER] para voltar ao menu");
            getchar();
            limpaecra();

            while(treino.listaPares!=NULL){
                auxTreino = treino.listaPares;
                free(treino.listaPares->parVoltas);
                treino.listaPares = treino.listaPares->prox;
                free(auxTreino);

            }

            break;
        default:
            limpaecra();
        }
    }while(opc!=5); /*Se o input for diferente de 6 repete o menu, mas caso seja 6 retorna 1, informando a main de que
                    o programa vai encerrar*/
    return 1;
}



int main()
{
    /*Inicia as vari�veis de controlo de tamanho dos vetores e declara o inteiro "terminou" para verificar
    quando a fun��o menuPrincipal termina*/
    int tamPiloto, tamCarro,terminou=0;
    Piloto *listaPiloto=cria_vet_pilotos(FPILOTOS, &tamPiloto); //preenche o vetor listaPiloto com os dados presentes no ficheiro dos mesmos
    Carro *listaCarros=cria_vetor_carros(FCARROS, &tamCarro); //preenche o vetor listaCarros com os dados presentes no ficheiro dos mesmos

    parPilCar *listaParesCorrida =NULL;

    initRandom();

    do{ //MENU PRINCIPAL
        terminou=menuPrincipal(listaPiloto, listaCarros, listaParesCorrida, tamPiloto, tamCarro);
    }while(!terminou);


    limpaecra();
    printf("Aguarde enquanto o programa guarda os ficheiros e limpa a memoria utilizada. Por favor, nao feche esta janela.\n\n");


    guardaPilotos(listaPiloto, tamPiloto, FPILOTOS);
    guardaCarros(listaCarros, tamCarro, FCARROS);
    espera(1);
    free(listaCarros);
    free(listaPiloto);

    printf("Clique [ENTER] para sair do programa");
    getchar();
    return 0;
}
