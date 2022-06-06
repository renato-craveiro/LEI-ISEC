#include "Continente.h"

Continente::Continente() {
	pontos = 1;
	conquistado = false;
}

string const Continente::getTerritorio() {
	string s;
	s.append(Nome + "\n\t");
	s.append("Ouro por turno: " + to_string(ouro) + "\n\t");
	s.append("Produtos por turno: " + to_string(produtos) + "\n\t");
	s.append("Resistencia: " + to_string(resistencia) + "\n\t");
	if (conquistado)
		s.append(to_string(pontos) + " pontos foram recebidos ao ser conquistado.\n\t");
	else
		s.append(to_string(pontos) + " pontos serao recebidos se for conquistado\n\t");
	s.append("Este territorio faz parte de um continente.");
	return s;
}