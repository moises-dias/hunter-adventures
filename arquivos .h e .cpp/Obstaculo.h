#ifndef OBSTACULO_H
#define OBSTACULO_H
#include "Projetil.h"


class Obstaculo : public Entidade
{
public:
    Obstaculo();
    ~Obstaculo();
    void setJogador(Jogador* j, int i);
    void setMapaAtual(Mapa* m);

    virtual void desenha()=0;
    virtual void atualiza()=0;

    virtual void construiFrames()=0;
    virtual void atualizaComportamento()=0;
    virtual void atualizaEstados()=0;

    virtual void resetaEntidade()=0;

protected:
    Jogador* jogadores[2];
    Mapa* mapaAtual;
    int estado;



};

#endif // OBSTACULO_H
