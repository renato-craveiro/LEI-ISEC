#pragma once

#include "Imperio.h"
#include "Mundo.h"

#include "Territorio_Inicial.h"
#include "Tecnologias.h"
class savegame
{
	Imperio Imp;
	Mundo world;

	Territorio TI;
	Tecnologias Tec;

	std::string savename;

public:
	savegame(Imperio *i, Mundo *w, Territorio *t, Tecnologias *tecn, string sname): Imp(*i), world(*w),TI(*t), Tec(*tecn), savename(sname){};
	string getSaveName();

	void loadgame(Imperio* i, Mundo* w, Territorio* t, Tecnologias* tecn);
};

