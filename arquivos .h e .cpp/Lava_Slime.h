#include "Necromante.h"

#ifndef LAVA_SLIME_H
#define LAVA_SLIME_H

class Lista_Entidades;
class Gota_Lava;

class Lava_Slime : public Inimigo
{
public:
    Lava_Slime();
    Lava_Slime(int ix,int iy);
    ~Lava_Slime();

    void mudaEstado(int nEstado);
    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();

    void inicializaEntidade();
    void setFuncionando(bool f);
    void resetaEntidade();

    void paraTimer();

private:
    Jogador* alvo;
    float aceleracao;
    float vXMax;
    float vRand;
    float vYPulo;
    float vYPuloMedio;
    Vetor_R2 velPulo;
    bool atirar;
    int tempoProjetil;
    float distPulo;

    float yPlat;
    float delXQueda;

    float velX,delX,velY,delYSubir,delYDescer,tempo;

    Gota_Lava* gotas[3];



};

#endif // LAVA_SLIME_H
