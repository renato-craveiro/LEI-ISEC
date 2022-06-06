#include "Territorio.h"

bool Territorio::conquista(int ataque) { 
	if (ataque > resistencia) {
		conquistado = true;
		return true;
	}
	else
		return false;
};

void Territorio::perde() {
	conquistado = false;
}


string const Territorio::getTerritorio() { 
	string s;
	s.append(Nome+"\n\t");
	s.append("Ouro por turno: "+to_string(ouro)+"\n\t");
	s.append("Produtos por turno: " + to_string(produtos) + "\n\t");
	s.append("Resistencia: " + to_string(resistencia) + "\n\t");
	if (conquistado)
		s.append(to_string(pontos)+" pontos foram recebidos ao ser conquistado.");
	else
		s.append(to_string(pontos)+" pontos serao recebidos se for conquistado");
	return s;
};