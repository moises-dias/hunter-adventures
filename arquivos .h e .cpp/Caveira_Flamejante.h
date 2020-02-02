#ifndef CAVEIRA_FLAMEJANTE_H
#define CAVEIRA_FLAMEJANTE_H
#include "Projetil.h"

class Caveira_Flamejante : public Projetil
{
public:
    Caveira_Flamejante();
    Caveira_Flamejante(Vetor_R2 posI);
    ~Caveira_Flamejante();
    void desenha();
    void atualiza();
    void criaFrames();
    void resetaEntidade();
    void setDir(float d);


private:
    int fade;
};

#endif // CAVEIRA_FLAMEJANTE_H
