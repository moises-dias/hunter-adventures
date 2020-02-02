#ifndef OSSO_AEREO_H
#define OSSO_AEREO_H
#include "Projetil.h"

class Osso_Aereo : public Projetil
{
    public:
        Osso_Aereo();
        Osso_Aereo(Vetor_R2 posI, Vetor_R2 velI, float aceleracao);
        ~Osso_Aereo();

        void desenha();
        void atualiza();
        void criaFrames();

        void setFuncionando(bool f);

        void resetaEntidade();

    private:
        float coefAng;
};

#endif // OSSO_AEREO_H
