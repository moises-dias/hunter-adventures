#include "Slime.h"

#ifndef BESOURO_H
#define BESOURO_H


class Besouro : public Inimigo
{
public:
    Besouro();
    Besouro(int ix,int iy);
    ~Besouro();

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

private:
    Jogador* alvo;
    float vXMax;
    float vRand;
    float velXDano, velYDano;
    float delX,delY,denominador;
    bool atacou;

};

#endif // BESOURO_H
