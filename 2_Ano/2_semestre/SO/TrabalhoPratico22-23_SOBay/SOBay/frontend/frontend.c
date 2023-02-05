/*Desenvolvido por Renato Craveiro (2018011392) - LEI-PL - Ano Letivo 2022/2023 - Sistemas Operativos*/


//Biblioteca onde se encontram os includes, estruturas, definições e funções necessários à execução
#include "../SOBay_common/soBay.h"

//Estruturas de utilizador local e de mensagem para ser enviada ao Backend
struct user current_user; //Estrutura de utilizador local
struct usrMsg msgToSend; //Estrutura de mensagem para ser enviada ao Backend

//Variáveis de controlo
int run=1; //Indica se está em execução (1) ou não (0)
int heartbeat; //Armazena o tempo de heartbeat para enviar ao backend. Recebe esta info do próprio Backend 

//Definições de nomes dos FIFOS (ficheiros) de Backend e deste Frontend
char * befifo = BENDFIFO; //FIFOS (ficheiro) de Backend
char  frontendfifo[BUFFERMAX]; //FIFOS (ficheiro) de Frontend



//Referências de Funções
//Funções
int interpComm(char* comando);//Envia um comando ao Backend para ser interpretado
void getUserData(); //Imprime o username e palavra-passe em *. Apenas para apresentação
void sair(); //envia mensagem de Disconnect ao Backend, elimina o FIFO de Frontend e termina a execução

//Funções Thread
void *readBackend(); //Recebe e Interpreta mensagens recebidas do Backend
void *KB(); //Recebe Input do teclado e sai se input="exit". Também indica se foi um comando sucedido ou não
void *sendHeartBeat(); //Envia um Heartbeat ao Backend um segundo antes de terminar o tempo de HEARTBEAT




int main(int argc, char *argv[]){
    struct beMsg respBE;

    int fd;
    int i;
    char olaf[BUFFERMAX];

    fd = open(befifo, O_WRONLY);
    if(fd==-1){
        printf("BACKEND NÃO SE ENCONTRA EM EXECUÇÃO!!\n\n");
        exit(-1);
    }
    close(fd);

    setbuf(stdout, NULL);
    
    int aux=0;
    
    if(argc<3){
        printf("--ERRO!--\n\nUse frontend [username] [password] (sem pareênteses retos)\n\n");
        return -1;
    }
    printIntro();
    printf("_Frontend:\n\n");
    
    current_user.pid=getpid();

    sprintf(frontendfifo, FENDFIFO, current_user.pid);
    mkfifo(frontendfifo,0666);

    strcpy(current_user.username, argv[1]);
    strcpy(current_user.pw, argv[2]);
    
    msgToSend.who=current_user;

    fd = open(befifo, O_WRONLY);
    msgToSend.type=LOGIN;
    strcpy(msgToSend.msg,"");
    write(fd, &msgToSend, sizeof(msgToSend));
    close(fd);
    
    int bytes;

    fd = open(frontendfifo, O_RDONLY);
    while(read(fd, &respBE, sizeof(respBE))<0);
    if(respBE.type==GOODLOGIN){
        printf("Login efetuado [BACKEND INDICA HEARTBEAT: %s]\n\n", respBE.msg);
        heartbeat=atoi(respBE.msg);
    }if(respBE.type==BADLOGIN){
        printf("Login falhado [BACKEND: %s]\n\n", respBE.msg);    
        close(fd);
        sair();
    }
    close (fd);

    printf("Bem-vindo(a) ao SOBay %s\n-------------\n\n", current_user.username);
    getUserData();
    signal(SIGINT,sair);
    
    pthread_t thrBE, thrKB, thrHB; 

	int iretBE, iretKB, iretHB;
    	
    iretBE = pthread_create( &thrBE, NULL, readBackend, NULL);
    iretKB = pthread_create( &thrKB, NULL, KB, NULL);
    iretHB = pthread_create(&thrHB,NULL,sendHeartBeat,NULL);

    pthread_join( thrKB, NULL);
    pthread_join( thrBE, NULL);
    pthread_join(thrHB,NULL);

    sair();

    return 0;
}




int interpComm(char* comando){
        if(strcmp(comando,"")){
            int fdr;
            printf("a enviar %s\n\n",comando);
            strcpy(msgToSend.msg,comando);
            msgToSend.type=COMMAND;
            msgToSend.who=current_user;
            fdr=open(befifo, O_WRONLY);
            write(fdr, &msgToSend, sizeof(msgToSend));
            close(fdr);
        }
        
        return 0;
}

void getUserData(){
    int aux;
    printf("Login:\n\nUsername: %s\nPassword:", current_user.username);
    for(aux=0;aux<strlen(current_user.pw);aux++){
        printf("*");
    }
}

void sair(){
    int fd;
    fd = open(befifo, O_WRONLY);
    msgToSend.type=DISCONNECT;
    strcpy(msgToSend.msg,"");
    write(fd, &msgToSend, sizeof(msgToSend));
    close(fd);
    unlink(frontendfifo);
    exit(0);
}

void *readBackend(){
    int fd;
    struct beMsg respBE;
    while(run==1){
        fd = open(frontendfifo, O_RDONLY);
        if(fd>0){
        while(read(fd, &respBE, sizeof(respBE))<0);
            if(respBE.type==BE_SHUTDOWN){
                printf("\n[BACKEND FOI ENCERRADO!]\n\n");
                sair();
            }
            if(respBE.type==BE_KICK){
                printf("\n[BACKEND MANDOU KICK!]\n\n");
                sair();
            }
            printf("\n[BACKEND] %s\n", respBE.msg);
        }
        
        close(fd);
    }
}

void *KB(){
    char buffer[125];
    int errorType;
     while(run==1){
        printf("\n> ");
        scanf("%[^\n]", buffer);
        while (getchar() != '\n')
            continue;   

        if(!strcmp(buffer,"exit")){
            run =0;
            sair(); 
            break;
        }

        if((errorType= interpComm(buffer))<0){
            printf("ERRO: ");
            if(errorType==-1){
                printf("Comando não encontrado!\n\n");
            }
            if(errorType==-2){
                printf("Parametros incorretos!\n\n");
            }
        }
        strcpy(buffer,"");      
    }

}

void *sendHeartBeat(){
    int fdr;
    while(run==1){
        sleep(heartbeat-1);
        printf("\nA enviar HEARTBEAT\n\n");
        msgToSend.type=HBEAT;
        msgToSend.who=current_user;
        fdr=open(befifo, O_WRONLY);
        write(fdr, &msgToSend, sizeof(msgToSend));
        close(fdr);
    }
}