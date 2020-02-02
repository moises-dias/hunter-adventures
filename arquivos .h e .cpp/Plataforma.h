#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include "Vetor_R2.h"


class Plataforma
{
    public:
        Plataforma();
        ~Plataforma();

        void setTipo( int t);
        int getTipo() ;
        Vetor_R2& getPos(int i);
    private:
        int tipo;
        Vetor_R2 posicao[2];
};

#endif // PLATAFORMA_H
