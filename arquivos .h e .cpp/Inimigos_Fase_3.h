#ifndef INIMIGOS_FASE_3_H
#define INIMIGOS_FASE_3_H
#include "Lista_Generica.h"
#include "Vetor_R2.h"

enum INIMIGOS_FASE_3{ SLIME_LAVA_3, ESQUELETO_3, FOGO_FATUO_3, GOLEM_3, GOLEM_MALIGNO_3, NECROMANTE_3};

class Inimigos_Fase_3
{
    public:
        Inimigos_Fase_3();
        ~Inimigos_Fase_3();

        Lista_Generica<Vetor_R2>* getPosIni();
        Lista_Generica<int>* getTipoIni();

        Lista_Generica<Vetor_R2> *getPosFan();
    private:
        Lista_Generica<Vetor_R2> *posIni;
        Lista_Generica<int> *tipoIni;

        Lista_Generica<Vetor_R2> * posFan;
};

#endif // INIMIGOS_FASE_3_H
