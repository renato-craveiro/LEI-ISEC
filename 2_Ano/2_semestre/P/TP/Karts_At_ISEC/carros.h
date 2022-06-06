/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
Número de aluno: 21280286

No âmbito da Unidade Curricular de Programação
Licenciatura em Engenharia Informática - ISEC - 2018/2019

-----------------------------------------------------------*/

#ifndef CARROS_H
#define CARROS_H

struct veiculo{
    int id;
    int potencia;
    int avariado;
};

typedef struct veiculo Carro, *pCarro;

pCarro cria_vetor_carros(char *nomefich, int *tam);
void mostraCarros(Carro v[], int tam);
void mostraCarroUnico(int id, Carro v[], int tam);
int contaCarrosDisp(Carro v[], int tam);
void guardaCarros(Carro v[], int tam, char *nomefich);

#endif // CARROS_H
