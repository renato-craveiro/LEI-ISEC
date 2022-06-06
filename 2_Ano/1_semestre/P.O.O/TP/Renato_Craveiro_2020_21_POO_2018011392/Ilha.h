#pragma once
#include "Territorio.h"
class Ilha: public Territorio
{
protected:
	string const getTerritorio() override;
	Ilha();
};

