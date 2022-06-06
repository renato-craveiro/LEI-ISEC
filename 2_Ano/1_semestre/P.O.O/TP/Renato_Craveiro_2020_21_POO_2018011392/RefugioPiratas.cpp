#include "RefugioPiratas.h"
RefugioPiratas::RefugioPiratas() {
	id = id_count++;
	Nome = "Refugio" + to_string(id);

	evoluiu = false;

	resistencia = 9;
	produtos = 0;
	ouro = 1;
};

int RefugioPiratas::id_count;

void RefugioPiratas::evolui(int turno) {
	return;
}