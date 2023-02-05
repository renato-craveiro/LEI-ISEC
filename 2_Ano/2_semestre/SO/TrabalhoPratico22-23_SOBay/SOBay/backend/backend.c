/*Desenvolvido por Renato Craveiro (2018011392) - LEI-PL - Ano Letivo 2022/2023 - Sistemas Operativos*/


//Biblioteca onde se encontram os includes, estruturas, definições e funções necessários à execução
#include "../SOBay_common/soBay.h"

/*VARIÁVEIS E ESTRUTURAS GLOBAIS-*/

//Matrizes de utilizadores, itens e promoções
struct user users[MAXUSERS];
struct item items[MAXITEMS];
char promo[MAXPROMO][125];
struct promo promoInUse[MAXPROMO];

//Variáveis de controlo
int running=1; //Flag que indica se backend está em execução (1) ou não (0)
int nusers=0; //número de users do sistema
int nitems=0; //número de itens no sistema
int npromo=0; //número de promotores no sistema
int npromoIn=0; //número de promotores em execução
int lastItemId; //último ID de item encontrado no sistema
int heartbeat; //guarda o valor na Vvar. ambient HEARTBEAT ou, se não existir, aplica o DEF_HEARTBEAT que se encontra em soBay.h
int tempo=0; //tempo do sistema. se não existir aplica 0

//Definição de pipe de receção do Backend
char * befifo = BENDFIFO; 


/*FUNÇÕES*/
//Funções de carregamento de ficheiros e defaults do Backend
void loadPromotores(); //Carrega os promotores presentes no sistema
void loadTempo(); //Carrega o tempo presente no ficheiro de tempo
void loadUtilizadores(); //Carrega os utilizadores presentes no ficheiro de utilizadores
void loadItems(); //Carrega os itens presentes no ficheiro de itens

//Funções de leitura e interpretação de input por parte do Admin. no Backend
void *KB(); //Thread de leitura de input do teclado (Admin.)
void comand(); //lê o input do teclado e envia ao interpretador. Apresenta se o interpretador retornou comando sucedido (ou não)
int interpComm(comando comandorec); //Interpreta os comandos recebidos pelo Admin. e executa em conformidade
void oth_comm(); //Interpreta outros tipos de comandos (utilizadores, promotores e itens)
void userOperations(); //Operações do tipo utilizador
void retiraUmSaldoTodos(); //retira um saldo a todos os users (para consulta de dados dos mesmos pelo Admin.)
void promoOperations(); //Operações do tipo Promotor
int execPromo(char *prog); //Executa um promotor
void cancelPromos(); //Cancela os promotores

//Funções de apresentação de dados no Backend (visualização do Admin.)
void printUtil(); //Imprime os utilizadores do sistema no ecran
void printOnUsers(); //Imprime os utilizadores online do sistema no ecra
void printPromo(); //Imprime os promotores do sistema no ecra
void printItems(); //Imprime os itens do sistema no ecra

//Funções de atualização periódica/automática no tempo
void *ftempo(); //thread que incrementa a variável tempo, chama as restantes operações automáticas no tempo e "adormece" durante 1 seg.
void updateItems(); //atualiza os tempos dos itens e retira-os se o tempo estiver a 0
void checkHbAndKick(); //Verifica o tempo de Heartbeat dos utilizadores conectados e kicka-os se estiver a 0
void reduceHBUsers(); //retira um de heartbeat aos users conectados
void checkPrInUse(); //Verifica se o tempo de promoções ativa já acabou e atualiza os dados para default caso acabem

//Receção de dados oriundos dos Frontend
void *readpipe(); //Thread que recebe as mensagens dos utilizadores e interpreta o tipo das mesmas, agindo em conformidade
void interpUserComm(struct usrMsg recUser, char* fiforesp); //interpreta comandos solicitados pelos Frontend

//Manipulação de dados de utilizador
void loginUser(struct user usr); //Modifica o user (se existir em sistema) para estar conectado ao sistema
void logoffUser(struct user usr);//Modifica o user (se existir em sistema) para estar desconectado do sistema
int userPIDByName (char*un); //Retorna o PID de um user (se existir) através de um nome indicado
int idUserByName(char*un); //Retorna o ID de um user (se existir) através de um nome indicado
struct user getUserByID(int id); //Retorna a estrutura de um user (se existir) através de um id indicado
int isonline(char *un); //Indica se um user (se o nome for válido) está online (ou não)
void banirUser(struct user usr); //Kicka um user
void resetHBeatUser(struct user usr); //Retorna o Heartbeat de um utilizador para o valor default


//Avisos de Itens a todos os utilizadores Online
void warnNewItem(struct item itm); //Avisa todos os utilizadores de um novo item em sistema
void warnSoldItem(struct item itm); //Avisa todos os utilizadores que o item foi vendido (ou não)

//Funções de encerramento do Backend
void trata_terminar(); /*Guarda todos os itens no ficheiro de itens, envia mensagem de Disconnect
                         a todos os Frontend, elimina o FIFO de Backend, guarda o ficheiro de utilizadores,
                         termina a execução de possíveis promotores em execução, elimina o ficheiro de preseça,
                         guarda o tempo e termina a execução */
void saveTempo(); //Guarda o tempo no ficheiro de tempo
void saveItems(); //Guarda os itens no ficheiro de itens
void sairTodosUsr(); //Envia uma mensagem a todos os users a indicar o encerramento do Backend


//Funções criadas mas não utilizadas no sistema por falhas na execução
void *promoLauncher(void * pPath); //possíveis threads de interpretação de promoções
void *interpPromo(); //possível thread que receberia informação de inicio de promoções e lançava threads de interpretação de promoções
void printPromosInUse(); //Imprimiria promoções/promotores ativas/os




int main(int argc, char *argv[]){
    char charHeartBeat[BUFFERMAX];
    if (access("be.init", F_OK) == 0) {
        printf("\nJA ABERTO!\n");
        exit(-1); 
    }
    printIntro();
    printf("_Backend:\n\n");

    if(getenv("HEARTBEAT")==NULL){
        printf("[SEM VAR AMBIENTE HEARTBEAT! APLICAR DEFAULT %d]\n\n", DEF_HEARTBEAT);
        heartbeat=DEF_HEARTBEAT;
    }else{
        if(atoi(getenv("HEARTBEAT"))>0){
            strcpy(charHeartBeat, getenv("HEARTBEAT"));

            heartbeat=atoi(charHeartBeat);
            printf("[VAR AMBIENTE HEARTBEAT É O NÚMERO: %d]\n\n", heartbeat);
        }else{
            printf("[VAR HEARTBEAT NÃO É NÚMERO POSITIVO! A APLICAR DEFAULT %d]\n\n", DEF_HEARTBEAT);
            heartbeat=DEF_HEARTBEAT;
        }  
    }

    printf("[HEARTBEAT de cada utilizador: %d]\n\n", heartbeat);

    printf("[%d utilizadores em sistema]\n\n",loadUsersFile(FUSERS));   
    loadTempo();
    printf("\n[Tempo em execução: %d seg.]\n\n", tempo);

    loadUtilizadores();
    loadItems();
    loadPromotores();
    

    FILE *fp;

    fp  = fopen ("be.init", "w");
    fclose(fp);
    signal(SIGINT,trata_terminar);
    printf("\n");


    pthread_t thrTempo, thrKB, thrPipe, thrPromo; 
	int iretTempo, iretKB, iretPipe, iretPromo; 
    	
    iretTempo = pthread_create( &thrTempo, NULL, ftempo, NULL);
    iretKB = pthread_create( &thrKB, NULL, KB, NULL);
    iretPipe = pthread_create( &thrPipe, NULL, readpipe, NULL);

    pthread_join( thrKB, NULL);
    pthread_join( thrTempo, NULL);
    pthread_join( thrPipe, NULL);
	
	printf("ThrTempo retorna: %d\n\n",iretTempo);
    printf("ThrKB retorna: %d\n\n",iretKB);
    printf("ThrPipe retorna: %d\n\n",iretPipe);

    trata_terminar();
}




void reduceHBUsers(){
    int i;
    for(i=0; i<nusers;i++){
        if(users[i].online==CNTD && users[i].hbeat>0){
            users[i].hbeat--;
        }
    }
}

void checkHbAndKick(){
    int i;
    for(i=0; i<nusers;i++){
        if(users[i].online==CNTD && users[i].hbeat<=0){
            banirUser(users[i]);
        }
    }
}

void saveTempo(){
    FILE *fptr;

    fptr = fopen(FTEMPO, "w"); 

    fprintf(fptr, "%d", tempo);
    
    fclose(fptr);
}

void sairTodosUsr(){
    struct beMsg rspBE;
    char fiforesp[BUFFERMAX];
    int fdr;
    int i;
    for(i=0; i<nusers;i++){
        if(users[i].online==CNTD){
            sprintf(fiforesp, FENDFIFO, users[i].pid);
            logoffUser(users[i]);
            rspBE.type=BE_SHUTDOWN;

            strcpy(rspBE.msg,"");
            fdr=open(fiforesp, O_WRONLY);
            write(fdr, &rspBE , sizeof(rspBE));
            close(fdr);
        }
    }
}

void banirUser(struct user usr){
    struct beMsg rspBE;
    char fiforesp[BUFFERMAX];
    int i,fdr;
    
    for(i=0;i<=nusers;i++){
        if(!strcmp(usr.username,users[i].username)){
            printf("A BANIR %s\n", users[i].username);
            sprintf(fiforesp, FENDFIFO, users[i].pid);
            logoffUser(usr);
            rspBE.type=BE_KICK;
            strcpy(rspBE.msg,"");

            if((fdr=open(fiforesp, O_WRONLY))>0){;
                write(fdr, &rspBE , sizeof(rspBE));
                close(fdr);
            }
            return;
        }
    }
}

void resetHBeatUser(struct user usr){
    int i;
    for(i=0;i<=nusers;i++){
        if(!strcmp(usr.username,users[i].username)){
            users[i].hbeat=heartbeat;
            return;
        }
    }

}

void trata_terminar(){
    int fdr;
    saveItems();
    sairTodosUsr();

        
    unlink( befifo);

    int i;
    if(saveUsersFile(FUSERS)!=0){
        printf("ERRO: %s\n\n",getLastErrorText());
    }
    printf("\nBackend a encerrar...\n");

    for(i=0;i<npromoIn;i++){
        kill(promoInUse[i].pid, SIGINT);
    }

    if (remove("be.init") == 0) {
        printf("[O ficheiro de presença foi eliminado com sucesso.]\n\n");
    } else {
        printf("[ERRO: O ficheiro de presença não foi eliminado.]\n\n");
    }
    saveTempo();
    
    exit(0);
}

void retiraUmSaldoTodos(){
    int i=0;
    for(i=0; i<nusers; i++){
        users[i].saldo--;
        updateUserBalance(users[i].username,getUserBalance(users[i].username)-1);
    }
    saveUsersFile(FUSERS);
}

void applyPromo(struct promo p){
    int i,descTeste,aux, fdr;
    char fiforesp[BUFFERMAX];
    struct beMsg rspBE;
    printf("A aplicar promoção!\n");
    for(aux=0;aux<nusers;aux++){
        if(users[aux].online==CNTD){
            sprintf(rspBE.msg,"COMEÇOU A PROMOÇÃO DA CATEGORIA %s (%d por cento durante %d seg.)!\n", p.cat,p.perc,p.tempoSeg);
            sprintf(fiforesp,FENDFIFO,users[aux].pid);
            fdr = open(fiforesp, O_WRONLY);
            write(fdr, &rspBE, sizeof(rspBE));
            close(fdr);
        }
    }

    for(i=0;i<nitems;i++){
        if(!strcmp(items[i].categoria,p.cat)){
            items[i].bckcompreJa=items[i].compreJa;
            items[i].bckvBase=items[i].vBase;
            items[i].compreJa=((double)items[i].compreJa-((double)items[i].compreJa*((double)p.perc/100)));
            items[i].vBase=((double)items[i].vBase-((double)items[i].vBase*((double)p.perc/100)));
        }
    }
}

void checkPrInUse(){
    int i,j, aux,fdr;
    struct beMsg rspBE;
    char fiforesp[BUFFERMAX];
    for(i=0;i<npromoIn;i++){
        if(promoInUse[i].tempoSeg==0 || promoInUse[i].ativa==0){
            printf("Acabou o tempo de %s\n", promoInUse[i].progPath);
            for(aux=0;aux<nusers;aux++){
            if(users[aux].online==CNTD){
                sprintf(rspBE.msg,"TERMINOU A PROMOÇÃO DA CATEGORIA %s (%d por cento)!\n", promoInUse[i].cat,promoInUse[i].perc);
                sprintf(fiforesp,FENDFIFO,users[aux].pid);
                fdr = open(fiforesp, O_WRONLY);
                write(fdr, &rspBE, sizeof(rspBE));
                close(fdr);
            }
    }


            promoInUse[i].ativa=-1;
            promoInUse[i].tempoSeg=-1;
            for(j=0; j<nitems;j++){
                if(!strcmp(items[j].categoria, promoInUse[i].cat)){
                    items[j].compreJa=items[j].bckcompreJa;
                    items[j].vBase=items[j].bckvBase;
                    
                }
            }
        }
        promoInUse[i].tempoSeg--;
    }
}

void printPromosInUse(){
    int i;
    for(i=0;i<npromoIn;i++){
        printf("Promo: ID %d | Cat: %s | Desc.: %d | Tempo: %d |", promoInUse[i].id,promoInUse[i].cat, promoInUse[i].perc, promoInUse[i].tempoSeg);
        if(promoInUse[i].ativa==1){
            printf("Ativo\n");
        }
    }
}

int execPromo(char *prog){
    comando recPromo;

    int pf[2];
    pid_t p, f;

    char buff[BUFFERMAX];
    char progPath[125];
    strcpy(progPath, "../promotor/");
    strcat(progPath, prog);

    printf("PATH: %s\n\n", progPath);

    if (pipe(pf) != 0)
    {
        fprintf(stderr, "Pipe Failed");
        return -1;
    }

    f = fork();
    if (f < 0)
    {
        printf("SON FORK ERROR \n");
        return -1;
    }
    if (f > 0)
    {

        close(pf[1]);
        read(pf[0], buff, BUFFERMAX);
        printf("PROMOTOR ENVIOU %s\n", buff);
        recPromo = splitString(buff);
        promoInUse[npromoIn].pid = f;
        strcpy(promoInUse[npromoIn].progPath, progPath);
        promoInUse[npromoIn].ativa = 1;

        strcpy(promoInUse[npromoIn].cat, recPromo.comando[0]);
        promoInUse[npromoIn].perc = atoi(recPromo.comando[1]);
        promoInUse[npromoIn].tempoSeg = atoi(recPromo.comando[2]);
        applyPromo(promoInUse[npromoIn]);
        npromoIn++;
    }
    if (f == 0)
    {
        close(pf[0]);
        close(1);
        dup(pf[1]);
        close(pf[1]);
        execl(progPath, progPath, (char *)NULL);
        printf("PROGRAMA NAO ENCONTRADO!\n");
        exit(-1);
    }
    return -1;
}

void cancelPromos(){
    int i=0;
    for(i=0;i<npromoIn;i++){
        printf("Promocao %s a terminar!\n",promoInUse[i].progPath);
        promoInUse[i].ativa=0;
        kill(promoInUse[i].pid, SIGINT);
    }

    npromoIn=0;    

}

void loadPromotores(){
    FILE *fptr;
    fptr = fopen(FPROMOTERS, "r");
    char nome[125];
    
    if(fptr==NULL){
        printf("FICHEIRO DE PROMOTORES NÃO ENCONTRADO!\n\n");
        return;
    }
    
    while (fgets(nome, sizeof(nome), fptr)) {
        if(npromo!=MAXPROMO){
            if(nome[strlen(nome)-1]=='\n')
                nome[strlen(nome)-1]='\0';
            strcpy(promo[npromo],nome);
            npromo++;
        }
    }
    fclose(fptr);
    printf("\n[%d promotores em sistema]:\n\n", npromo);
}

void loadTempo(){
    FILE *fptr;
    fptr = fopen(FTEMPO, "r");
    char st_tempo[BUFFERMAX];
    if(fptr==NULL){
       fptr = fopen(FTEMPO, "w"); 
       printf("FICHEIRO DE TEMPO NÃO ENCONTRADO. A COMEÇAR TEMPO DE BACKEND A 0 SEGUNDOS!\n\n"); 
       fprintf(fptr, "%d", tempo);
       fclose(fptr);
        return;
    }
    while (fgets(st_tempo, sizeof(st_tempo), fptr)){
    }
    tempo = atoi(st_tempo);
    return;
}

void loadUtilizadores(){
    char nome[125];
    struct command splitLine;
    char aux[125];

    FILE *fptr;
    fptr = fopen(FUSERS, "r");

    if(fptr==NULL){
        printf("FICHEIRO DE UTILIZADORES NÃO ENCONTRADO!\n\n");
        return;
    }

    while (fgets(nome, sizeof(nome), fptr)) {
        strcpy(aux,nome);
        splitLine = splitString(nome);
        if(splitLine.param==3){
           
           
           if(nusers<MAXUSERS){
                strcpy(users[nusers].username,splitLine.comando[0]);
                strcpy(users[nusers].pw,splitLine.comando[1]);
                users[nusers].saldo=atoi(splitLine.comando[2]);
                users[nusers].online=DCNTD;
                nusers++;
            }else{
                printf("MAXUSERS atingido!\n\n");
            }
        }else{
            printf("[LINHA INCORRETAMENTE ESCRITA! %s]\n\n", aux);
        }
    }
   fclose(fptr);
        
}

void loadItems(){
    char nome[125];
    struct command splitLine;
    char aux[125];

    FILE *fptr;
    fptr = fopen(FITEMS, "r");

    if(fptr==NULL){
        printf("FICHEIRO DE ITENS NÃO ENCONTRADO!\n\n");
        return;
    }

    while (fgets(nome, sizeof(nome), fptr)) {
        strcpy(aux,nome);
        splitLine = splitString(nome);
        if(splitLine.param==8){
            if(nitems!=MAXITEMS){
                items[nitems].id=atoi(splitLine.comando[0]);
                strcpy(items[nitems].nome,splitLine.comando[1]);
                strcpy(items[nitems].categoria,splitLine.comando[2]);
                items[nitems].vBase=atoi(splitLine.comando[3]);
                items[nitems].compreJa= atoi(splitLine.comando[4]);
                items[nitems].duracao=atoi(splitLine.comando[5]);
                strcpy(items[nitems].vendedor,splitLine.comando[6]);
                strcpy(items[nitems].licitador,splitLine.comando[7]);
                strtok(items[nitems].licitador, "\n");
                nitems++;
            }
        }else{
            if(splitLine.comando[0][0]=='\n'){}else{
            printf("[LINHA INCORRETAMENTE ESCRITA! %s]\n\n", aux);}
        }   
    }
    printf("\n[%d items em sistema]\n\n", nitems);
    lastItemId=items[nitems-1].id;
    printf("LAST ITEM ID %d\n\n", lastItemId);
   
   fclose(fptr);
}

void saveItems(){
    char nome[BUFFERMAX];
    struct command splitLine;
    char aux[125];
    int i;
    FILE *fptr;
    fptr = fopen(FITEMS, "w");

    if(fptr==NULL){
        printf("FICHEIRO DE ITENS NÃO ENCONTRADO!\n\n");
        return;
    }
        for(i=0;i<nitems;i++){
            if(items[i].id!=-1){
                fprintf(fptr, "\n%d %s %s %d %d %d %s %s",
                    items[i].id,
                    items[i].nome, 
                    items[i].categoria, 
                    items[i].vBase, 
                    items[i].compreJa,
                    items[i].duracao,
                    items[i].vendedor,
                    items[i].licitador);
            }
        }
    fclose(fptr);
}

void printUtil(){
    int i=0;
    for(i=0;i<nusers;i++){                
        printf("Nome: %s | Password: %s | Saldo: %d\n\n", users[i].username, users[i].pw, users[i].saldo);
    }
    
}

void printOnUsers(){
    int i=0;
    for(i=0;i<nusers;i++){ 
        if(users[i].online==CNTD)               
            printf("Nome: %s | Password: %s | Saldo: %d\n\n", users[i].username, users[i].pw, users[i].saldo);
    }
}

void printPromo(){
    int i=0;
    printf("Promotores guardados no sistema: \n\n");
    for(i=0;i<npromo;i++){
        printf("[%s]\n\n", promo[i]);
    }
}

void printItems(){
    int i=0;
    printf("Itens guardados no sistema: \n\n");
    for(i=0;i<nitems;i++){                
        
        if(items[i].id!=-1){
            printf("[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                    items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                    items[i].vendedor,items[i].licitador);
        }
    }
}

void userOperations(){

    char buffer[125];
    struct command comm;
    int errorType=999;
    retiraUmSaldoTodos();
    printf("Operações de utilizador [listar (retira 1 de saldo a todos) | pass [username] [password] (testa user/password) | checkbal [username] (ve saldo de utilizador) | updbal [username] [saldo_novo] (atualiza o saldo do user)]: ");
    scanf("%[^\n]", buffer);
    comm = splitString(buffer);
    while (getchar() != '\n')
            continue;

    if(!strcmp(comm.comando[0],"listar")){
        if(comm.param!=1){
            printf("parâmetros incorretos! Utilisar apenas 'listar'\n\n");
            return;
        }
        printUtil();
        return;
    }

    if(!strcmp(comm.comando[0],"pass")){
        if(comm.param!=3){
            printf("parâmetros incorretos! pass [username] [password]\n\n");
            return;
        }
        errorType = isUserValid(comm.comando[1],comm.comando[2]);
        if(errorType==-1){
            printf("ERRO: %s\n\n", getLastErrorText());
            return;
        }
        if(errorType==0){
            printf("User/pass inválida!\n\n");
            return;
        }
        printf("CORRETO!\n\n");
        return;
    }

    if(!strcmp(comm.comando[0],"checkbal")){
        if(comm.param!=2){
            printf("parâmetros incorretos! checkbal [username]\n\n");
            return;
        }
        errorType=getUserBalance(comm.comando[1]);
        if(errorType==-1){
            printf("ERRO: %s\n\n", getLastErrorText());
            return;
        }
        printf("SALDO: %d\n\n", errorType);
        return;
    }

    if(!strcmp(comm.comando[0],"updbal")){
        if(comm.param!=3){
            printf("parâmetros incorretos! updbal [username] [saldo]\n\n");
            return;
        }
        printf("[DEBUG] SALDO NOVO %d\n\n", atoi(comm.comando[2]));
        errorType=updateUserBalance(comm.comando[1],atoi(comm.comando[2]));
        printf("ERRORTYPE %d\n\n", errorType);
        if(errorType==-1){
            printf("ERRO: %s\n\n", getLastErrorText());
            return;
        }else{
            printf("SALDO ATUALIZADO!\n\n");
            int i;
            for (i=0; i<nusers; i++){
                if(!strcmp(users[i].username,comm.comando[1])){
                    users[i].saldo=getUserBalance(users[i].username);
                    break;
                }

            }
            saveUsersFile(FUSERS);
            return;
        }
        
        return;
    }

    printf("Comando inválido.\n\n");
    return;
}

void promoOperations(){
    char buffer[125];
    struct command comm;
    int errorType=999;

    printf("Operações de promotor  [listar | run (lança primeiro promotor da lista) | cpromos (cancela todas as promocoes em execução)]: ");
    scanf("%[^\n]", buffer);
    comm = splitString(buffer);
    while (getchar() != '\n')
            continue;

    if(!strcmp(comm.comando[0],"listar")){
        if(comm.param!=1){
            printf("parâmetros incorretos! Utilisar apenas 'listar'\n\n");
            return;
        }
        printPromo();
        printf("ATIVOS: \n\n");
        printPromosInUse();
        return;
    }

    if(!strcmp(comm.comando[0],"run")){
        if(comm.param!=1){
            printf("parâmetros incorretos! Utilisar apenas 'run'\n\n");
            return;
        }
        execPromo(promo[0]);
        return;
    }

    if(!strcmp(comm.comando[0],"cpromos")){
        if(comm.param!=1){
            printf("parâmetros incorretos! Utilisar apenas 'cpromos'\n\n");
            return;
        }
        cancelPromos();
        return;
    }

    printf("Comando inválido.\n\n");
    return;
}

void oth_comm(){
    char buffer[125];
    while (getchar() != '\n')
            continue;
    printf("O que deseja testar [promotor | utilizadores | itens (apenas apresenta lista de itens existentes)]: ");
        scanf("%[^\n]", buffer);

        while (getchar() != '\n')
            continue;   

         if(!strcmp(buffer,"utilizadores")){
            userOperations();
        }

        if(!strcmp(buffer,"promotor")){
            promoOperations();
        }

        if(!strcmp(buffer,"itens")){
            printItems();
        }

}

int interpComm(comando comandorec){
    if(!strcmp(comandorec.comando[0],"users")){
        if(comandorec.param!=1){
            printf("not ok.\n\n");
            
            return -2;
        }
        printf("UTILIZADORES REGISTADOS\n\n");
        printUtil();
        printf("UTILIZADORES ONLINE\n\n");
        printOnUsers();
        return 0;
     }

    if(!strcmp(comandorec.comando[0],"list")){
        if(comandorec.param!=1){
            printf("not ok.\n\n");
            return -2;
        }
        
        printf("LISTA DE ITENS ATUAIS\n\n");
        printItems();
        return 0;
     }
    
    if(!strcmp(comandorec.comando[0],"kick")){
        if(comandorec.param!=2){
            printf("not ok.\n\n");
            return -2;
        }
        printf("BANIR USER %s\n\n", comandorec.comando[1]);
        struct user usr;
        strtok(comandorec.comando[1],"\n");
        strcpy(usr.username,comandorec.comando[1]);
        banirUser(usr);
        
        return 0;
     }


     if(!strcmp(comandorec.comando[0],"promo")){
        if(comandorec.param!=2){
            return -2;
        }
        printf("INICIAR %s:\n\n",comandorec.comando[1]);
        execPromo(comandorec.comando[1]);
        return 0;
     }

     if(!strcmp(comandorec.comando[0],"cancel")){
        if(comandorec.param!=2){
            return -2;
        }
        printf("ENCERRA %s\n\n",comandorec.comando[1]);

        cancelPromos();
        return 0;
     }

    if(!strcmp(comandorec.comando[0],"others")){
        if(comandorec.param!=1){
            return -2;
        }
        oth_comm();
        return 0;
    }


    if(!strcmp(comandorec.comando[0],"close")){
        if(comandorec.param!=1){
            return -2;
        }
        running=0;
        printf("BACKEND A ENCERRAR...\n\n");       
        
        trata_terminar();
        return 0;
     }
    return(-1);
}

void comand(){
    char buffer[125];
    struct command comm;
    int errorType;
        printf("Command: ");
        scanf("%[^\n]", buffer);

        if(!strcmp(buffer,"")){
            while (getchar() != '\n')
                continue;
            return;
        }
        comm = splitString(buffer);
        printf("Comando lido: \n\n");

        if((errorType= interpComm(comm))<0){
            printf("ERRO: ");
            if(errorType==-1){
                printf("Comando não encontrado!\n\n");
            }
            if(errorType==-2){
                printf("Parametros incorretos!\n\n");
            }
            if(errorType==-3){
                printf("Comportamento não esperado (erro interno)!\n\n");
            }
        }
        while (getchar() != '\n')
            continue;
        strcpy( buffer,"");
}

void *ftempo(){
    while(running){
        tempo++;
        updateItems();
        reduceHBUsers();
        checkHbAndKick();
        checkPrInUse();
        sleep(1);
    }
}

void *KB(){
    while(running){
        comand();        
    }

    running=0;
}

int userPIDByName (char*un){
    int i;
    for(i=0;i<=nusers;i++){
        if(!strcmp(un,users[i].username)){
            return users[i].pid;
        }
    }
    return -1;
}

int idUserByName(char*un){
    int i;
    for(i=0;i<=nusers;i++){
        if(!strcmp(un,users[i].username)){
            return i;
        }
    }
    return -1;
}

int isonline(char *un){
    int i;

    for(i=0;i<=nusers;i++){
        if(!strcmp(un,users[i].username)){
            if(users[i].online== CNTD)
                return 1;
            else
                return 0;
                }
    }
    return -1;
}

void loginUser(struct user usr){
    int i;
    for(i=0;i<=nusers;i++){
        if(!strcmp(usr.username,users[i].username)){
            users[i].online=CNTD;
            users[i].pid=usr.pid;
            users[i].hbeat=heartbeat;
        }
    }
}

void logoffUser(struct user usr){
    int i;
    for(i=0;i<=nusers;i++){
        if(!strcmp(usr.username,users[i].username)){
            users[i].online=DCNTD;
            users[i].pid=-1;
        }
    }
}

void warnNewItem(struct item itm){
    int i,fdr, aux;
    char fiforesp[BUFFERMAX];
    struct beMsg rspBE;

    for(aux=0;aux<nusers;aux++){
        if(users[aux].online==CNTD){
            sprintf(rspBE.msg,"Novo item:\n\n[ID: %d | Nome: %s | Cat.: %s | P.Base: %d | Compre Já: %d]", itm.id,itm.nome,itm.categoria,itm.vBase,itm.compreJa);
            sprintf(fiforesp,FENDFIFO,users[aux].pid);
            fdr = open(fiforesp, O_WRONLY);
            write(fdr, &rspBE, sizeof(rspBE));
            close(fdr);
        }
    }

}

void warnSoldItem(struct item itm){
    int i,fdr, aux;
    char fiforesp[BUFFERMAX];
    struct beMsg rspBE;

    for(aux=0;aux<nusers;aux++){
        if(users[aux].online==CNTD){
            sprintf(rspBE.msg,"O ITEM %s FOI VENDIDO A %s! (PAGOU %d)", items[i].nome,items[i].licitador, items[i].vBase);
            sprintf(fiforesp,FENDFIFO,users[aux].pid);
            fdr = open(fiforesp, O_WRONLY);
            write(fdr, &rspBE, sizeof(rspBE));
            close(fdr);
        }
    }
}

void interpUserComm(struct usrMsg recUser, char* fiforesp){
    comando comandorec;
    comandorec=splitString(recUser.msg);
    int fdr;
    struct beMsg rspBE;
    int i;
    int itemInserido=0;

    if(!strcmp(comandorec.comando[0],"lival")){
        char tmpbuff[BUFFERMAX];
        if(comandorec.param!=2){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!\n");
        }else{
            if(atoi(comandorec.comando[1])==0){
                sprintf(rspBE.msg, "Preço não introduzido como número!\n");
            }else{

                if(atoi(comandorec.comando[1])>0){
                    i=0;
 
                    sprintf(rspBE.msg,"Preço base até %d:\n\n",atoi(comandorec.comando[1]));
                    if(items[i].id!=-1 && items[i].vBase<=atoi(comandorec.comando[1])){
                        sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                        strcat(rspBE.msg, tmpbuff); 
                    }
                    for(i=1;i<nitems;i++){
                        if(items[i].id!=-1 && items[i].vBase<=atoi(comandorec.comando[1])){ 
                            sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                            strcat(rspBE.msg, tmpbuff); 
                        }              
                        
                    }
                    i=0;

                    sprintf(tmpbuff,"Preço compre já até %d:\n\n",atoi(comandorec.comando[1]));
                    strcat(rspBE.msg, tmpbuff); 
                    if(items[i].id!=-1 && items[i].compreJa<=atoi(comandorec.comando[1])){
                        sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                        strcat(rspBE.msg, tmpbuff); 
                    }
                    for(i=1;i<nitems;i++){
                        if(items[i].id!=-1 && items[i].compreJa<=atoi(comandorec.comando[1])){ 
                            sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                            strcat(rspBE.msg, tmpbuff); 
                        }              
                        
                    }
                }else{
                    sprintf(rspBE.msg, "Preço é um valor negativo!\n");
                }
            }
        }
        rspBE.type = BE_RESPONSE;
        
        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

    if(!strcmp(comandorec.comando[0],"litime")){
        char tmpbuff[BUFFERMAX];
        if(comandorec.param!=2){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!\n");
        }else{
            if(atoi(comandorec.comando[1])==0){
                sprintf(rspBE.msg, "Tempo não introduzido como número!\n");
            }else{

                if(atoi(comandorec.comando[1])>tempo){
                    i=0;
                    if(items[i].id!=-1 && (tempo + items[i].duracao)<=atoi(comandorec.comando[1])){
                        sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                        strcat(rspBE.msg, tmpbuff); 
                    }
                    for(i=1;i<nitems;i++){
                        if(items[i].id!=-1 && (tempo + items[i].duracao)<=atoi(comandorec.comando[1])){ 
                            sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                                items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                                items[i].vendedor,items[i].licitador);
                            strcat(rspBE.msg, tmpbuff); 
                        }              
                        
                    }
                }else{
                    sprintf(rspBE.msg, "Tempo introduzido menor que tempo atual!\n");
                }
            }
        }
        rspBE.type = BE_RESPONSE;
        
        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

    if(!strcmp(comandorec.comando[0],"licat")){
        char tmpbuff[BUFFERMAX];
        if(comandorec.param!=2){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }else{
            strtok(comandorec.comando[1], "\n");
            i=0;
            sprintf(rspBE.msg,"LISTAGEM DE INTENS DE CATEGORIA %s\n\n", comandorec.comando[1]);
            if(items[i].id!=-1 && !strcmp(items[i].categoria,comandorec.comando[1])){
                sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                        items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                        items[i].vendedor,items[i].licitador);
                strcat(rspBE.msg, tmpbuff); 
            }
            for(i=1;i<nitems;i++){
                if(items[i].id!=-1 && !strcmp(items[i].categoria,comandorec.comando[1])){ 
                    sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                        items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                        items[i].vendedor,items[i].licitador);
                    strcat(rspBE.msg, tmpbuff); 
                }              
                
            }

        }
        rspBE.type = BE_RESPONSE;
        
        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }
       
    if(!strcmp(comandorec.comando[0],"lisel")){
        char tmpbuff[BUFFERMAX];
        if(comandorec.param!=2){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }else{
            strtok(comandorec.comando[1], "\n");
            if(idUserByName(comandorec.comando[1])!=-1){
                i=0;
                sprintf(rspBE.msg,"LISTAGEM DE INTENS DO VENDEDOR %s\n\n", comandorec.comando[1]);
                if(items[i].id!=-1 && !strcmp(items[i].vendedor,comandorec.comando[1])){
                    sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                            items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                            items[i].vendedor,items[i].licitador);
                    strcat(rspBE.msg, tmpbuff); 
                }
                for(i=1;i<nitems;i++){
                    if(items[i].id!=-1 && !strcmp(items[i].vendedor,comandorec.comando[1])){ 
                        sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                            items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                            items[i].vendedor,items[i].licitador);
                        strcat(rspBE.msg, tmpbuff); 
                    }              
                    
                }
            }else{
                sprintf(rspBE.msg,"%s NÃO EXISTE REGISTADO",comandorec.comando[1]);
            }
        }
        rspBE.type = BE_RESPONSE;
        
        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

    if(!strcmp(comandorec.comando[0],"sell")){
        printf("sell!\n\n");
        if(comandorec.param!=6){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");

        }else{
        if(atoi(comandorec.comando[3])!=0 && 
        atoi(comandorec.comando[3])!=0 && 
        atoi(comandorec.comando[3])!=0){
            sprintf(rspBE.msg, "Item inserido com sucesso!");
            printf("Nome %s || Categoria %s || Preço = %d || Preço compre já = %d || Duração = %d seg\n\n", 
                                                                                comandorec.comando[1],
                                                                                comandorec.comando[2],
                                                                                atoi(comandorec.comando[3]),
                                                                                atoi(comandorec.comando[4]),            
                                                                                atoi(comandorec.comando[5]));
            for(i=0; i<nitems;i++){
                    if(items[i].id==-1){
                        itemInserido=1;
                        items[i].id=lastItemId+2;
                        lastItemId=items[i].id;
                        items[i].id=nitems+1;
                        strcpy(items[i].nome,comandorec.comando[1]);
                        strcpy(items[nitems].categoria,comandorec.comando[2]);
                        items[i].vBase=atoi(comandorec.comando[3]);
                        items[i].compreJa= atoi(comandorec.comando[4]);
                        items[i].duracao=atoi(comandorec.comando[5]);
                        strcpy(items[i].vendedor,recUser.who.username);
                        strcpy(items[i].licitador, "SEM_LICITADOR");
                        warnNewItem(items[i]);
                    }
            }  
            
            if(itemInserido==0){
                items[nitems].id=nitems+1;
                strcpy(items[nitems].nome,comandorec.comando[1]);
                strcpy(items[nitems].categoria,comandorec.comando[2]);
                items[nitems].vBase=atoi(comandorec.comando[3]);
                items[nitems].compreJa= atoi(comandorec.comando[4]);
                items[nitems].duracao=atoi(comandorec.comando[5]);
                strcpy(items[nitems].vendedor,recUser.who.username);
                strcpy(items[nitems].licitador, "SEM_LICITADOR");
                warnNewItem(items[nitems]);
                nitems++;
            }

        }else{
            sprintf(rspBE.msg, "Alguns parâmetros estão incorretos");
        }
        
        }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

    if(!strcmp(comandorec.comando[0],"buy")){
        if(comandorec.param!=3){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }else{
            if(atoi(comandorec.comando[1])==0 || atoi(comandorec.comando[2])==0){
                sprintf(rspBE.msg, "Alguns parâmetros estão incorretos");
            }else{
                int iExiste=0;
                for(i=0;i<nitems;i++){
                    if(atoi(comandorec.comando[1])==items[i].id){
                        iExiste=1;
                        sprintf(rspBE.msg,"COMPRAR ITEM COM ID  %d POR %d\n\n", atoi(comandorec.comando[1]), atoi(comandorec.comando[2]));
                        if(getUserBalance(recUser.who.username)<atoi(comandorec.comando[2])){
                            sprintf(rspBE.msg,"NÃO TEM SALDO DISPONIVEL\n\n");
                            break;
                        }
                        if(items[i].compreJa<=atoi(comandorec.comando[2])){
                            items[i].id=-1;
                            updateUserBalance(recUser.who.username, (getUserBalance(recUser.who.username)-items[i].compreJa));
                            updateUserBalance(items[i].vendedor,(getUserBalance(items[i].vendedor)+ items[i].compreJa));
                            warnSoldItem(items[i]);
                            
                            sprintf(rspBE.msg,"COMPRA DIRETA! DESCONTADO VALOR BASE DO PRODUTO!\n\n");
                            break;
                        }
                        if(items[i].vBase>=atoi(comandorec.comando[2])){
                            sprintf(rspBE.msg,"JÁ EXITSE UMA LICITACAO COM PREÇO MAIOR OU IGUAL!\n\n");
                            break;
                        }
                        items[i].vBase=atoi(comandorec.comando[2]);
                        strcpy(items[i].licitador, recUser.who.username);
                        sprintf(rspBE.msg,"AUMENTOU A LICITAÇÃO PARA %d!\n\n", atoi(comandorec.comando[2]));
                       

                        break;                    
                    }

                }
                if(iExiste==0){
                    sprintf(rspBE.msg,"ITEM COM ID %d NÃO EXISTE!\n\n", atoi(comandorec.comando[1]));
                }
            }
        }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

    if(!strcmp(comandorec.comando[0],"list")){
        int i=0;
        char tmpbuff[BUFFERMAX];
        if(comandorec.param!=1){
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }else{
            if(items[i].id!=-1){
                sprintf(rspBE.msg, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                        items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                        items[i].vendedor,items[i].licitador);
            }
            for(i=1;i<nitems;i++){
                if(items[i].id!=-1){ 
                    sprintf(tmpbuff, "[ID: %d | Nome: %s | Cat: %s | V_atual: %d | V_CompreJa: %d | Duracao: %d | Vendedor: %s | Licitador Mais Elevado: %s]\n\n",
                        items[i].id,items[i].nome,items[i].categoria,items[i].vBase,items[i].compreJa,items[i].duracao,
                        items[i].vendedor,items[i].licitador);
                    strcat(rspBE.msg, tmpbuff); 
                }              
                
            }
            }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }
       
    if (!strcmp(comandorec.comando[0], "time"))
    {
        if (comandorec.param != 1)
        {
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }
        else
        {
            sprintf(rspBE.msg, "Tempo: %d", tempo);
        }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        return;
    }

    if (!strcmp(comandorec.comando[0], "cash"))
    {
        if (comandorec.param != 1)
        {
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }else{

            int errorType;
            errorType=getUserBalance(recUser.who.username);

            if(errorType==-1){
                sprintf(rspBE.msg,"ERRO: %s\n\n", getLastErrorText());
            }else{
                sprintf(rspBE.msg,"TEM %d DE SALDO DISPONIVEL\n\n", errorType);
            }
        }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return ;
    }

    if (!strcmp(comandorec.comando[0], "add"))
    {
        if (comandorec.param != 2 || !isNumber(comandorec.comando[1]))
        {
            sprintf(rspBE.msg, "Comando utilizado incorretamente!");
        }
        int errorType;
        errorType=updateUserBalance(recUser.who.username, (getUserBalance(recUser.who.username))+(atoi(comandorec.comando[1])));
        if (errorType == -1)
        {
            sprintf(rspBE.msg, "ERRO: %s\n\n", getLastErrorText());
        }
        else
        {
            sprintf(rspBE.msg,"ADICIONOU %d AO SEU SALDO. TEM %d DE SALDO DISPONIVEL\n\n", atoi(comandorec.comando[1]),getUserBalance(recUser.who.username));
        }
        rspBE.type = BE_RESPONSE;

        fdr = open(fiforesp, O_WRONLY);
        write(fdr, &rspBE, sizeof(rspBE));
        close(fdr);
        strcpy(rspBE.msg,"");
        return;
    }

}

void updateItems(){
    int i,fdr, aux;
    char fiforesp[BUFFERMAX];
    char warning[BUFFERMAX];
    struct beMsg rspBE;

    for (i=0;i<nitems;i++){
        if(items[i].id!=-1){

            if(items[i].duracao>1){
                items[i].duracao--;
            }
            else{
                items[i].id=-1;
                if(idUserByName(items[i].licitador)!=-1){
                    updateUserBalance(items[i].licitador, getUserBalance(items[i].licitador)-items[i].vBase);
                    if(isonline(items[i].licitador)>0){
                        
                        sprintf(rspBE.msg,"COMPROU O ITEM %s POR LEILAO! (PAGOU %d)", items[i].nome, items[i].vBase);
                        sprintf(fiforesp,FENDFIFO,userPIDByName(items[i].licitador));
                        
                        fdr = open(fiforesp, O_WRONLY);
                        write(fdr, &rspBE, sizeof(rspBE));
                        close(fdr);
                        
                        for(aux=0;aux<nusers;aux++){
                            if(users[aux].online==CNTD){
                                sprintf(rspBE.msg,"O ITEM %s FOI VENDIDO A %s! (PAGOU %d)", items[i].nome,items[i].licitador, items[i].vBase);
                                sprintf(fiforesp,FENDFIFO,users[aux].pid);
                                fdr = open(fiforesp, O_WRONLY);
                                write(fdr, &rspBE, sizeof(rspBE));
                                close(fdr);
                            }
                        }

                    }else{
                        printf("%s OFFLINE\n", items[i].licitador);
                    }

                }else{
                    for(aux=0;aux<nusers;aux++){
                            if(users[aux].online==CNTD){
                                sprintf(rspBE.msg,"O ITEM %s NÃO FOI VENDIDO!", items[i].nome);
                                sprintf(fiforesp,FENDFIFO,users[aux].pid);
                                fdr = open(fiforesp, O_WRONLY);
                                write(fdr, &rspBE, sizeof(rspBE));
                                close(fdr);
                            }
                        }
                }
            }
        }
    }
}

void *readpipe(){
	int *loops;
    struct usrMsg recUser;
	int fd, fdr;
    char fiforesp[BUFFERMAX];

    struct beMsg rspBE;

	char buf[BUFFERMAX];
	int bytes=1;
    int errorType;
    mkfifo(befifo,0666);
	fd = open(befifo, O_RDONLY);


	while(running){
		bytes = read(fd, &recUser, sizeof(recUser));
		if(bytes==-1){
			printf("SEM FICHEIRO DE RECEÇÃO DE USERS!\n\n");
			break;
		}
        if(bytes==0){
            continue;
        }
        if(recUser.type==BE_SHUTDOWN)break;
        sprintf(fiforesp, FENDFIFO, recUser.who.pid);
        if(recUser.type==LOGIN){          
            
            errorType = isUserValid(recUser.who.username,recUser.who.pw);
            if(errorType==-1){
                printf("ERRO AO RECEBER USER: %s\n\n", getLastErrorText());

                rspBE.type=BADLOGIN;
                strcpy(rspBE.msg,"USER NAO ACEITE! ERRO INTERNO!");

                fdr=open(fiforesp, O_WRONLY);
                write(fdr, &rspBE , sizeof(rspBE));
                close(fdr);
                continue;
            }
            if(errorType==0){
                printf("Um utilizador tentou entrar com User/pass inválida!\n\n");
                rspBE.type=BADLOGIN;
                strcpy(rspBE.msg,"USER/PASS INCORRETOS!");

                fdr=open(fiforesp, O_WRONLY);
                write(fdr, &rspBE , sizeof(rspBE));
                close(fdr);
                continue;
            }


            printf("\n%s Entrou!\n\n", recUser.who.username);
            
            loginUser(recUser.who);
            rspBE.type=GOODLOGIN;

            sprintf(rspBE.msg,"%d", heartbeat);

            fdr=open(fiforesp, O_WRONLY);
            write(fdr, &rspBE , sizeof(rspBE));
            close(fdr);
        }

        if(recUser.type==COMMAND){
            if(isonline(recUser.who.username)){
                interpUserComm(recUser, fiforesp);
            }
        }

        if(recUser.type==HBEAT){
            if(isonline(recUser.who.username)){
                printf("RECEBIDO HEARTBEAT DE %s\n\n", recUser.who.username);
                resetHBeatUser(recUser.who);
            }
        }

        if(recUser.type==DISCONNECT){
            if(isonline(recUser.who.username)){
                printf("%s Saiu!\n", recUser.who.username);
                logoffUser(recUser.who);
            }
        }

		sleep(2);
	}
	close(fd);
}



void *promoLauncher(void * pPath){
    int pf[2], promoID;
    pid_t p, f;
    comando recPromo;
    struct promo recP;
    int i;

    char buff[BUFFERMAX];
    char progPath[125];
    recP=*(struct promo *)pPath;
    strcpy(progPath,recP.progPath);

    printf("PATH: %s\n\n", progPath);
    
    for(i=0; i<npromoIn;i++){
        if(recP.id==promoInUse[i].id){
            promoID=i;
        }
    }
        
    f= fork();
    if(f<0){
        printf("SON FORK ERROR \n");
        return NULL;
    }
    if(f==0){
        printf("DENTRO DO FILHO A EXECUTAR PROMOTOR!\n\n");
        close(pf[0]);
        close(1);
        dup(pf[1]);
        close(pf[1]);
        execl(progPath, progPath,(char *)NULL);
        printf("PROGRAMA NAO ENCONTRADO!\n");
        exit(-1);
    }
    if(f>0){
        int bytes;
        promoInUse[npromoIn].pid=f;
        npromoIn++;
        close(pf[1]);
        printf("DENTRO DO PAI A ESPERA DE DADOS!\n\n");
        while(running && promoInUse[promoID].ativa==1){
        while((bytes=read(pf[0], buff,BUFFERMAX))<=0)continue; 
        
            recPromo=splitString(buff);
            printf("Categoria %s | Desconto %s por cento | Duração %s segundos\n", recPromo.comando[0],recPromo.comando[1],recPromo.comando[2]);
            sleep(1);

        }
        kill(f,SIGUSR1);
    }

}

void *interpPromo(){
    int i;
    int ultPromoID=0;

	int iretPromos;

    while(running){
        for(i=0; i<npromoIn;i++){
            if(promoInUse[i].ativa==1 && promoInUse[i].execucao==0){
                promoInUse[i].execucao=1;
                pthread_create(& promoInUse[i].tid, NULL,promoLauncher,& promoInUse[i]);
                //THREAD promoLauncher com pid no nome
            }
        }
    }
    for(i=0; i<npromoIn;i++){
            pthread_join( promoInUse[i].tid, NULL);
    }

    
}