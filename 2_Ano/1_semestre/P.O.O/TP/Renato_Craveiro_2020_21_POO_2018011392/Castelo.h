#pragma once
#include "Continente.h"
class Castelo: public Continente
{
	static int id_count;
	int id;
public:
	Castelo();
	void evolui(int turno) override;
};

