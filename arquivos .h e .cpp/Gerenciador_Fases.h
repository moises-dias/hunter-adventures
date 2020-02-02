#ifndef GERENCIADOR_FASES_H
#define GERENCIADOR_FASES_H
#include "Fase_3.h"

class Gerenciador_Fases
{
    public:
        Gerenciador_Fases();
        ~Gerenciador_Fases();
        void fase1();

        static void criaFase(Jogo* j, Lista_Entidades * listEnt,  bool doisJog, Controle * control);
        static void atualizaFase(int f, int c);
    private:
        int numFases;
        static Fase_1 *f1;
        static Fase_2 *f2;
        static Fase_3 *f3;
};

#endif // GERENCIADOR_FASES_H
