#ifndef GOTA_LAVA_H
#define GOTA_LAVA_H
#include "Projetil.h"

class Gota_Lava : public Projetil
{
    public:
        Gota_Lava();
        Gota_Lava(Vetor_R2 posI);
        ~Gota_Lava();

        void desenha();
        void atualiza();
        void criaFrames();

        void setEst(int e);

        void resetaEntidade();

        void paraTimer();

    private:


};

#endif // GOTA_LAVA_H
