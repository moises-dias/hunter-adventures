#ifndef SLIME_H
#define SLIME_H
#include "Esqueleto.h"


class Slime : public Inimigo
{
public:
    Slime();
    Slime(int ix,int iy);
    ~Slime();

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
    void resetaEntidade();
    void setFuncionando(bool f);

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

    float yPlat;
    float delXQueda;
};

#endif // SLIME_H
