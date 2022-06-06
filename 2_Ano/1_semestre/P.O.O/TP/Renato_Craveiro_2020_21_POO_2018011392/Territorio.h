#pragma once

#include <string>
#include<iostream>

using namespace std;

class Territorio
{
protected:
	string Nome;
	int resistencia;
	int produtos;
	int ouro;
	int pontos;
	bool conquistado;
	Territorio() {}; 
	
public:
	Territorio(string n, int r, int pr, int ou, int pts) : Nome(n), resistencia(r), produtos(pr), ouro(ou), pontos(pts), conquistado(false) {};
	virtual string const getTerritorio() ;

	string const getNome() { return Nome; };
	int const getOuro(){ return ouro; };
	int const getPontos() { return pontos; };
	int const getProdutos() { return produtos; };
	int const getResistencia() { return resistencia; };
	bool const getConquistado() { return conquistado; };


	void perde();
	bool conquista(int ataque);
	virtual void evolui(int turno) {};
};

