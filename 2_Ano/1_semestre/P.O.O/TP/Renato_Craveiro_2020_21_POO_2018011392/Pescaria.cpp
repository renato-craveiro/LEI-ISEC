#include "Pescaria.h"
Pescaria::Pescaria() {
	id = id_count++;
	Nome = "Pescaria" + to_string(id);

	evoluiu = false;

	resistencia = 9;
	produtos = 2;
	ouro = 0;
};

int Pescaria::id_count;

void Pescaria::evolui(int turno) {
	if (!evoluiu && turno >= 6) {
		produtos=4;
		evoluiu = true;
		return;
	}
	else
		return;
}