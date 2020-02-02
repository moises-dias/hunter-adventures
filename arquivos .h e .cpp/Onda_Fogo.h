#ifndef ONDA_FOGO_H
#define ONDA_FOGO_H
#include "Projetil.h"

class Onda_Fogo : public Projetil
{
public:
    Onda_Fogo();
    Onda_Fogo(Vetor_R2 posI);
    ~Onda_Fogo();

    void desenha();
    void atualiza();
    void criaFrames();
    void setCoefAce(float c);


    void resetaEntidade();

private:
    float coefAce;
    bool iniciarSom;
};

#endif // ONDA_FOGO_H
