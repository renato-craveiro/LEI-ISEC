#pragma once

#include "Imperio.h"
class Imperio;
class Tecnologias
{
	bool drones = false;
	bool misseis = false;
	bool defTerr = false;
	bool bVal = false;
	bool bCent = false;

	void ativDrones(Imperio* myImperio);
	void ativMisseis(Imperio* myImperio);
	void ativDefTerr(Imperio* myImperio);
	void ativBolVal(Imperio* myImperio);
	void ativBanCent(Imperio* myImperio);

	friend class Imperio;
	friend class Debugger;




public:

	bool compraDrones(Imperio* myImperio);
	bool compraMisseis(Imperio* myImperio);
	bool defTerritoriais(Imperio* myImperio);
	bool bolsaValores(Imperio* myImperio);
	bool BancoCentral(Imperio* myImperio);

	bool temDrones();
	bool temMisseis();
	bool temDefTerr();
	bool temBolVal();
	bool temBanCent();

	std::string getTecnologias();
	friend class Deugger;
};

