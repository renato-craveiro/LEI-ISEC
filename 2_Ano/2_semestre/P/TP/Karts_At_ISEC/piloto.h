/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
Número de aluno: 21280286

No âmbito da Unidade Curricular de Programação
Licenciatura em Engenharia Informática - ISEC - 2018/2019

-----------------------------------------------------------*/

#ifndef PILOTO_H
#define PILOTO_H

struct condutor{
    char nome[100];
    int id;
    int dia;
    int mes;
    int ano;
    int peso;
    float experiencia;
    int impedimento;
};

typedef struct condutor Piloto, *pPiloto;

pPiloto cria_vet_pilotos(char *nomefich, int *tam);
void mostraPilotos(Piloto v[], int tam);
void mostraPilotoUnico(int id, Piloto v[], int tam);
int contaPilotosDisp(Piloto v[], int tam);
void penaliza(Piloto listaPiloto[], int tamPiloto);
void guardaPilotos(Piloto v[], int tam, char *nomefich);

#endif // PILOTO_H
