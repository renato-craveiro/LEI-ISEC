#include "Imperio.h"

void Imperio::gastaOuro() {
	if (ouro > 0)
		ouro--;
};

void Imperio::gastaProduto() {
	if (produtos > 0)
		produtos--;
};


void Imperio::adicionaOuro() {
	if(ouro<max_ouro)
		ouro++;
};
void Imperio::adicionaProdutos() {
	if(produtos<max_prod)
		produtos++;
};
void Imperio::adicionaPontos() {
	pontos++;
};
void Imperio::adicionaMilitares() {
	if(militares<max_militares)
		militares++;
};


void Imperio::alianca() {
	adicionaMilitares();
}

void Imperio::evRecOuro() {
	adicionaOuro();
}
void Imperio::evRecProd() {
	adicionaProdutos();
}

bool Imperio::Invasao(int ataque) {
	if (tecn->defTerr) {
		if (ataque > territoriosImperio.back()->getResistencia()+1) {
			territoriosImperio.back()->perde();
			territoriosImperio.pop_back();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (ataque > territoriosImperio.back()->getResistencia()) {
			territoriosImperio.back()->perde();
			territoriosImperio.pop_back();
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

Imperio::Imperio(Territorio_Inicial  * terrInic, Tecnologias* tec) {
	territoriosImperio.push_back((terrInic));
	tecn = tec;
	ouro = 0; 
	produtos = 0;
	pontos = 0;
	militares = 0;
	max_ouro = 3;
	max_prod = 3;
	max_militares = 3;
	ultimaSorte = 0;
};

string Imperio::ConquistaTerritorio(Territorio* t) {
	vector<Territorio*>::iterator it;

	for (it = territoriosImperio.begin(); it != territoriosImperio.end(); it++) {
		if ((* it)->getNome() == t->getNome()) {
			return "Território " + (*it)->getNome() + "já foi conquistado.";
		}
	}

	if (militares>0) {
		ultimaSorte = rand() % 6 + 1;
		if (t->conquista(militares + ultimaSorte)) {
			territoriosImperio.push_back((t));
			adicionaPontos();
			return(t->getNome() + " foi conquistado.  (sorte: " + to_string(ultimaSorte) + ")\n");
		}
		else {
			militares--;
			return (t->getNome() + " nao foi conquistado. Perdeu uma unidade militar. (sorte: "+to_string(ultimaSorte)+")\n");
		}
	}
	else {
		return (t->getNome() + " nao pode ser conquistado por nao ter nenhum militar.\n");
	}
};
void Imperio::RemoveTerritorio(Territorio t) {

};
void Imperio::recolheProdutos() {
	vector<Territorio *>::iterator it;

	for (it = territoriosImperio.begin(); it != territoriosImperio.end(); it++) {
		if(produtos<max_prod)
			produtos += (*it)->getProdutos();
	}
};
void Imperio::recolheOuro() {
	vector<Territorio *>::iterator it; 

	for (it = territoriosImperio.begin(); it != territoriosImperio.end(); it++) {
		if(ouro<max_ouro)
			ouro += (*it)->getOuro();
	}
};

void Imperio::maismilitar() {
	if (ouro < 1) {
		cout << "Ouro insuficiente!" << endl;
		return;
	}
	if (produtos < 1) {
		cout << "Produtos insuficiente!" << endl;
		return;
	}
	if (militares >= max_militares) {
		cout << "Numero maximo de militares atingido" << endl;
		return;
	}
	ouro--;
	produtos--;
	adicionaMilitares();
	return;
};

void Imperio::maisouro() {
	if (produtos < 2) {
		cout << "\nProdutos insuficientes!" << endl;
		return;
	}
	if (getMaxOuro()==ouro) {
		cout << "\nNu'mero ma'ximo de ouro atingido!" << endl;
		return;
	}

	produtos = produtos - 2;
	adicionaOuro();
}

void Imperio::maisprod() {
	if (ouro < 2) {
		cout << "\nOuro insuficiente!" << endl;
		return;
	}
	if (getMaxProdutos() == ouro) {
		cout << "\nNu'mero ma'ximo de produtos atingido!" << endl;
		return;
	}

	ouro = ouro - 2;
	adicionaProdutos();
}


int Imperio::getNumTerr() {
	return territoriosImperio.size();
}

int Imperio::getPontos() {
	return pontos;
}

int Imperio::getMilitares() {
	return militares;
}
int Imperio::getMaxMilitares() {
	return max_militares;
}

int Imperio::getOuro() {
	return ouro;
}
int Imperio::getMaxOuro() {
	return max_ouro;
}

int Imperio::getProdutos() {
	return produtos;
}
int Imperio::getMaxProdutos() {
	return max_prod;
}
string Imperio::getImperio() {
	string s;
	s.append("--Imperio--\n\n");
	s.append("Forc,a Militar (Militares Disponiveis): "+ to_string(militares)+" (maximo: "+to_string(max_militares)+")\n");
	s.append("Cofre (Ouro Disponivel): "+to_string(ouro)+" (maximo: "+to_string(max_ouro)+")\n");
	s.append("Armaze'm (Produtos Disponiveis): "+to_string(produtos)+" (maximo: "+to_string(max_prod)+")\n");
	s.append("Ultimo fator sorte: +" + to_string(ultimaSorte) +"\n");
	s.append("Tecnologias adquiridas:\n" + tecn->getTecnologias()+"\n");
	s.append("Territorios:\n");

	vector<Territorio *>::iterator it;

	for (it = territoriosImperio.begin(); it != territoriosImperio.end(); it++) {
		s.append("\t"+(*it)->getTerritorio()+"\n\n");
	}
	s.append("Pontos: " + to_string(pontos)+"\n");

	return(s);
};

int Imperio::nTerr() {
	return territoriosImperio.size();
}

Territorio* Imperio::getTerrInic(){
	return territoriosImperio.front();
}


Tecnologias* Imperio::getTecnologias() {
	return tecn;
}