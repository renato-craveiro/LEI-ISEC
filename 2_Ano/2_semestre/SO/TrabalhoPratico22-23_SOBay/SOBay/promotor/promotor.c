/*Desenvolvido por Renato Craveiro (2018011392) - LEI-PL - Ano Letivo 2022/2023 - Sistemas Operativos*/
#include "../SOBay_common/soBay.h"

#define NPROMOS 100

#define TEMPO 10

struct  promot{   
    char categ[50];       
    int perc;
    int tempo_s;
};

char cat[5][50]={"construcao","informatica","gaming","literatura","escritorio"};

struct promot promocoes[NPROMOS];

void preenchePromos(){
    time_t t1;
    srand ( (unsigned) time (&t1));
    int num,i;
    for(i=0;i<NPROMOS;i++){
        promocoes[i].perc = rand() % 100 + 1;
        //=num;
        promocoes[i].tempo_s = rand() % 300 + 30;
        //=num;
        num = rand() % 4;
        strcpy(promocoes[i].categ,cat[num]);
    }

}

void lancaPromo(){
    time_t t1;
    int num;
    srand ( (unsigned) time (&t1));
    while((num = rand() % NPROMOS-1)<0);
    
    printf("%s %d %d\n",promocoes[num].categ,promocoes[num].perc,promocoes[num].tempo_s);
}


int main(){
    signal(SIGALRM,lancaPromo); // Register signal handler
    preenchePromos();
    int i;
    /*for(i=0;i<NPROMOS;i++){
        printf("[%s %d %d]\n", promocoes[i].categ,promocoes[i].perc,promocoes[i].tempo_s);
    }*/

    lancaPromo();
    while(1){
        alarm(3);
        pause();
    }  
}