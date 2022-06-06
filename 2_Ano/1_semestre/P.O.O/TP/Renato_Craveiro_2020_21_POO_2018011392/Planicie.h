#pragma once
#include "Continente.h"
class Planicie: public Continente
{
	static int id_count;
	int id;
	bool evoluiu;
public:
	Planicie();
	void evolui(int turno) override;
};

