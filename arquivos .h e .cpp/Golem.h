#ifndef GOLEM_H
#define GOLEM_H
#include "Fogo_Fatuo.h"

class Golem : public Inimigo
{
public:
    Golem();
    Golem(int ix,int iy);
    ~Golem();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();

    void inicializaEntidade();
    void resetaEntidade();

    void paraTimer();

private:
    Jogador* alvo;
    bool perto;
    float vXMax;
    float vRand;
    int pisadaFrame;
    Ataque_Generico* atkGolem[3];
};

#endif // GOLEM_H
