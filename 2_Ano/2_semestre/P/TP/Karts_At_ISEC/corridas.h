/*-----------------------------------------------------------

Trabalho realizado por: Renato Alexandre Oliveira Craveiro
N�mero de aluno: 21280286

No �mbito da Unidade Curricular de Programa��o
Licenciatura em Engenharia Inform�tica - ISEC - 2018/2019

-----------------------------------------------------------*/

#ifndef CORRIDAS_H
#define CORRIDAS_H

typedef struct parVol parVolt, *pParVolt;
struct parVol{
        int ganhou;
        float exp;
        int desistiu;
        int segundos;
        int posicao;
};

typedef struct parpc parPilCar, *pParPilCar;
struct parpc{
    Piloto parPiloto;
    Carro parCarro;
    int pIdade;
    int totSeg;
    parVolt *parVoltas;
    pParPilCar prox;
};



typedef struct corr corridas, *pCorridas;
struct corr{
    parPilCar *listaPares;
    int nVoltas;
    int comprimento;
    int maxConcorrentes;
};


void apNaoSelec(Piloto listaPiloto[], Carro listaCarros[],
                int tamCarro, int tamPiloto, int vetSelec[], int vetCarSelec[], int nPilotos);

void parAleat(parPilCar *parunic, Piloto *listaPiloto, Carro *listaCarros,
               int tamCarro, int tamPiloto, int vetSelec[], int vetCarSelec[], int nPilotos);

pParPilCar criaListaPC(Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto, parPilCar *listaPC,
                         int *nPilotos, int nvoltas);

void mostraPares(parPilCar *listaPC, Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto);

corridas corrida(Piloto *listaPiloto, Carro *listaCarros, int tamCarro, int tamPiloto);

#endif // CORRIDAS_H
