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


/*A função cria_vet_pilotos tem como objetivo devolver um
vetor de estruturas Piloto preenchido com os dados que se encontram
no ficheiro pilotos.txt*/

pPiloto cria_vet_pilotos(char *nomefich,int *tam){
//Cria o vetor dinâmico dos pilotos, indo buscar os mesmos ao ficheiro dos pilotos
    pPiloto v=NULL;
    Piloto aux;

    FILE *f;
    int contador=0,i=0;

    f = fopen(nomefich, "rt");
    if(f==NULL){ fprintf(stderr,"ERRO ao abrir o ficheiro %s\n", nomefich); return NULL;}
    //Caso ocorra um erro na abertura do ficheiro a função apresenta uma mensagem de erro


    while(fscanf(f," %99[^\n] %d %d %d %d %d %f %d\n\n", aux.nome,
                 &aux.id, &aux.dia, &aux.mes,
                 &aux.ano, &aux.peso, &aux.experiencia, &aux.impedimento)==8){contador++;}
    //Percorre o ficheiro pilotos.txt e conta quantos pilotos (e os seus dados) estão presentes no mesmo

    rewind(f);
    //Volta para o início do ficheiro

    if(contador==0){
        fprintf(stderr,"ERRO: o ficheiro %s encontra-se vazio, sendo que nao existem pilotos no mesmo.\n\n", nomefich);
        fclose(f);
        return NULL;
    }/*Caso não tenha sido encontrado nenhum piloto (ou seja o contador esteja a zero (0)
    mostra uma mensagem de erro fecha o ficheiro e devolve o vetor vazio*/

    v=malloc(contador * sizeof(Piloto)); //Reserva espaço suficiente para os pilotos encontrados no vetor
    if(v == NULL){
        fprintf(stderr,"ERRO: nao foi possivel reservar memoria para o numero de pilotos no ficheiro %s.\n\n",nomefich);
        fclose(f);
        return NULL;
    }//Se, por alguma razão, não tenha sido reservada a memória no vetor, fecha o ficheiro e devolve o vetor vazio

    for(i=0;i<contador;i++){
        fscanf(f," %99[^\n] %d %d %d %d %d %f %d\n\n", v[i].nome, &v[i].id, &v[i].dia, &v[i].mes,
                &v[i].ano, &v[i].peso, &v[i].experiencia, &v[i].impedimento);
    }//Lê os dados do ficheiro para o vetor até atingir o número de pilotos contados

    *tam=contador; //Altera a variável tamanho que foi passada como parâmetro para o tamanho atual do vetor

    fclose(f);
    return v;
}


void mostraPilotos(Piloto v[], int tam){
//Imprime os pilotos guardados no array dinâmico
    int i;


    if (tam ==0){
        fprintf(stdout, "Nao existem pilotos na lista.\n\n");
        return;
    }
    for(i=0; i<tam; i++){
        fprintf(stdout,"%s\nID: %d\tData de Nascimento: %2d / %2d / %d\t Peso: %dkg \tExperiencia: %0.2f\tImpedimentos: %d\n\n", v[i].nome,
               v[i].id, v[i].dia, v[i].mes, v[i].ano, v[i].peso, v[i].experiencia, v[i].impedimento);

    }
}

void mostraPilotoUnico(int id, Piloto v[], int tam){
//Imprime o piloto guardado no array dinâmico com o id fornecido
    int i;
    if (tam ==0){
        fprintf(stdout, "Nao existem pilotos na lista.\n\n");
        return;
    }
    for(i=0; i<tam; i++){
        if(id==v[i].id){
            fprintf(stdout,"%s\nID: %d\tData de Nascimento: %2d / %2d / %d\t Peso: %dkg \tExperiencia: %0.2f\tImpedimentos: %d\n\n", v[i].nome,
            v[i].id, v[i].dia, v[i].mes, v[i].ano, v[i].peso, v[i].experiencia, v[i].impedimento);
            return;
        }
    }

    fprintf(stdout, "O piloto nao foi encontrado.\n\n");
    return;
}

int contaPilotosDisp(Piloto v[], int tam){
//Devolve o numero de pilotos que não tenham nenhum impedimento
    int i, cont=0;
    if (tam ==0){
        fprintf(stdout, "Nao existem pilotos na lista.\n\n");
        return 0;
    }
    for(i=0; i<tam; i++){
        if(v[i].impedimento==0){
            cont++;
        }
    }

    return cont;
}


void penaliza(Piloto listaPiloto[], int tamPiloto){
//Altera o valor do impedimento do piloto com o id que seja fornecido na função
    int penalizacao, id, encontrou=0,i;
    if(tamPiloto==0){
        fprintf(stdout,"A lista de Pilotos encontra-se vazia!\n\n");
        return;
    }
    fprintf(stdout,"Insira o ID do piloto a penalizar: ");
    do{
        fscanf(stdin,"%d", &id);
        for(i=0;i<tamPiloto;i++){
            if(id==listaPiloto[i].id){
                encontrou=1;
                break;
            }
        }
        if(!encontrou)
            fprintf(stdout,"\nNao foi encontrado o piloto com o Id fornecido, insira um id correto: ");
    }while(!encontrou);

    printf("Insira a penatizacao a atribuir ao piloto [1-3]: ");
    do{
       fscanf(stdin,"%d", &penalizacao);
    }while(penalizacao<1||penalizacao>3);
    for(i=0;i<tamPiloto;i++){
        if(id==listaPiloto[i].id){
            listaPiloto[i].impedimento+=penalizacao;
            break;
        }
    }

}


void guardaPilotos(Piloto v[], int tam, char *nomefich){
//Guarda o vetor dinâmico no ficheiro dos pilotos
    FILE *f;
    int contador=0,i=0;

    f = fopen(nomefich, "wt");
    if(f==NULL){ fprintf(stderr,"ERRO ao abrir o ficheiro %s\n", nomefich); return NULL;}

    if (tam ==0){
        fprintf(stdout, "Nao existem pilotos na lista.\n\n");
        fclose(f);
        return;
    }
    for(i=0; i<tam; i++){
        fprintf(f,"%s\n%d %2d %2d %d %d %0.2f %d\n\n", v[i].nome,
               v[i].id, v[i].dia, v[i].mes, v[i].ano, v[i].peso, v[i].experiencia, v[i].impedimento);

    }
    fclose(f);
}




