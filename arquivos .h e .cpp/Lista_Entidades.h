#ifndef LISTA_ENTIDADES_H
#define LISTA_ENTIDADES_H
#include "Morcego.h"

class Fase;

class Lista_Entidades
{
private:
    Lista_Generica<Entidade*> listaE;
    Fase * ptrFase;
    Mapa* ptrMapa;
    int score;
    int scoreMudanca;
    int velScore;
    int scoreSalvo;

    ALLEGRO_TIMER* timerL;
public:
    Lista_Entidades();
    ~Lista_Entidades();
    void incluiEntidade(Entidade* e);
    void deletaListaEntidade();
    void colisoesEnt();
    void colisoesChao();
    bool verificaColisao(Entidade * ent1, Entidade * ent2);
    void atualiza();
    void desenha();
    void mataJogador(int i);
    void resetaLista();
    void resetaJogador(Jogador* pJ, int i);
    void inicializaTimer();
    void salvaScore();
    void resetaScore();
    void setScore(int s);
    int getScore();
    int getScoreSave();

    void mudaScore(int i);
    void paraMusica();
    void pausaEntidades();
    void despausaEntidades();

    Lista_Generica<Entidade*>* getListGen();
    void setFase(Fase* f);
    void setMapa(Mapa* pM);


};

#endif // LISTA_ENTIDADES_H
