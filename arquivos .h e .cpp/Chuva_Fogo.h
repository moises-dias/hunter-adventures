#ifndef CHUVA_FOGO_H
#define CHUVA_FOGO_H
#include "Projetil.h"

class Chuva_Fogo : public Projetil
{
public:
    Chuva_Fogo();
    Chuva_Fogo(Vetor_R2 posI);
    ~Chuva_Fogo();

    void desenha();
    void atualiza();
    void criaFrames();


    void resetaEntidade();
    void setFuncionando(bool f);

private:

};

#endif // CHUVA_FOGO_H
