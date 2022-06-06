#include "Duna.h"
Duna::Duna() {
	id = id_count++;
	Nome = "Duna" + to_string(id);
	resistencia = 4;
	produtos = 1;
	ouro = 0;
}
int Duna::id_count;