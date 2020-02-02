#include "Obstaculo.h"
#ifndef INIMIGO_H
#define INIMIGO_H

class Inimigo : public Entidade
{
public:
    Inimigo();
    ~Inimigo();
    void setJogador(Jogador* j, int i);
    void setMapaAtual(Mapa* m);

    virtual void desenha()=0;
    virtual void atualiza()=0;
    virtual void criaProjetil()=0;
    virtual void construiFrames()=0;
    virtual void atualizaComportamento()=0;
    virtual void atualizaColisoes()=0;
    virtual void atualizaEstados()=0;
    virtual void criaEfeitos()=0;

    virtual void inicializaEntidade()=0;
    virtual void resetaEntidade()=0;

protected:
    Jogador* jogadores[2];
    Mapa* mapaAtual;
    int estado;
    int score;
    int danoGolpe,danoGolpeRand;
};

#endif // INIMIGO_H
