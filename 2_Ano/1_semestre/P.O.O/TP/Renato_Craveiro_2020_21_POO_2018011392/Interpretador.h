#pragma once

#include <iostream>
#include <sstream>

#include <algorithm>

#include <fstream>


#include "Tecnologias.h"

#include "Imperio.h"
#include "Mundo.h"

#include "savegame.h"


#include "Debugger.h"
class Interpretador
{
private:
	Debugger debug;

	bool is_number(const std::string& s);
	void addTer(Mundo* world);
	void cria(Mundo* world, int n, Territorio* t);
	void recolha(/*Mundo* world,*/ Imperio* myImperio);
	bool lefich(Mundo* world, Imperio* myImperio, string nomeFicheiro, vector<savegame>* saves);
	void limpaEcra();

	void limpaFases(Imperio* myImperio);

	
	bool procuraSave(string nome, vector<savegame>* saves);
	void loadGame(string nome, vector<savegame>* saves, Imperio* myImperio, Mundo* world);
	bool eliminaSave(string nome, vector<savegame>* saves);


	void evRecAband(Mundo* world, Imperio* myImperio);

	void evInvasao(Mundo* world, Imperio* myImperio);

	void evAlianca(Imperio* myImperio);

	void listaSaves(vector<savegame>* saves);

public:
	bool Recolher(Imperio* myImperio);
	bool Evento(Mundo* world, Imperio* myImperio);
	bool interpretadorComandos(Mundo* world, Imperio* myImperio, string comando, vector<savegame> *saves);
};

