#ifndef INIMIGOS_FASE_2_H
#define INIMIGOS_FASE_2_H
#include "Lista_Generica.h"
#include "Vetor_R2.h"

enum INIMIGOS_FASE_2{SLIME_2, GOBLIN_2, MORCEGO_2, SLIME_LAVA_2, ESQUELETO_2, FOGO_FATUO_2, GOLEM_2};

class Inimigos_Fase_2
{
    public:
        Inimigos_Fase_2();
        ~Inimigos_Fase_2();

        Lista_Generica<Vetor_R2>* getPosIni();
        Lista_Generica<int>* getTipoIni();

        Lista_Generica<Vetor_R2> * getPosLava();
    private:
        Lista_Generica<Vetor_R2> *posIni;
        Lista_Generica<int> *tipoIni;

        Lista_Generica<Vetor_R2> * posLava;
};

#endif // INIMIGOS_FASE_2_H
