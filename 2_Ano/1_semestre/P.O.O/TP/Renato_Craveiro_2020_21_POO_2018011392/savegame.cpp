#include "savegame.h"


string savegame::getSaveName() {
	return savename;
}

void savegame::loadgame(Imperio* i, Mundo* w, Territorio* t, Tecnologias* tecn) {
	*i = Imp;
	*w = world;
	*t = TI;
	*tecn = Tec;
	cout << "\n\n[Savefile carregado com sucesso]\n" << endl;
}