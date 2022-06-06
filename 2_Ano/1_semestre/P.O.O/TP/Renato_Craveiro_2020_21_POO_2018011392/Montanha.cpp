#include "Montanha.h"

Montanha::Montanha() {
	id = id_count++;
	Nome = "Montanha" + to_string(id);
	evoluiu = false;
	
	resistencia = 6;
	produtos = 0;
	ouro = 0;
};
int Montanha::id_count;
void Montanha::evolui(int turno) {
	if (!evoluiu && turno >= 2) {
		produtos++;
		evoluiu = true;
		return;
	}
	else
		return;
}