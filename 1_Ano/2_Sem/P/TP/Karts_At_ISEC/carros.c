/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
N�mero de aluno: 21280286

No �mbito da Unidade Curricular de Programa��o
Licenciatura em Engenharia Inform�tica - ISEC - 2018/2019

-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


#include<windows.h>

#include "carros.h"

pCarro cria_vetor_carros(char *nomefich, int *tam){
//Cria o vetor din�mico dos carros, indo buscar os mesmos ao ficheiro dos carros
    pCarro v=NULL;
    Carro aux;
    int contador=0,i=0;

    FILE *f;
    f = fopen(nomefich, "rt");
    if(f==NULL){ printf("Erro ao abrir o ficheiro %s\n", nomefich); return NULL;}
    //Caso ocorra um erro na abertura do ficheiro a fun��o apresenta uma mensagem de erro

    while(fscanf(f," %d %d %d \n", &aux.id, &aux.potencia, &aux.avariado)==3){contador++;}
    //Percorre o ficheiro carros.txt e conta quantos carros (e os seus dados) est�o presentes no mesmo

    rewind(f);
    //Volta para o in�cio do ficheiro

    v=malloc(contador * sizeof(Carro));
    for(i=0;i<contador;i++){
        fscanf(f," %d %d %d \n", &v[i].id, &v[i].potencia, &v[i].avariado);
    }

    *tam=i;


    fclose(f);
    return v;
}

void mostraCarros(Carro v[], int tam){
//Imprime os Carros guardados no array din�mico
    int i;

    if (tam ==0){
        fprintf(stdout, "Nao existem carros na lista.\n\n");
        return;
    }

    for(i=0; i<tam; i++){
        printf("ID: %d\tPotencia: %d\tAvariado: %d\n\n", v[i].id, v[i].potencia, v[i].avariado);

    }
}



void mostraCarroUnico(int id, Carro v[], int tam){
//Imprime o carro guardado no array din�mico com o id fornecido
    int i;

    if (tam ==0){
        fprintf(stdout, "Nao existem carros na lista.\n\n");
        return;
    }

    for(i=0; i<tam; i++){
        if(id== v[i].id){
            printf("ID: %d\tPotencia: %d\tAvariado: %d\n\n", v[i].id, v[i].potencia, v[i].avariado);
            return;
        }
    }
}

int contaCarrosDisp(Carro v[], int tam){
//Devolve o numero de carros que n�o tenham nenhuma avaria
    int i, cont=0;
    if (tam ==0){
        fprintf(stdout, "Nao existem carros na lista.\n\n");
        return 0;
    }
    for(i=0; i<tam; i++){
        if(v[i].avariado==0){
            cont++;
        }
    }

    return cont;
}


void guardaCarros(Carro v[], int tam, char *nomefich){
//Guarda o vetor din�mico no ficheiro dos carros
    FILE *f;
    int contador=0,i=0;

    f = fopen(nomefich, "wt");
    if(f==NULL){ fprintf(stderr,"ERRO ao abrir o ficheiro %s\n", nomefich); return NULL;}

    if (tam ==0){
        fprintf(stdout, "Nao existem carros na lista.\n\n");
        fclose(f);
        return;
    }
    for(i=0; i<tam; i++){
        fprintf(f,"%d %d %d \n", v[i].id, v[i].potencia, v[i].avariado);

    }

    fclose(f);
}

