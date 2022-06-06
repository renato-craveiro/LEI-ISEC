#include "Planicie.h"

Planicie::Planicie() {
	id = id_count++;
	Nome = "Planicie" + to_string(id);
	evoluiu = false;
	resistencia = 5;
	produtos = 1;
	ouro = 1;
};
int Planicie::id_count;

void Planicie::evolui(int turno) {
	if (!evoluiu && turno >= 6) {
		produtos++;
		evoluiu = true;
		return;
	}
	else
		return;
}