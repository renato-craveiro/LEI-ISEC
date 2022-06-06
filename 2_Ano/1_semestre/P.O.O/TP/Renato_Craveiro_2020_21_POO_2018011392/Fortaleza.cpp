#include "Fortaleza.h"
Fortaleza::Fortaleza() {
	id = id_count++;
	Nome = "Fortaleza" + to_string(id);

	evoluiu = false;
	
	resistencia = 8;
	produtos = 0;
	ouro = 0;
};
int Fortaleza::id_count;
void Fortaleza::evolui(int turno) {
	return;
}