#include "Inimigos_Fase_2.h"

Inimigos_Fase_2::Inimigos_Fase_2()
{
    posIni = new Lista_Generica<Vetor_R2> [4];
    tipoIni = new Lista_Generica<int> [6];
    /**Inicializando posição de Slimes**/
    //Parte 1
    for (int i = 0; i < 5; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1360 + (1150/5)*i,680));
        tipoIni[0].inclui_frente(SLIME_2);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(1430,470));
    tipoIni[0].inclui_frente(SLIME_2);
    posIni[0].inclui_frente(Vetor_R2::vetor(3000,345));
    tipoIni[0].inclui_frente(SLIME_2);
    /**Inicializando posição de Slimes**/

    /**Inicializando posição de Goblins**/
    //Parte 1
    for (int i = 0; i < 5; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1360 + (850/3)*i,680));
        tipoIni[0].inclui_frente(GOBLIN_2);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(2900,345));
    tipoIni[0].inclui_frente(GOBLIN_2);

    //Parte 2
    for (int i = 0; i < 10; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(4800 + (335)*i,600));
        tipoIni[1].inclui_frente(GOBLIN_2);
    }

    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor(12360,560));
    tipoIni[2].inclui_frente(GOBLIN_2);
    /**Inicializando posição de Goblins**/

    /**Inicializando posição de Morcegos**/
    //Parte 1
    for (int i = 0; i < 2; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1400 + (500)*i + rand()%100 - 50,20));
        tipoIni[0].inclui_frente(MORCEGO_2);
    }
    for (int i = 0; i < 2; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(3400 + (600/3)*i + rand()%100 - 50,20));
        tipoIni[0].inclui_frente(MORCEGO_2);
    }

    //Parte 2
    for (int i = 0; i < 10; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(4600 + ((8000 - 4600)/10)*i + rand()%100 - 50,20));
        tipoIni[1].inclui_frente(MORCEGO_2);
    }

    //Parte 3

    posIni[2].inclui_frente(Vetor_R2::vetor(13000 ,20));
    tipoIni[2].inclui_frente(MORCEGO_2);
    for (int i = 0; i < 3; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(14500 + (300)*i + rand()%100 - 50 ,20));
        tipoIni[2].inclui_frente(MORCEGO_2);
    }

    //Parte 4
    for (int i = 0; i < 4; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor(17400 + (2200/4)*i + rand()%100 - 50 ,20));
        tipoIni[3].inclui_frente(MORCEGO_2);
    }

    /**Inicializando posição de Morcegos**/

    /**Inicializando posição de Slime Lava**/
    //Parte 2
    for (int i = 0; i < 3; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(6500 + (1300/3)*i , 550));
        tipoIni[1].inclui_frente(SLIME_LAVA_2);
    }
    //Parte 3

        posIni[2].inclui_frente(Vetor_R2::vetor(12400 , 550));
        tipoIni[2].inclui_frente(SLIME_LAVA_2);
    for (int i = 0; i < 7; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(14750 + (1100/7)*i , 550));
        tipoIni[2].inclui_frente(SLIME_LAVA_2);
    }

    //Parte 4
    posIni[3].inclui_frente(Vetor_R2::vetor(15700 , 330));
    tipoIni[3].inclui_frente(SLIME_LAVA_2);

    for (int i = 0; i < 9; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor(17240 + ((19600-17240)/8)*i , 570));
        tipoIni[3].inclui_frente(SLIME_LAVA_2);
    }
    /**Inicializando posição de Slime Lava**/

    /**Inicializando posição de Esqueleto**/
    //Parte 3
    for (int i = 0; i < 3; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(15200 + 20*i , 550));
        tipoIni[2].inclui_frente(ESQUELETO_2);
    }
    /**Inicializando posição de Esqueleto**/

    /**Inicializando posição de Fogo Fatuo**/
    //Parte 2
    for (int i = 0; i < 4; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(5000 + (3000/4)*i , 100));
        tipoIni[1].inclui_frente(FOGO_FATUO_2);
    }
    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor(12500 , 100));
    tipoIni[2].inclui_frente(FOGO_FATUO_2);

    //Parte 4
    posIni[3].inclui_frente(Vetor_R2::vetor(14800 , 100));
    tipoIni[3].inclui_frente(FOGO_FATUO_2);
    for (int i = 0; i < 3; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor(17600 + (2400/3)*i , 100));
        tipoIni[3].inclui_frente(FOGO_FATUO_2);
    }
    /**Inicializando posição de Fogo Fatuo**/

    /**Inicializando posição de Golem**/
    //Parte 1
    posIni[0].inclui_frente(Vetor_R2::vetor(3250 , 345));
    tipoIni[0].inclui_frente(GOLEM_2);

    //Parte 2
    for (int i = 0; i < 3; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(5100 + (900)*i , 570));
        tipoIni[1].inclui_frente(GOLEM_2);
    }

    //parte 3
    if (rand()%2)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(12400 , 560));
        tipoIni[2].inclui_frente(GOLEM_2);
    }
    posIni[2].inclui_frente(Vetor_R2::vetor(15800 , 320));
    tipoIni[2].inclui_frente(GOLEM_2);

    //Parte 4
    for (int i = 0; i < 3; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor(17700 + 600*i, 570));
        tipoIni[3].inclui_frente(GOLEM_2);
    }
    /**Inicializando posição de Golem**/

    /**Ponto de Parada 1**/

    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(ESQUELETO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(FOGO_FATUO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(ESQUELETO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOLEM_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(ESQUELETO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(FOGO_FATUO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(ESQUELETO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOLEM_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(ESQUELETO_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    tipoIni[4].inclui_frente(GOLEM_2);
    tipoIni[4].inclui_frente(MORCEGO_2);
    tipoIni[4].inclui_frente(GOBLIN_2);
    /**Ponto de Parada 1**/

    /**Ponto de Parada 2**/
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(FOGO_FATUO_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(FOGO_FATUO_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(FOGO_FATUO_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(FOGO_FATUO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(SLIME_LAVA_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    tipoIni[5].inclui_frente(GOBLIN_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(GOLEM_2);
    tipoIni[5].inclui_frente(MORCEGO_2);
    tipoIni[5].inclui_frente(ESQUELETO_2);
    /**Ponto de Parada 2**/

    posLava = new Lista_Generica<Vetor_R2>[1];
    posLava[0].inclui_frente(Vetor_R2::vetor((16764 - 15918)/2 + 15918, 376));
    posLava[0].inclui_frente(Vetor_R2::vetor((14006 - 12662)/2 + 12662, 604));
    posLava[0].inclui_frente(Vetor_R2::vetor((19448 - 17498)/2 + 17498, 694));

}

Inimigos_Fase_2::~Inimigos_Fase_2()
{

}
Lista_Generica<Vetor_R2>* Inimigos_Fase_2::getPosIni()
{
    return posIni;
}
Lista_Generica<int>* Inimigos_Fase_2::getTipoIni()
{
    return tipoIni;
}
Lista_Generica<Vetor_R2> * Inimigos_Fase_2::getPosLava()
{
    return posLava;
}
