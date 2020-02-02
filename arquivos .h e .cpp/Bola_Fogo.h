#ifndef BOLA_FOGO_H
#define BOLA_FOGO_H
#include "Projetil.h"

class Bola_Fogo : public Projetil
{
public:
    Bola_Fogo();
    Bola_Fogo(Vetor_R2 posI);
    ~Bola_Fogo();

    void desenha();
    void atualiza();
    void criaFrames();

    void setFuncionando(bool f);
    void resetaEntidade();

    void paraTimer();

private:

};

#endif // BOLA_FOGO_H
