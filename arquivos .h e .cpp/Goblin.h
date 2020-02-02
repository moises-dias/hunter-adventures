#ifndef GOBLIN_H
#define GOBLIN_H
#include "Lava_Slime.h"

class Goblin : public Inimigo
{
public:
    Goblin();
    Goblin(int ix,int iy);
    ~Goblin();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();

    void gerenciaAudios();

    void inicializaEntidade();
    void resetaEntidade();

    void paraTimer();

private:
    Jogador* alvo;
    bool perto;
    float vXMax;
    float vRand;
    float yPlat;
    float delXPulo;
    Ataque_Generico* atkGoblin[2];
};

#endif // GOBLIN_H
