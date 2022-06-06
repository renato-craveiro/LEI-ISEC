#include "Interpretador.h"


void Interpretador::limpaEcra()
{
    int i;
    for (i = 0; i < 5; i++)
       cout<<("\n\n\n\n\n\n\n\n\n\n")<<endl;
}

bool Interpretador::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Interpretador::limpaFases(Imperio *myImperio) {
    for (int i = 0; i < 4; i++)
        myImperio->fase[i] = 0;
}

void Interpretador::addTer(Mundo* world) {
    world->adicionaTerritorio(new Castelo());
}

void Interpretador::cria(Mundo* world, int n, Territorio* t) {
    if (Castelo* c = dynamic_cast<Castelo*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Castelo());
    }
    else if (Fortaleza* f = dynamic_cast<Fortaleza*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Fortaleza());
    }
    else if (Duna* d = dynamic_cast<Duna*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Duna());
    }
    else if (Mina* m = dynamic_cast<Mina*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Mina());
    }
    else if (Montanha* mo = dynamic_cast<Montanha*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Montanha());
    }
    else if (Planicie* p = dynamic_cast<Planicie*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Planicie());
    }
    else if (RefugioPiratas* rp = dynamic_cast<RefugioPiratas*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new RefugioPiratas());
    }
    else if (Pescaria* pe = dynamic_cast<Pescaria*>(t)) {
        world->adicionaTerritorio(t);
        for (int i = 1; i < n; i++)
            world->adicionaTerritorio(new Pescaria());
    }
    else {
        cout << "Tipo de territorio desconhecido." << endl;
    }
}

void Interpretador::recolha(/*Mundo* world,*/ Imperio* myImperio) {
    //world->novoTurno();
    myImperio->recolheOuro();
    myImperio->recolheProdutos();
}

bool Interpretador::lefich(Mundo* world, Imperio* myImperio, string nomeFicheiro, vector<savegame>* saves) {
    ifstream file;
    //istringstream iss;
    file.open(nomeFicheiro);
    if (!file.is_open()) {
        cout << "Erro ao abrir o ficheiro " << nomeFicheiro << "." << endl;
        return false;
    }
    string linha;
    while (file.good()) {
        //iss.clear();
        getline(file, linha);
        if (!interpretadorComandos(world, myImperio, linha,saves)) {
            cout << "Comando \"" << linha << "\" nao reconhecido. \nAvanco para o proximo." << endl;
        }
    }
    file.close();
    return true;
}

bool Interpretador::Recolher(Imperio* myImperio) {
    if (myImperio->fase[1] == 1 && !myImperio->recolheu) {
        cout << "\n\n[Fase de recolha de produtos/ouro]\n\n"<<endl;
        recolha(myImperio);
        myImperio->recolheu = true;
        /*limpaFases();
        fase[2] = 1;*/
        cout << "\n\n[Recolha conclui'ida]\n\n" << endl;
        return true;
    }
    else {
        return false;
    }
}


void Interpretador::evRecAband(Mundo* world, Imperio* myImperio) {
    if (world->getTurno() < 6) {
        myImperio->evRecProd();
        cout << "\n\n[EVENTO: O que e' isto? Produtos?! Acho que acabou de ser nosso...]\n" << endl;
    }
    else {
        myImperio->evRecOuro();
        cout << "\n\n[EVENTO: O que e' isto? Ouro?! Acho que acabou de ser nosso...]\n" << endl;
    }
}

void Interpretador::evInvasao(Mundo* world, Imperio* myImperio) {
    int sorte;
    int forca;
    if (world->getTurno() < 6) {
        forca = 2;
    }
    else {
        forca = 3;
    }
    sorte = rand() % 6 + 1;
    if (myImperio->Invasao(sorte + forca)) {
        cout << "\n\n[EVENTO: Foi atacado por um Impe'erio Ba'rbaro e desrespeituoso perante si. Perdeu a sua u'ltima conquista...]\n" << endl;
    }
    else {
        cout << "\n\n[EVENTO: Foi atacado por um Impe'erio Ba'rbaro e desrespeituoso perante si, No entanto a nossa uniao foi mais forte. O seu Impe'rio esta' intacto!]\n" << endl;
    }

}

void Interpretador::evAlianca(Imperio* myImperio) {
    myImperio->alianca();
    cout << "\n\n[EVENTO: Criou uma alianc,a com um Impe'erio completamente aleato'rio e deconhecido a si. Parabe'ns!]\n" << endl;
}

bool Interpretador::Evento(Mundo* world, Imperio* myImperio) {
    if (myImperio->fase[3] == 1) {
        int eventn;
        //CALL EVENTS
        eventn=rand() % 4 + 1;

        switch (eventn) {
        case 1:
            evRecAband(world, myImperio);
            break;
        case 2:
            evInvasao(world,myImperio);
            break;
        case 3:
            evAlianca(myImperio);
            break;
        case 4:
            cout << "\n\n[NAO OCORREU NENHUM EVENTO NESTA RONDA]\n" << endl;
            break;
        }
        limpaFases(myImperio);
        myImperio->recolheu = false;
        world->novoTurno();
        myImperio->fase[0] = 1;
        cout << "\n\n[Fase de Eventos terminada]\n\n[Fase de Conquista comec,a]\n" << endl;
        return true;
    }
    else {
        return false;
    }
}

void Interpretador::listaSaves(vector<savegame>* saves) {
    vector<savegame>::iterator it;
    cout<<"\n\n[Lista de Saves]\n"<<endl;
    for (it = saves->begin(); it != saves->end(); it++) {
        cout << it->getSaveName() << endl;
    }
    return;
}


bool Interpretador::procuraSave(string nome, vector<savegame>* saves) {
    vector<savegame>::iterator it;
    cout << "\n\n[Lista de Saves]\n" << endl;
    for (it = saves->begin(); it != saves->end(); it++) {
        if (nome.compare(it->getSaveName())==0)
            return true;
    }
    return false;
}

bool Interpretador::eliminaSave(string nome, vector<savegame>* saves) {
    vector<savegame>::iterator it;
    int i = 0;
    it = saves->begin();
    while (it != saves->end()) {
        if (nome.compare(it->getSaveName()) == 0) {
            /*it = */saves->erase(saves->begin()+i);
            cout << "\n\n[SAVEFILE ELIMINADO]\n" << endl;
            return true;
        }
        else {
            it++;
            i++;
        }
    }
    cout << "\n\n[ERRO AO ELIMINAR SAVEFILE]\n" << endl;
    return false;
}

void Interpretador::loadGame(string nome, vector<savegame>* saves,Imperio *myImperio, Mundo* world) {
    vector<savegame>::iterator it;
    for (it = saves->begin(); it != saves->end(); it++) {
        if (nome.compare(it->getSaveName()) == 0) {
            it->loadgame(myImperio, world, myImperio->getTerrInic(), myImperio->getTecnologias());
            cout << "\n\n\n-----[SAVE CARREGADO: " + it->getSaveName() + "]-----\n\n" << endl;

            return;
        }
    }
    return;
}




bool Interpretador::interpretadorComandos(Mundo* world, Imperio* myImperio, string comando, vector<savegame>* saves) {
    istringstream ss(comando);
    string aux;
    string arr[4];
    int i = 0;
    //getline(ss, aux, ' ');
    //transform(aux.begin(), aux.end(), aux.begin(), ::toupper);


    while (ss.good() && i < 3) {
        ss >> arr[i];
        if (i == 0) {
            transform(arr[i].begin(), arr[i].end(), arr[i].begin(), ::toupper);
        }
        ++i;
    }

    if (arr[0] == "LIMPAR") {
        limpaEcra();
        return true;
    }
    if (arr[0] == "INTRO") {
        cout << "\t\t  oooooooo8 o88              ooooooooooo                          o8\n"
            "\t\to888     88 oooo oooo   oooo 88  888  88 ooooooooo8 oooo   oooo o888oo ooooooo\n"
            "\t\t888          888  888   888      888    888oooooo8    888o888    888 888     888\n"
            "\t\t888o     oo  888   888 888       888    888           o88 88o    888 888     888\n"
            "\t\t 888oooo88  o888o    888        o888o     88oooo888 o88o   o88o   888o 88ooo88\n" << endl;

        cout << "    /| \n   |||" << endl;
        cout << "   |||\t\tEste e' um jogo inspirado em jogos como Civilization e Age of empires em que\n"
            << "   |||\t\to seu objetivo e' conquistar todos os territo'rios do mundo.\n"
            << "   |||\n   |||\n"
            "   |||\t\t\t(Escreva 'ajuda' para mostrar os comandos possiveis de executar)\n   |||\n   |||\t\t\t\t\t\tDivirta-se!" << endl;
        cout << "~-[{o}]-~\t[Projeto Desenvolvido no ambito da disciplina de Programac,ao Orientada a Objetos]"
            << " \n   |/|\t\t\t[Renato Craveiro (2018011392 - P7 - ISEC) - Ano Letivo 2020/21]\n   |/|\n   `0'" << endl;

        cout << "========================================================================================================================\n\n\n" << endl;
        return true;
    }
    if (arr[0] == "CREDITOS") {
        cout << "[Projeto Desenvolvido no ambito da disciplina de Programação Orientada a Objetos"
            << " por Renato Craveiro (2018011392 - P7 - ISEC) - Ano Letivo 2020/21]\n\n" << endl;
        return true;
    }
    if (arr[0] == "AJUDA") {
        cout << "\nComandos:\n\n\n\tcarrega  <nomeFicheiro> - Executa  um  conjunto  de  comandos  de  configuracao existentes em nomeFicheiro, um por cada linha.\n"
            << "\n\tcria <tipo> <n> - Acrescenta ao mundo n territo'rios de um determinado tipo (exemplo: mina, refugio, para respetivamente um territo'rio do tipo Mina ou do tipo Refugio)."
            << "\n\tconquista <nome> - Da' a ordem ao impe'rio para adquirir umdeterminado territo'rio neste  turno  desde  que  este  esteja  disponi'vel  no  Mundo.  O  parametro  nome  indica qual o nome do territo'rio a conquistar (planicie1, duna3, etc.).\n"
            
            <<"\n\tpassa – Neste turno nao se pretende conquistar nenhum territo'rio"
            <<"\n\tmaisouro  – obte'm mais 1 de ouro, perdendo 2 de produtos, se nao tiver 2 de produtos esta troca nao e' possi'vel."
            <<"\n\tmaisprod – obte'm  mais  1  de  produtos,  perdendo  2  de  ouro,  se  nao  tiver  2  de  ouro esta troca nao e' possi'vel"
            
            << "\n\tmaismilitar - compra uma unidade militar: reduz uma unidade de produtos e uma de ouro para obter uma unidade de forc,a militar adicional, desde que a forc,a militar nao ultrapasse o valor ma'ximo possi'vel.\n"
            <<"\n\tadquire <tipo> -Da' a ordem ao impe'rio para adquirir uma determinada tecnologia. O parametro tipo indica qual a tecnologia a comprar (drone, missil, etc.). Todas as regras anteriormente descritas sao aqui seguidas: se e' possi'vel adquirir tal objeto o seu custo e'   descontado   dos   recursos   existentes   do   impe'rio.   Esta   nova   aquisic,ao sera' efetivamente aplicada ao impe'rio na fase correspondente do turno."
            << "\n\tlista <nome> - Obte'm a  informac,ao do jogo, tanto globalmente como apenas de um territo'rio  especi'fico caso o seu nome seja indicado como parametro.\n"
            
            << "\n\tavanca  - Termina afase de recolha de comandos e desencadeia as ac,oes necessa'rias a cada fase.\n"
            <<"\n\tgrava <nome> -Grava o estado do jogo em memo'ria, associando-lhe um nome. Esta ac,ao  consiste  em  fazer  uma  espe'cie  de savegame para  memo'ria,  possibilitando  ao jogador manter em memo'ria diversos snapshotsdo jogo, correspondentes a diversos momentos, permitindo-lhe a qualquer momento recuperar um desses momentos. O jogo  continua ativo,  mas  a  co'pia  feita  para  memo'ria ja'  nao  sera' afetada pelos comandos entretanto escritos a partir deste momento"
            << "\n\tativa <nome> -Recupera um dado estado do jogo em memo'ria, identificadoao nome indicado,  e  carrega-o. O  jogo  recuperado  passa  a  ser  o  que  esta'  em  efeito:  os comandos passam a agir sobre este."
            << "\n\tapaga <nome> -Apaga da memo'ria um dado estado do jogo previamente gravado e associado ao nome especificado."
            
            
            << "\n\ttoma    <qual>  <nome> -Toma  de  assalto  um  determinado  territo'rio  ou  tecnologia desde  que  esteja  disponi'vel.  O  parametro  qual  pode  ser  terr –território  ou  tec -tecnologia.  O  parametro  nomeindica  o  nome  do  territo'rio  ou  da  tecnologia.  Este comando  serve  apenas  para  DEBUG,  nao  sendo  seguida qualquer  regra  para a sua aquisic,ao ou mesmo aplicado o custo associado. Este comando tem efeito imediato, nao sendo necessário esperar pela fase correspondente."
            << "\n\tmodifica <ouro|prod> N -Modifica os dados do impe'rio: quantidade de ouro no cofre ou quantidade de unidades de produtos a ter nos armaze'ns. Este comando tambe'm e' essencialmente para DEBUG (para efeitos de testes e demonstrac,ao)"
            << "\n\tfevento <nome-evento> -forc,a a ocorrencia de um evento indicado pelo seu nome, tal  como  descrito  na  lista  indicada  atra's.  Este  comando  serve  para  DEBUG  e teste/avaliac,ao"
            

            << "\n\texit - Forc,a o encerramento do programa\n"
            << "\n\tcreditos - mostra os creditos do programa\n"
            << "\n\tintro - mostra novamente a introduc,ao do jogo\n"
            
            << "\n\tlimpar - limpa o ecran\n"
            << "\n\tlista pontos - mostra os pontos atuais\n"
            << "\n\t"


            << "\n\tajuda - mostra os comandos do jogo\n\n"
            << endl;
        return true;
    }
    if (arr[0] == "LISTA") {
        if (arr[1] == "") {
            std::cout << world->getMundo() << "\n\n" << std::endl;
            cout << myImperio->getImperio() << endl;
            return true;
        }
        if (!(world->getTerritorioNome(arr[1]) == NULL)) {
            cout << world->getTerritorioNome(arr[1])->getTerritorio() << endl;
            return true;
        }
        transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
        if (arr[1] == "SAVES") {
            listaSaves(saves);
            return true;
        }
        if (arr[1] == "MUNDO") {
            cout << world->getMundo() << "\n\n" << endl;
            return true;
        }
        if (arr[1] == "IMPERIO") {
            cout << myImperio->getImperio() << "\n\n" << endl;
            return true;
        }
        if (arr[1] == "TURNO") {
            cout << "Turno: " << to_string(world->getTurno()) << "\n\n" << endl;
            return true;
        }
        if (arr[1] == "ANO") {
            if (world->getTurno() < 6) {
                cout << "Ano: 1" << endl;
            }
            else {
                cout << "Ano: 2" << endl;
            }
            return true;
        }


        if (arr[1] == "CONQUISTADOS") {
            cout << world->getConquistados() << "\n\n" << endl;
            return true;
        }
        if (arr[1] == "NAOCONQUISTADOS") {
            cout << world->getNaoConquistados() << "\n\n" << endl;
            return true;
        }


        if (arr[1] == "PRODUTOS") {
            cout << "Produtos: " << to_string(myImperio->getProdutos()) << " (maximo: " << to_string(myImperio->getMaxProdutos()) << ")\n\n" << endl;
            return true;
        }
        if (arr[1] == "MILITAR") {
            cout << "Militares: " << to_string(myImperio->getMilitares()) << " (maximo: " << to_string(myImperio->getMaxMilitares()) << ")\n\n" << endl;
            return true;
        }
        if (arr[1] == "OURO") {
            cout << "Ouro: " << to_string(myImperio->getOuro()) << " (maximo: " << to_string(myImperio->getMaxOuro()) << ")\n\n" << endl;
            return true;
        }
        if (arr[1] == "TECNOLOGIAS") {
            cout << myImperio->tecn->getTecnologias() << "\n\n" << endl;
            return true;
        }
        /*if (arr[1] == "EVENTOS") {
            cout << myImperio->getImperio() << "\n\n" << endl;
            return true;
        }*/
        if (arr[1] == "PONTOS") {
            cout << "PONTUACAO: " << endl;
            int pontFinal;
            string s;
            s.append("[PONTUACAO OBTIDA NA CONQUISTA DE TERRITORIOS] +" + to_string(myImperio->getPontos()) + "\n");
            pontFinal = myImperio->getPontos();
            if (world->getNumNaoConquistados() == 0) {
                pontFinal += 3;
                s.append("[BONUS: IMPERADOR SUPREMO - conquistou todo o mundo] +3\n");
            }
            /*PONTOS POR TECNOLOGIA
            PONTOS POR OBTER 5 TECNOLOGIAS*/
            if(myImperio->tecn->temDrones() && myImperio->tecn->temMisseis() && myImperio->tecn->temDefTerr() && 
                myImperio->tecn->temBolVal() && myImperio->tecn->temBanCent()){ 
                pontFinal += 5;
            }

            s.append("\nPONTUACAO FINAL: " + to_string(pontFinal) + "\n\n");
            cout << s << endl;
            return true;
        }
        if (arr[1] == "FASE") {
            if (myImperio->fase[0] == 1) {
                cout << "\n\n[FASE DE CONQUISTA]\n" << endl;
                return true;
            }
            if (myImperio->fase[1] == 1) {
                cout << "\n\n[FASE DE RECOLHA]\n" << endl;
                return true;
            }
            if (myImperio->fase[2] == 1) {
                cout << "\n\n[FASE DE COMPRA MILITAR/TECNOLO'GICA]\n" << endl;
                return true;
            }
            if (myImperio->fase[3] == 1) {
                cout << "\n\n[FASE DE EVENTOS]\n" << endl;
                return true;
            }
            if (!myImperio->MundoCriado) {
                cout << "\n\n[FASE DE CRIAC,AO DE MUNDO]\n" << endl;
                return true;
            }
        }
    }
    if (arr[0] == "EXIT") {
        exit(0);
    }

    if (arr[0] == "GRAVA") {
        if (arr[1] == "") {
            cout << "\n\n[ERRO: O savegame tem que ter um nome (grava <nome_do_savegame>)]\n" << endl;
            return false;
        }
        if (procuraSave(arr[1], saves)) {
            cout << "\n\n[ERRO: Savefile com o nome \"" + arr[1] + "\"ja' existe.]" << endl;
            return false;
        }
        savegame newSave(myImperio, world, myImperio->getTerrInic() , myImperio->getTecnologias(), arr[1]);
        saves->push_back(newSave);
        return true;
    }

    if (arr[0] == "ATIVA") {
        if (arr[1] == "") {
            cout << "\n\n[ERRO: O savegame tem que ter um nome (ativa <nome_do_savegame>)]\n" << endl;
            return false;
        }
        if (!procuraSave(arr[1],saves)) {
            cout << "\n\n[ERRO: Nome do savefile nao existente]\n" << endl;
            return false;
        }
        loadGame(arr[1], saves, myImperio,world);
        return true;
        
    }

    if (arr[0] == "APAGA") {
        if (arr[1] == "") {
            cout << "\n\n[ERRO: O savegame tem que ter um nome (apaga <nome_do_savegame>)]\n" << endl;
            return false;
        }
        if (!procuraSave(arr[1], saves)) {
            cout << "\n\n[ERRO: Nome do savefile nao existente]\n" << endl;
            return false;
        }
        eliminaSave(arr[1], saves);
        return true;

    }

    if (arr[0] == "MODIFICA") {
        if (arr[1] == "") {
            cout << "\n\n[ERRO: indique o tipo de valor a alterar (ouro ou prod) (sintaxe 'modifica <ouro|prod> <n>' em que n indica a quantidade)]\n" << endl;
            return false;
        }
        if (arr[2] == "") {
            cout << "\n\n[ERRO: indique a quantidade a alterar (sintaxe 'modifica <ouro|prod> <n>' em que n indica a quantidade)]\n" << endl;
            return false;
        }

        int n;

        transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
        n = stoi(arr[2]);
        if (debug.modifica(myImperio, arr[1], n))
            return true;
    }

    if (arr[0] == "FEVENTO") {
        if (arr[1] == "") {
            cout << "\n\n[ERRO: indique o tipo de evento a acontecer]\n" << endl;
            return false;
        }
        transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
        if (arr[1] == "RECURSO") {
            evRecAband(world, myImperio);
            return true;
        }
        if (arr[1] == "INVASAO") {
            evInvasao(world, myImperio);
            return true;
        }
        if (arr[1] == "ALIANCA") {
            evAlianca(myImperio);
            return true;
        }
        cout << "\n\n[ERRO: Tipo de evento nao reconhecido (recurso/invasao/alianca)]\n" << endl;
    }

    if (arr[0] == "TOMA") {

        if (arr[1] == "" || arr[2]=="") {
            cout << "\n\n[Sintaxe: toma terr <nomeTerritorio> | toma terr <nomeTecnologia>]\n" << endl;
            return false;
        }
        transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
        return debug.toma(myImperio, world, arr[1], arr[2]);

    }

    if (!myImperio->MundoCriado) {

        if (arr[0] == "CRIA") {
            //cout << arr[0] << endl;
            int quantd;
            transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
            //cout << arr[1] << endl;

            if (arr[1] == "CASTELO" && !(arr[2] == "") && is_number(arr[2])) {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Castelo());
                return true;
            }
            else if (arr[1] == "FORTALEZA" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Fortaleza());
                return true;
            }
            else if (arr[1] == "DUNA" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Duna());
                return true;
            }
            else if (arr[1] == "MINA" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Mina());
                return true;
            }
            else if (arr[1] == "MONTANHA" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Montanha());
                return true;
            }
            else if (arr[1] == "PLANICIE" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Planicie());
                return true;
            }
            else if (arr[1] == "REFUGIO" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new RefugioPiratas());
                return true;
            }
            else if (arr[1] == "PESCARIA" && !(arr[2] == "") && is_number(arr[2]))
            {
                quantd = stoi(arr[2]);
                cria(world, quantd, new Pescaria());
                return true;
            }
            else {
                cout << "Utilizacao do comando cria: cria <tipo de territorio> <numero de territorios>" << endl;
                return false;
            }
        }

        if (arr[0] == "CARREGA") {
            if (lefich(world, myImperio, arr[1], saves)) {
                cout << "Sucesso na leitura dos comandos." << endl;
                return true;
            }
            else
                return false;
        }
    
        if (arr[0] == "AVANCA" || arr[0] == "AVANÇA") {
            char resposta;
            cout << "\nTem a certeza que deseja terminar a fase de criac,ao do mundo?[y/n]"<<endl;
            while (1) {
                resposta = (char)_getwche();
                if (resposta == 'y' || resposta == 'Y') {
                    cout << "\n\n--FASE DE CRIACAO CONCLUI'DA--\n\n";
                    myImperio->MundoCriado = true;
                    limpaFases(myImperio);
                    myImperio->fase[0] = 1;
                    cout << "\n\n[Fase de Criac,ao de mundo terminada]\n\n[Fase de Conquista comec,a]\n" << endl;
                    return true;
                }
                if (resposta == 'n' || resposta == 'N') {
                    cout << "\n\n[Comando cancelado]\n\n";
                    return true;
                }
                cout << "\n\n[y/Y = sim  ||  n/N = nao]\n\n" << endl;
            }
        }
        return false;
    }    
    else {

        if (arr[0] == "FCRIACAO") {
            cout << "\n\n[DEBUG: FASE CRIACAO DE MUNDO ATIVADA]\n\n" << endl;
            myImperio->MundoCriado = false;
            return true;
        }
        if (myImperio->fase[0] == 1) {
            if (arr[0] == "CONQUISTA" && !(arr[1] == "")) {
                if (!(world->getTerritorioNome(arr[1]) == NULL)) {
                    if (Ilha* i = dynamic_cast<Ilha*>(world->getTerritorioNome(arr[1]))) {
                        if (myImperio->getNumTerr() > 5 && myImperio->tecn->temMisseis()) {
                            cout << myImperio->ConquistaTerritorio(world->getTerritorioNome(arr[1])) << endl;
                            limpaFases(myImperio);
                            myImperio->fase[1] = 1;
                            return true;
                        }
                        if(myImperio->tecn->temMisseis())
                            cout << "Nao tem a tecnologia Mi'sseis Teleguiados." << endl;
                        else
                            cout << "Nao tem territorios suficientes para conquistar uma ilha." << endl;
                        return false;
                    }
                    cout << myImperio->ConquistaTerritorio(world->getTerritorioNome(arr[1])) << endl;
                    myImperio->fase[1] = 1;
                    limpaFases(myImperio);
                    return true;
                }
            }
            if (arr[0] == "PASSA" || arr[0] == "AVANCA"|| arr[0] == "AVANÇA") {
                cout << "\n\n[Nao foi efetuada nenhuma tentativa de conquista neste turno]\n\n" << endl;
                limpaFases(myImperio);
                myImperio->fase[1] = 1;
                cout << "\n\n[Fase de Conquista terminada.]\n\n[Fase de Recolha comec,a]\n" << endl;
                return true;
            }
        }
        if (myImperio->fase[1]){
            //COMANDOS PARA TROCA
            if (arr[0] == "MAISOURO") {
                myImperio->maisouro();
                return true;
            }

            if (arr[0] == "MAISPROD") {
                myImperio->maisprod();
                return true;
            }
            if (arr[0] == "AVANCA" || arr[0] == "AVANÇA") {
                    myImperio->comprouMilitar = false;
                    myImperio->comprouTecn = false;
                    limpaFases(myImperio);
                    myImperio->fase[2] = 1;
                    cout << "\n\n[Fase de Recolha terminada]\n\n[Fase de Compra Tecnolo'gica/Militar comec,a]\n" << endl;
                    return true;
            }


        }

        if (myImperio->fase[2]) {
            if (arr[0] == "MAISMILITAR") {
                if (!myImperio->comprouMilitar) {
                    myImperio->maismilitar();
                    myImperio->comprouMilitar = true;
                }else{
                    cout << "\n\n<<[Ja comprou uma unidade militar neste turno]\n\n" << endl;
                }
                return true;
            }

            if (arr[0] == "ADQUIRE") {
                if (!myImperio->comprouTecn) {
                    transform(arr[1].begin(), arr[1].end(), arr[1].begin(), ::toupper);
                    //cout << arr[1] << endl;

                    if (arr[1] == "DRONE") {
                        if (myImperio->tecn->compraDrones(myImperio)) {
                            return true;
                            myImperio->comprouTecn = true;
                        }
                        else
                            return false;
                    }
                    if (arr[1] == "MISSIL") {
                        if (myImperio->tecn->compraMisseis(myImperio)) {
                            return true;
                            myImperio->comprouTecn = true;
                        }
                        else
                            return false;
                    }
                    if (arr[1] == "DEFESA") {
                        if (myImperio->tecn->defTerritoriais(myImperio)) {
                            return true;
                            myImperio->comprouTecn = true;
                        }
                        else
                            return false;
                    }
                    if (arr[1] == "BOLSA") {
                        if (myImperio->tecn->bolsaValores(myImperio)) {
                            return true;
                            myImperio->comprouTecn = true;
                        }
                        else
                            return false;
                    }
                    if (arr[1] == "BANCO") {
                        if (myImperio->tecn->BancoCentral(myImperio)) {
                            return true;
                            myImperio->comprouTecn = true;
                        }
                        else
                            return false;
                    }
                    return false;
                }
                else {
                    cout << "\n[Ja' comprou uma tecnologia neste turno]\n"<<endl;
                    return false;
                }
                
            }

            if (arr[0] == "AVANCA" || arr[0] == "AVANÇA") {
                char resposta;
                if (!myImperio->comprouMilitar && !myImperio->comprouTecn) {
                    cout << "Nao comprou nenhum militar nem tecnologia, deseja mesmo avanc,ar para a proxima fase?[y/n]" << endl;
                    while (1) {
                        resposta = (char)_getwche();
                        if (resposta == 'y' || resposta == 'Y') {
                            cout << "\n\n--FASE DE COMPRA CONCLUI'DA--\n\n";
                            limpaFases(myImperio);
                            myImperio->fase[3] = 1;
                            return true;
                        }
                        if (resposta == 'n' || resposta == 'N') {
                            cout << "\n\n[Comando cancelado]\n\n";
                            return true;
                        }
                    cout << "\n\n[y/Y = sim  ||  n/N = nao]\n\n" << endl;
                    }
                }
                else{
                    cout << "\n\n--FASE DE COMPRA CONCLUI'DA--\n\n";
                    myImperio->comprouMilitar = false;
                    myImperio->comprouTecn = false;
                    limpaFases(myImperio);
                    myImperio->fase[3] = 1;
                    cout << "\n\n[Fase de Compra Tecnolo'gica/Militar terminada]\n\n[Fase de Eventos comec,a]\n" << endl;
                    return true;
                }
            }
        }

       /* if (arr[0] == "AVANCA" || arr[0] == "AVANÇA") {
            avanca(world, myImperio);
            return true;
        }*/

        
       

    }
    
    return false;
}

