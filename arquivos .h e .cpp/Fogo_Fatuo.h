#ifndef FOGO_FATUO_H
#define FOGO_FATUO_H
#include "Goblin.h"

class Fogo_Fatuo : public Inimigo
{
public:
    Fogo_Fatuo();
    Fogo_Fatuo(int ix,int iy);
    Fogo_Fatuo(int ix,int iy, bool t);
    ~Fogo_Fatuo();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();
    void setFuncionando(bool f);
    void inicializaEntidade();
    void resetaEntidade();

    void paraTimer();

private:
    Jogador* alvo;
    float vXMax;
    float vRand;
    float velXDano, velYDano;
    float delX,delY,denominador;
    bool tipo;
    int tempoInvoca;
    Fogo_Fatuo* fogos[4];
    ALLEGRO_COLOR corNormal;
    ALLEGRO_COLOR corDano;
};

#endif // FOGO_FATUO_H
