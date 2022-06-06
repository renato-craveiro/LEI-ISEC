#pragma once
#include "Continente.h"
class Mina: public Continente
{
	static int id_count;
	int id;
public:
	Mina();
	void evolui(int turno) override;
};

