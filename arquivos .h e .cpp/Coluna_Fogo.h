#ifndef COLUNA_FOGO_H
#define COLUNA_FOGO_H
#include "Projetil.h"

class Coluna_Fogo : public Projetil
{
public:
    Coluna_Fogo();
    Coluna_Fogo(Vetor_R2 posI);
    ~Coluna_Fogo();

    void desenha();
    void atualiza();
    void criaFrames();

    void resetaEntidade();

    void paraTimer();

private:

};

#endif // COLUNA_FOGO_H
