#include "Mina.h"

Mina::Mina() {
	id = id_count++;
	Nome = "Mina" + to_string(id);
	resistencia = 5;
	produtos = 0;
	ouro = 1;
}
int Mina::id_count;
void Mina::evolui(int turno) {
	if ((turno >= 1 && turno<=3) || (turno >=7 && turno<=9)) {
		ouro = 1;
		return;
	}
	else { 
		ouro = 2;
		return; 
	}
}