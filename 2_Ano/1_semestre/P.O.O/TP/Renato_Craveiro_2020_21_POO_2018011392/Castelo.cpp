#include "Castelo.h"

Castelo::Castelo() {
	id = id_count++;
	Nome = "Castelo" + to_string(id);
	resistencia = 7;
	ouro = 1;
	produtos = 0;
}
int Castelo::id_count;
void Castelo::evolui(int turno) {
	if ((turno >= 1 && turno <= 2) || (turno >= 7 && turno <= 8)) {
		produtos = 3;
		return;
	}
	else {
		produtos = 0;
		return;
	}
}