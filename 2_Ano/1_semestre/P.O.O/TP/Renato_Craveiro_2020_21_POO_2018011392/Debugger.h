#pragma once
#include "Imperio.h"
#include "Mundo.h"
#include "Tecnologias.h"
class Debugger
{
	friend class Imperio;
	friend class Mundo;
public:
	bool toma(Imperio* myImperio, Mundo* world,string tipo, string nome);
	bool modifica(Imperio* myImperio, string tipo, int quant);
};

