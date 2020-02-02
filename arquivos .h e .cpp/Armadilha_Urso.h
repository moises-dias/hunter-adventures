#ifndef ARMADILHA_URSO_H
#define ARMADILHA_URSO_H
#include "Obstaculo.h"

class Armadilha_Urso : public Obstaculo
{
public:
    Armadilha_Urso();
    Armadilha_Urso(int ix,int iy);
    ~Armadilha_Urso();

    void desenha();
    void atualiza();

    void construiFrames();
    void atualizaComportamento();
    void atualizaEstados();

    void resetaEntidade();

    void paraTimer();

private:
    int cor;
    bool pisado;

};

#endif // ARMADILHA_URSO_H
