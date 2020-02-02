#ifndef INIMIGOS_FASE_1_H
#define INIMIGOS_FASE_1_H
#include "Vetor_R2.h"
#include "Lista_Generica.h"

enum INIMIGOS_FASE_1{SLIME_1, BESOURO_1, GOBLIN_1, MORCEGO_1, ESQUELETO_1};

class Inimigos_Fase_1
{
    public:
        Inimigos_Fase_1();
        ~Inimigos_Fase_1();

        Lista_Generica<Vetor_R2>* getPosIni();
        Lista_Generica<int>* getTipoIni();

        Lista_Generica<Vetor_R2>* getPosArmadilhas();
    private:
        Lista_Generica<Vetor_R2> *posIni;
        Lista_Generica<int> *tipoIni;

        Lista_Generica<Vetor_R2> * posArmadilhas;

};

#endif // INIMIGOS_FASE_1_H
