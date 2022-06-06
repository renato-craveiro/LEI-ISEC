#pragma once
#include "Ilha.h"
class RefugioPiratas: public Ilha
{
	static int id_count;
	int id;
	bool evoluiu;
public:
	RefugioPiratas();
	void evolui(int turno) override;

};

