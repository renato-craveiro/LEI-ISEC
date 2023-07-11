/*Desenvolvido por Renato Craveiro (2018011392) - LEI-PL - Ano Letivo 2022/2023 - Sistemas Operativos*/

//Includes necessários à execução/compilação do programa
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include "users_lib.h"


//DEFAULTS
//Tamanho máximo de buffers, por defeito
#define BUFFERMAX 1024

//Máximo de utilizadores, promoções e itens no sistema
#define MAXUSERS 20
#define MAXPROMO 10
#define MAXITEMS 30

//Localizações dos ficheiros necessários ao sistema: utilizadores, promotores, tempo, itens e local dos promotores
#define FUSERS "../files/users"
#define FPROMOTERS "../files/promo"
#define FTEMPO "../files/tempo"
#define FITEMS "../files/item"
#define PPROMO "../promotor/"

//Definições dos FIFOS (por defeito) do Backend e dos Frontends
#define BENDFIFO "/tmp/befifo"
#define FENDFIFO "/tmp/fe%d"

//Valor por defeito do HEARTBEAT dos Frontend quando não existe variável de ambiente HEARTBEAT
#define DEF_HEARTBEAT 60

//Indicação de user conectado ou não
#define CNTD 5 //connected
#define DCNTD 8 //disconnected

//Tipos de mensagem enciados do Frontend->Backend
#define LOGIN 9 //A tentar efetuar login
#define HBEAT 1 //A enviar um HEARTBEAT
#define COMMAND 2 //A enviar um comando
#define DISCONNECT 3 //A indicar que vai encerrar

//Tipos de mensagem enciados do Backend->Frontend
#define BE_SHUTDOWN 99 //Backend a encerrar
#define BE_RESPONSE 77 //Resposta do Backend a um comando/operação
#define BE_KICK 22 //A kickar um Frontend
#define BADLOGIN 44 //Indicação de login não sucedido
#define GOODLOGIN 33 //Indicação de login sucedido
#define WARNING 54 //A enviar uma mensagem de aviso / informação


//ESTRUTURAS
/*Estrutura de utilizador com indicação do pid (se estiver em execução)
username, password, saldo que tem, se está CNTD ou DCNTD e tempo restante de HEARTBEAT*/
struct  user{   
    int pid;          
    char username[50];      
    char pw[50];
    int saldo;
    int online;
    int hbeat;
};

/*Estrutura de item com indicação de id, nome, categoria, valor base
valor "compre já", um backup dos dois anteriores caso exista uma promoção,
a sua duração restante em sistema, quem o está a vender e o último licitador mais alto*/
struct item{
    int id;
    char nome[50];
    char categoria[50];
    int vBase;
    int compreJa;
    int bckvBase;
    int bckcompreJa;
    int duracao;
    char vendedor[50];
    char licitador[50];
};

/*Estrutura de mensagem de Frontend->Backend que indica o tipo de mensagem
o utilizador que a enviou e a mensagem a enviar*/
struct usrMsg{
    int type; 
    struct user who;
    char msg[BUFFERMAX*2];
};

/*Estrutura de mensagem de Backend->Frontend que indica o tipo de mensagem e a mensagem a enviar*/
struct beMsg{
    int type;
    char msg[BUFFERMAX*2];
};

/*Estrutura de promoção com indicação do id, o endereço do programa que a lançou
o pid do programa que a lançou, a categoria, a percentagem de desconto e o tempo 
restante da promoção, se se encontra ativa (1) ou não (0), se se encontra em execução (1) ou não (0)
e o tid da thread que a lançou (!!não utilizado no sistema!!) */
struct promo{
    int id;
    char progPath[BUFFERMAX];
    pid_t pid;
    char cat[BUFFERMAX];
    int perc;
    int tempoSeg;
    int ativa;
    int execucao;
    pthread_t tid;
};

/*Estrutura de comando com uma matriz de strings que armazenam cada uma uma palavra (comando) 
e o número de palavras que existem nessa matriz*/
typedef struct command{
    char *comando[125];
    int param;
}comando;


//FUNÇÕES
struct command splitString();//Função que divide uma string em comandos
void printIntro(); //Imprime a introdução do Frontend e Backend
int isNumber(char s[]); //Indica se um caracter é número ou não (NÃO FUNCIONA A 100%)
