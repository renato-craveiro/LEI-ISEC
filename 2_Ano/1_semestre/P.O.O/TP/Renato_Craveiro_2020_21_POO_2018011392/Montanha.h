#pragma once
#include "Continente.h"
class Montanha: public Continente
{
	int id;
	static int id_count;
	bool evoluiu;
public:
	Montanha();
	void evolui(int turno) override;
};

