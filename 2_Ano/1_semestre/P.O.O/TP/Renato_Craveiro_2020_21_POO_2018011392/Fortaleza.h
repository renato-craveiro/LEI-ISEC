#pragma once
#include "Continente.h"
class Fortaleza: public Continente
{
	static int id_count;
	int id;
	bool evoluiu;
public:
	Fortaleza();
	void evolui(int turno) override;
};

