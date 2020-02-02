#ifndef EFEITO_DANO_H
#define EFEITO_DANO_H
#include "Efeito.h"

class Efeito_Dano : public Efeito
{
    public:
        Efeito_Dano();
        Efeito_Dano(Vetor_R2 es, int d);
        ~Efeito_Dano();

        void atualiza();
        void desenha();

        void setDano(int d);

    private:
        float alpha;
};

#endif // EFEITO_DANO_H
