// Renato_Craveiro_2020_21_POO_2018011392.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Interpretador.h"

int main()
{
    vector<savegame> saves;
    Mundo world;
    Interpretador LinhaComandos;
    Territorio_Inicial tI;
    Tecnologias tec;
    world.adicionaTerritorio(&tI);
    Imperio myImperio(&tI, &tec);


    LinhaComandos.interpretadorComandos(&world, &myImperio, "INTRO", &saves);
    while (world.getTurno()<12 && myImperio.nTerr()>0) {
        LinhaComandos.Recolher(&myImperio);
        LinhaComandos.Evento(&world, &myImperio);
        cout << "\n<Comando> ";
        string uInput;
        getline(cin, uInput);
        if (!LinhaComandos.interpretadorComandos(&world, &myImperio, uInput, &saves)) {
            cout << "\n!!!! <Comando introduzido nao foi executado com sucesso.> !!!!\n" << endl;
        }
    }
    LinhaComandos.interpretadorComandos(&world, &myImperio, "LIMPAR", &saves);
    cout << "---FIM DE JOGO---\n\nRESULTADOS: \n" << endl;
    cout << myImperio.getImperio() << endl;
    LinhaComandos.interpretadorComandos(&world, &myImperio, "PONTOS", &saves);
    LinhaComandos.interpretadorComandos(&world, &myImperio, "CREDITOS", &saves);
    cin.get();
}

