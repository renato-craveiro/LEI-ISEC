#pragma once
#include "Ilha.h"
class Pescaria:public Ilha
{
	static int id_count;
	int id;
	bool evoluiu;
public:
	Pescaria();
	void evolui(int turno) override;

};

