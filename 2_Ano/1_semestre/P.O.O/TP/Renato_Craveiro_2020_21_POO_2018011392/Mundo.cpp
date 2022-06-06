#include "Mundo.h"

//void Mundo::adicionaTerritorio(string Nome, int resistencia, int produtos, int ouro, int pontos) {
//	Territorio temp(Nome, resistencia, produtos, ouro, pontos);
//	territoriosMundo.push_back(&temp);
//}

void Mundo::adicionaTerritorio(Territorio* t) {
	territoriosMundo.push_back(t);
}

void Mundo::novoTurno() {
	if (turno < 12) 
		turno++;
	else 
		return;
	vector<Territorio*>::iterator it;
	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++) {
		(*it)->evolui(turno);
	}
}

Territorio* Mundo::getTerritorioNome(string nome) {
	vector<Territorio*>::iterator it;
	int i = 0;
	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++,i++) {
		if ((*it)->getNome() == nome) {
			return territoriosMundo.at(i);
		}
	}
	return NULL;
}

int Mundo::getTurno() {
	return turno;
}
int Mundo::getNumNaoConquistados() {
	int n=0;
	vector<Territorio*>::iterator it;
	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++) {
		if ((*it)->getConquistado()) {
			n++;
		}
	}
	return n;
}

string Mundo::getNaoConquistados() {
	string s;
	vector<Territorio*>::iterator it;

	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++) {
		if(!(*it)->getConquistado())
			s.append("\t" + (*it)->getTerritorio() + "\n\n");
	}
	return s;
}

string Mundo::getConquistados() {
	string s;
	vector<Territorio*>::iterator it;

	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++) {
		if ((*it)->getConquistado())
			s.append("\t" + (*it)->getTerritorio() + "\n\n");
	}
	return s;
}
string const Mundo::getMundo() {
	string s;

	s.append("--Mundo--\n\nTerritorios:\n");

	vector<Territorio*>::iterator it;

	for (it = territoriosMundo.begin(); it != territoriosMundo.end(); it++) {
		s.append("\t" + (*it)->getTerritorio() + "\n\n");
	}
	if (turno < 6) {
		s.append("Ano: 1\n");
	}
	else {
		s.append("Ano: 2\n");
	}
	s.append("Turno: " + to_string(turno)+"\n\n");
	return s;
}

