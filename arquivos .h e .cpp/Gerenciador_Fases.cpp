#include "Gerenciador_Fases.h"

Fase_1 *Gerenciador_Fases::f1 = NULL;
Fase_2 *Gerenciador_Fases::f2 = NULL;
Fase_3 *Gerenciador_Fases::f3 = NULL;

Gerenciador_Fases::Gerenciador_Fases()
{
}

Gerenciador_Fases::~Gerenciador_Fases()
{
}

void Gerenciador_Fases::criaFase(Jogo* j, Lista_Entidades * listEnt, bool doisJog, Controle * control)
{




        Gerenciador_Fases::f1 = new Fase_1(&listEnt[0], doisJog, control);
        listEnt[0].setFase(static_cast<Fase*> (Gerenciador_Fases::f1));
        Gerenciador_Fases::f1->setJogo(j);


        Gerenciador_Fases::f2 = new Fase_2(&listEnt[1], doisJog, control);
        listEnt[1].setFase(static_cast<Fase*> (Gerenciador_Fases::f2));
        Gerenciador_Fases::f2->setJogo(j);


        Gerenciador_Fases::f3 = new Fase_3(&listEnt[2], doisJog, control);
        listEnt[2].setFase(static_cast<Fase*> (Gerenciador_Fases::f3));
        Gerenciador_Fases::f3->setJogo(j);

}

void Gerenciador_Fases::atualizaFase(int f, int c)
{
    if (f == 0)
        f1->atualizaCheckpoint(c);
    else if (f == 1)
        f2->atualizaCheckpoint(c);
    else if (f == 2)
        f3->atualizaCheckpoint(c);

}

