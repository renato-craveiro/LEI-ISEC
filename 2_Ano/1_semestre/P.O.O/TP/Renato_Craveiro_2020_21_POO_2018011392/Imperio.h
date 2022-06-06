#pragma once
#include "Mundo.h"
#include "Tecnologias.h"
#include "Debugger.h"
#include <vector>
#include <string>
class Imperio
{
	friend class Tecnologias;
	friend class Debugger;

	vector<Territorio *> territoriosImperio;
	int ouro; //inicialmente ate 3 unid - cofre
	int produtos;//inicialmente ate 3 unid - armazem
	int pontos;//inicialmente 0
	int militares;//inicialmente ate 3 unid mas aumentada ate 5

	int ultimaSorte;

	int max_ouro;
	int max_prod;
	int max_militares;



	

	void gastaOuro();
	void gastaProduto();

	void adicionaOuro();
	void adicionaProdutos();
	void adicionaPontos();
	void adicionaMilitares();

	

public:
	Imperio(Territorio_Inicial * terrInic, Tecnologias *tec);
	string ConquistaTerritorio(Territorio* t);
	void alianca();
	void evRecOuro();
	void evRecProd();

	bool Invasao(int ataque);

	Tecnologias* tecn;
	void RemoveTerritorio(Territorio t);
	void recolheProdutos();
	void recolheOuro();
	void maismilitar();
	void maisouro();
	void maisprod();
	string getImperio();
	int getNumTerr();
	int getPontos();
	int getMaxProdutos();
	int getProdutos();
	int getMaxOuro();
	int getOuro();
	int getMilitares();
	int getMaxMilitares();
	int nTerr();

	bool recolheu = false;

	bool comprouMilitar = false;
	bool comprouTecn = false;

	bool MundoCriado = false;

	int fase[4] = { 0,0,0,0 };


	Territorio* getTerrInic();

	Tecnologias* getTecnologias();
};

