#ifndef FANTASMA_H
#define FANTASMA_H
#include "Armadilha_Urso.h"

class Fantasma : public Obstaculo
{
public:
    Fantasma();
    Fantasma(int ix,int iy);
    ~Fantasma();

    void desenha();
    void atualiza();

    void construiFrames();
    void atualizaComportamento();
    void atualizaEstados();

    void resetaEntidade();
    void setFuncionando(bool f);
    void paraTimer();

private:
    float aceleracao;
    float posEquilib;
};

#endif // FANTASMA_H
