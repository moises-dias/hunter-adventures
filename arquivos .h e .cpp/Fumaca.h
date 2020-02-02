#ifndef FUMACA_H
#define FUMACA_H
#include "Efeito.h"


class Fumaca : public Efeito
{
    public:
        Fumaca();
        ~Fumaca();
        void desenha();
        void atualiza();
        void criaFrames();

        void setAreaEfetiva(Vetor_R2 a);
        void setFuncionando(bool f);
    private:
        int numFumaca;
        int numX;
        int numY;
        Vetor_R2 areaEfetiva;


};

#endif // FUMACA_H
