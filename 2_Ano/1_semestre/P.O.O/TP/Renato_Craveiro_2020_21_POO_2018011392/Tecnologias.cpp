#include "Tecnologias.h"


bool Tecnologias::temDrones() {
	return drones;
}

void Tecnologias::ativDrones(Imperio *myImperio) {
	drones = true;
	myImperio->ouro = myImperio->ouro - 3;
	myImperio->max_militares = 5;
}

bool Tecnologias::compraDrones(Imperio* myImperio){
	if (!temDrones() && myImperio->getOuro() >= 3) {
		
		ativDrones(myImperio);
		cout << "\n\n[Comprou a tecnologia 'Drones militares']\n" << endl;
		return true;
	}
	if(temDrones())
		cout << "\n\n[Ja' tem a tecnologia 'Drones militares']\n" << endl;
	else
		cout << "\n\n[Nao tem ouro suficiente para comprar a tecnologia 'Drones militares']\n" << endl;
	return false;
}

bool Tecnologias::temMisseis() {
	return misseis;
}

bool Tecnologias::temDefTerr() {
	return defTerr;
}

void Tecnologias::ativMisseis(Imperio* myImperio) {
	misseis = true;
}

void Tecnologias::ativDefTerr(Imperio* myImperio){
	defTerr = true;;
}

bool Tecnologias::compraMisseis(Imperio* myImperio){
	if (!temMisseis() && myImperio->getOuro() >= 4) {
		myImperio->ouro = myImperio->ouro - 4;
		ativMisseis(myImperio);
		cout << "\n\n[Comprou a tecnologia 'Misseis teleguiados']\n" << endl;
		return true;
	}
	if(temMisseis())
		cout << "\n\n[Ja' tem a tecnologia 'Misseis teleguiados']\n" << endl;
	else
		cout << "\n\n[Nao tem ouro suficiente para comprar a tecnologia 'Misseis teleguiados']\n" << endl;
	return false;
}

bool Tecnologias::defTerritoriais(Imperio* myImperio){
	if (!temDefTerr() && myImperio->getOuro() >= 4) {
		myImperio->ouro = myImperio->ouro - 4;
		ativDefTerr(myImperio);
		cout << "\n\n[Comprou a tecnologia 'Defesas Territoriais']\n" << endl;
		return true;
	}
	if (temDefTerr())
		cout << "\n\n[Ja' tem a tecnologia 'Defesas Territoriais']\n" << endl;
	else
		cout << "\n\n[Nao tem ouro suficiente para comprar a tecnologia 'Defesas Territoriais']\n" << endl;
	return false;
}

void Tecnologias::ativBolVal(Imperio* myImperio) {
	bVal = true;
}

bool Tecnologias::temBolVal() {
	return bVal;
}

bool Tecnologias::bolsaValores(Imperio* myImperio){
	if (!temBolVal() && myImperio->getOuro() >= 2) {
		myImperio->ouro = myImperio->ouro - 2;
		ativBolVal(myImperio);
		cout << "\n\n[Comprou a tecnologia 'Bolsa de Valores']\n" << endl;
		return true;
	}
	if (temBolVal())
		cout << "\n\n[Ja' tem a tecnologia 'Bolsa de Valores']\n" << endl;
	else
		cout << "\n\n[Nao tem ouro suficiente para comprar a tecnologia 'Bolsa de Valores']\n" << endl;
	return false;
}

void Tecnologias::ativBanCent(Imperio* myImperio) {
	bCent = true;
}


bool Tecnologias::temBanCent() {
	return bCent;
}

bool Tecnologias::BancoCentral(Imperio* myImperio){
	if (myImperio->getOuro() >= 3) {
		myImperio->ouro = myImperio->ouro - 3;
		cout << "\n\n[Comprou a tecnologia 'Banco Central']\n" << endl;
		if (!temBanCent()) {
			ativBanCent(myImperio);
			myImperio->max_ouro = 5;
			myImperio->max_prod = 5;
		}
		else {
			cout << "\n\n[Ja' tinha a tecnologia 'Banco Central', pelo que acabou de desperdic,ar recursos]\n" << endl;
		}
		return true;
	}
	
	cout << "\n\n[Nao tem ouro suficiente para comprar a tecnologia 'Bolsa de Valores']\n" << endl;
	return false;
}

string Tecnologias::getTecnologias() {
	string s;
	if (temDrones())
		s.append("\t[Drones Militares]\n");
	if (temMisseis())
		s.append("\t[Mi'sseis Teleguiados]\n");
	if (temDefTerr())
		s.append("\t[Defesas Territoriais]\n");
	if (temBolVal())
		s.append("\t[Bolsas de Valores]\n");
	if (temBanCent())
		s.append("\t[Banco Central]\n");

	if (s.empty())
		s.append("\tNenhuma tencologia ate' agora adquirida.\n");

	return s;
}