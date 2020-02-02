#ifndef GOLEM_MALIGNO_H
#define GOLEM_MALIGNO_H
#include "Chefao.h"

class Golem_Maligno : public Inimigo
{
 public:
    Golem_Maligno();
    Golem_Maligno(int ix,int iy);
    ~Golem_Maligno();

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

#endif // GOLEM_MALIGNO_H
