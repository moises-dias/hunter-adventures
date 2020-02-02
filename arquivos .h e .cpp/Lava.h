#ifndef LAVA_H
#define LAVA_H
#include "Armadilha_Urso.h"

class Bola_Fogo;


class Lava : public Obstaculo
{
public:
    Lava();
    Lava(int ix,int iy);
    ~Lava();

    void desenha();
    void atualiza();

    void construiFrames();
    void atualizaComportamento();
    void atualizaEstados();

    void resetaEntidade();
    void setFuncionando(bool f);
    void inicializaEntidade();

    void paraTimer();
private:
    Bola_Fogo* bolas[5];
    int tempoInvocar;
};

#endif // LAVA_H
