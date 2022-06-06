#include "Debugger.h"
bool Debugger::modifica(Imperio* myImperio, string tipo, int quant) {
	if (tipo.compare("OURO")==0) {
		if (quant > myImperio->max_ouro)
			myImperio->max_ouro = quant;
		myImperio->ouro = quant;

		cout << "\n\n[DEBUGGER: Alterou o ouro do Imperio para: " + to_string(myImperio->getOuro()) + "]\n" << endl;
		return true;
	}
	else if(tipo.compare("PROD")==0){
		if (quant > myImperio->max_prod)
			myImperio->max_prod = quant;
		myImperio->produtos = quant;

		cout << "\n\n[DEBUGGER: Alterou os produtos do Imperio para: " + to_string(myImperio->getProdutos()) + "]\n" << endl;
		return true;
	}

	cout << "\n\n[ERRO: \""+tipo+"\" nao identificado. So' pode alterar ouro (ouro) ou produtos (prod)]\n" << endl;
	return false;
}

bool Debugger::toma(Imperio* myImperio, Mundo* world, string tipo, string nome) {
	if (tipo.compare("TERR") == 0) {
		if (world->getTerritorioNome(nome) == NULL) {
			cout << "\n\n[DEBUGGER ERRO: Territo'rio nao encontrado]\n" << endl;
			return false;
		}
		if (world->getTerritorioNome(nome)->getConquistado()) {
			cout << "\n\n[DEBUGGER ERRO: Territo'rio ja' conquistado]\n" << endl;
			return false;
		}


		myImperio->territoriosImperio.push_back(world->getTerritorioNome(nome));
		(world->getTerritorioNome(nome))->conquista((world->getTerritorioNome(nome)->getResistencia() * 2));
		cout << "\n\n[DEBUGGER: Territo'rio " + nome + " conquistado.]\n" << endl;
		return true;
	}
	
	if (tipo.compare("TEC")) {
		if (nome == "DRONE") {
			myImperio->tecn->ativDrones(myImperio);
			cout << "\n\n[DEBUGGER: Tecnologia " + nome + " ativada.]\n" << endl;
			return true;
		}
		if (nome == "MISSIL") {
			myImperio->tecn->ativMisseis(myImperio);
			cout << "\n\n[DEBUGGER: Tecnologia " + nome + " ativada.]\n" << endl;
			return true;
		}
		if (nome == "DEFESA") {
			myImperio->tecn->ativDefTerr(myImperio);
			cout << "\n\n[DEBUGGER: Tecnologia " + nome + " ativada.]\n" << endl;
			return true;
		}
		if (nome == "BOLSA") {
			myImperio->tecn->ativBolVal(myImperio);
			cout << "\n\n[DEBUGGER: Tecnologia " + nome + " ativada.]\n" << endl;
			return true;
		}
		if (nome == "BANCO") {
			myImperio->tecn->ativBanCent(myImperio);
			cout << "\n\n[DEBUGGER: Tecnologia " + nome + " ativada.]\n" << endl;
			return true;
		}
		cout << "\n\n[DEBUGGER ERRO: Tecnologia nao encontrada]\n" << endl;
		return false;

	}
	return false;
}