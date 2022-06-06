#pragma once
#include <vector>
#include "Territorio.h"
#include "Continente.h"
#include "Ilha.h"

#include "Planicie.h"
#include "Montanha.h"
#include "Fortaleza.h"
#include "Mina.h"
#include "Duna.h"
#include "Castelo.h"

#include "RefugioPiratas.h"
#include "Pescaria.h"
#include "Debugger.h"
#include "Territorio_Inicial.h"
#include <string>
class Mundo
{
	friend class Debugger;
	vector<Territorio*> territoriosMundo;
	int turno;
public:
	
	Mundo() { turno = 1; }
	int getTurno();
	int getNumNaoConquistados();
	void adicionaTerritorio(Territorio* t);
	void novoTurno();
	Territorio* getTerritorioNome(string nome);
	string getNaoConquistados();
	string getConquistados();
	string const getMundo();
};

