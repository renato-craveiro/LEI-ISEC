#pragma once
#include "Territorio.h"
class Continente : public Territorio
{
	
protected:
	Continente();
	string const getTerritorio() override;
};

