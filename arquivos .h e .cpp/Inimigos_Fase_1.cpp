#include "Inimigos_Fase_1.h"

Inimigos_Fase_1::Inimigos_Fase_1()
{
    posIni = new Lista_Generica<Vetor_R2> [4];
    tipoIni = new Lista_Generica<int> [6];

    /**Inicializando posição de Slimes**/
    //Parte 1
    for (int i = 0; i < 5; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1360 + 420*i,660));
        tipoIni[0].inclui_frente(SLIME_1);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(1760,490));
    tipoIni[0].inclui_frente(SLIME_1);
    posIni[0].inclui_frente(Vetor_R2::vetor(3150,495));
    tipoIni[0].inclui_frente(SLIME_1);
    //Parte 2
    for (int i = 0; i < 8; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(3600 + ((6300- 3600)/8)*i,370));
        tipoIni[1].inclui_frente(SLIME_1);
    }
    for (int i = 0; i < 5; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(6600 + 80*i,600));
        tipoIni[1].inclui_frente(SLIME_1);
    }
    //Parte 3
    for (int i = 0; i < 5; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(10000 + 280*i,260));
        tipoIni[2].inclui_frente(SLIME_1);
    }

    //Parte 4
    for (int i = 0; i < 7; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor(12300 + 314*i,700));
        tipoIni[3].inclui_frente(SLIME_1);
    }
    posIni[3].inclui_frente(Vetor_R2::vetor(19400,700));
    tipoIni[3].inclui_frente(SLIME_1);
    /**Inicializando posição de SLIME_1s**/

    /**Inicializando posição de Besouro**/
    //Parte 1
    for ( int i = 0; i < 5; i ++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor( 1500+ ((3300- 1500)/7)*i,20 + rand()%200));
        tipoIni[0].inclui_frente(BESOURO_1);
    }

    //Parte 2
    for ( int i = 0; i < 8; i ++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor( 3200 + (4000/8)*i ,20 + rand()%150));
        tipoIni[1].inclui_frente(BESOURO_1);
    }

    //Parte 3
    for (int i = 0; i < 4; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor( 8000 + rand()%1000 ,10 + rand()%300));
        tipoIni[2].inclui_frente(BESOURO_1);
    }
    posIni[2].inclui_frente(Vetor_R2::vetor( 9700 ,20));
        tipoIni[2].inclui_frente(BESOURO_1);
    for (int i = 0; i < 5; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor( 9300 + ((11500-9300)/5)*i, 10 + rand()%100));
        tipoIni[2].inclui_frente(BESOURO_1);
    }

    //Parte 4

    for (int i = 0; i < 4; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor( 12000 + (2600/4)*i,10 + rand()%200));
        tipoIni[3].inclui_frente(BESOURO_1);
    }

    /**Inicializando posição de Besouro**/

    /**Inicializando posição de Goblin**/
    //Parte 1
    for (int i = 0; i < 3; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor( 1380 + (2800/3)*i,660));
        tipoIni[0].inclui_frente(GOBLIN_1);
    }

    //Parte 2
    for (int i = 0; i < 3; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor( 3300 + 1300*i,370));
        tipoIni[1].inclui_frente(GOBLIN_1);
    }
    //Parte 3
    for (int i = 0; i < 2; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor( 10200 + 1300*i,260));
        tipoIni[2].inclui_frente(GOBLIN_1);
    }
    //Parte 4
    for (int i = 0; i < 2; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor( 12500 + 2500/5*i,640));
        tipoIni[3].inclui_frente(GOBLIN_1);
    }
    posIni[3].inclui_frente(Vetor_R2::vetor( 13500,460));
    tipoIni[3].inclui_frente(GOBLIN_1);
    if (rand()%2)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor( 16200,470));
        tipoIni[3].inclui_frente(GOBLIN_1);
    }


    /**Inicializando posição de Goblin**/

    /**Inicializando posição de Morcego**/
    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor( 11000,10 + rand()%100));
    tipoIni[2].inclui_frente(MORCEGO_1);

    //Parte 4
    for ( int i = 0; i < 3; i++)
    {
        posIni[3].inclui_frente(Vetor_R2::vetor( 12500 + ((15000-12500)/3)*i,10 + rand()%200));
        tipoIni[3].inclui_frente(MORCEGO_1);
    }

    posIni[3].inclui_frente(Vetor_R2::vetor( 16300 ,10 + rand()%100));
    tipoIni[3].inclui_frente(MORCEGO_1);
    /**Inicializando posição de Morcego**/

    /**Inicializando posição de Esqueleto**/
    //parte 4
    posIni[3].inclui_frente(Vetor_R2::vetor( 13140, 640));
    tipoIni[3].inclui_frente(ESQUELETO_1);
    posIni[3].inclui_frente(Vetor_R2::vetor( 13280, 470));
    tipoIni[3].inclui_frente(ESQUELETO_1);
    posIni[3].inclui_frente(Vetor_R2::vetor( 16000, 480));
    tipoIni[3].inclui_frente(ESQUELETO_1);
    /**Inicializando posição de Esqueleto**/

    /**Ponto de parada 1**/
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(SLIME_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(BESOURO_1);
    tipoIni[4].inclui_frente(GOBLIN_1);
    /**Ponto de parada 1**/

    /**Ponto de parada 2**/
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(BESOURO_1);
    tipoIni[5].inclui_frente(SLIME_1);
    tipoIni[5].inclui_frente(GOBLIN_1);
    tipoIni[5].inclui_frente(MORCEGO_1);
    tipoIni[5].inclui_frente(ESQUELETO_1);
    /**Ponto de parada 2**/

    posArmadilhas = new Lista_Generica<Vetor_R2>[1];
    posArmadilhas[0].inclui_frente(Vetor_R2::vetor(600,600));
    for (int i = 0; i < 5; i++)
    {
        posArmadilhas[0].inclui_frente(Vetor_R2::vetor(1200 + i*(3334 -1000)/5,674));
    }
    for (int i = 0; i < 6; i ++)
    {
        if (rand()%100 < 90)
            posArmadilhas[0].inclui_frente(Vetor_R2::vetor(3634 + i*(6440 - 3334)/6,400));
    }
    for (int i = 0; i < 3; i ++)
    {
        posArmadilhas[0].inclui_frente(Vetor_R2::vetor(10004 + i*(11482 - 9804)/3,400));
    }
    for (int i = 0; i < 4; i ++)
    {
        posArmadilhas[0].inclui_frente(Vetor_R2::vetor(12196 + i*(14902 - 11996)/4,704));
    }

}

Inimigos_Fase_1::~Inimigos_Fase_1()
{

}
Lista_Generica<Vetor_R2>* Inimigos_Fase_1::getPosIni()
{
    return posIni;
}
Lista_Generica<int>* Inimigos_Fase_1::getTipoIni()
{
    return tipoIni;
}
Lista_Generica<Vetor_R2>* Inimigos_Fase_1::getPosArmadilhas()
{
    return posArmadilhas;
}
