#include "Inimigos_Fase_3.h"

Inimigos_Fase_3::Inimigos_Fase_3()
{
    posIni = new Lista_Generica<Vetor_R2> [3];
    tipoIni = new Lista_Generica<int> [4];


    /**Inicializando posição de Lava Slime**/
    for (int i = 0; i < 3; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1600 + (500/3)*i , 710));
        tipoIni[0].inclui_frente(SLIME_LAVA_3);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(2550 , 420));
    tipoIni[0].inclui_frente(SLIME_LAVA_3);
    /**Inicializando posição de Lava Slime**/

    /**Inicializando posição de Fogo Fatuo**/
    for (int i = 0; i < 3; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1620 + (460/3)*i , 100));
        tipoIni[0].inclui_frente(FOGO_FATUO_3);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(2650 , 100));
    tipoIni[0].inclui_frente(FOGO_FATUO_3);
    for (int i = 0; i < 4; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(4500 + (1100/4)*i , 100));
        tipoIni[0].inclui_frente(FOGO_FATUO_3);
    }
    //Parte 2
    for (int i = 0; i < 5; i++)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(5800 + (2100/5)*i , 100));
        tipoIni[1].inclui_frente(FOGO_FATUO_3);
    }
    posIni[1].inclui_frente(Vetor_R2::vetor(10800 , 100));
    tipoIni[1].inclui_frente(FOGO_FATUO_3);

    //Parte 3
    for (int i = 0; i < 3; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(15500 + 200*i , 100));
        tipoIni[2].inclui_frente(FOGO_FATUO_3);
    }
    /**Inicializando posição de Fogo Fatuo**/

    /**Inicializando posição de Esqueleto**/
    //Parte 1
    for (int i = 0; i < 2; i++)
    {
        posIni[0].inclui_frente(Vetor_R2::vetor(1560 + 400*i , 710));
        tipoIni[0].inclui_frente(ESQUELETO_3);
    }
    posIni[0].inclui_frente(Vetor_R2::vetor(3110 , 580));
    tipoIni[0].inclui_frente(ESQUELETO_3);
    posIni[0].inclui_frente(Vetor_R2::vetor(5300 , 490));
    tipoIni[0].inclui_frente(ESQUELETO_3);
    posIni[0].inclui_frente(Vetor_R2::vetor(5330 , 490));
    tipoIni[0].inclui_frente(ESQUELETO_3);

    //Parte 2
    posIni[1].inclui_frente(Vetor_R2::vetor(10400 , 560));
    tipoIni[1].inclui_frente(ESQUELETO_3);

    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor(14500 , 620));
    tipoIni[2].inclui_frente(ESQUELETO_3);
    posIni[2].inclui_frente(Vetor_R2::vetor(15930 , 620));
    tipoIni[2].inclui_frente(ESQUELETO_3);
    /**Inicializando posição de Esqueleto**/

    /**Inicializando posição de Golem**/
    //Parte 1
    posIni[0].inclui_frente(Vetor_R2::vetor(1750 , 710));
    tipoIni[0].inclui_frente(GOLEM_3);
    posIni[0].inclui_frente(Vetor_R2::vetor(5000 , 600));
    tipoIni[0].inclui_frente(GOLEM_3);

    //Parte 2
    posIni[1].inclui_frente(Vetor_R2::vetor(10700 , 560));
    tipoIni[1].inclui_frente(GOLEM_3);

    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor(15200 , 520));
    tipoIni[2].inclui_frente(GOLEM_3);
    /**Inicializando posição de Golem**/

    /**Inicializando posição de Golem Maligno**/
    posIni[0].inclui_frente(Vetor_R2::vetor(2800 , 420));
    tipoIni[0].inclui_frente(GOLEM_MALIGNO_3);

    //Parte 2
    posIni[1].inclui_frente(Vetor_R2::vetor(7420 , 590));
    tipoIni[1].inclui_frente(GOLEM_MALIGNO_3);
    posIni[1].inclui_frente(Vetor_R2::vetor(11000 , 560));
    tipoIni[1].inclui_frente(GOLEM_MALIGNO_3);

    //Parte 3
    posIni[2].inclui_frente(Vetor_R2::vetor(15400 , 520));
    tipoIni[2].inclui_frente(GOLEM_MALIGNO_3);
    posIni[2].inclui_frente(Vetor_R2::vetor(15650 , 520));
    tipoIni[2].inclui_frente(GOLEM_MALIGNO_3);
    /**Inicializando posição de Golem Maligno**/

    /**Inicializando posição de Necromante**/
    //Parte 1
    posIni[0].inclui_frente(Vetor_R2::vetor(2780 , 100));
    tipoIni[0].inclui_frente(NECROMANTE_3);
    posIni[0].inclui_frente(Vetor_R2::vetor(4950 , 100));
    tipoIni[0].inclui_frente(NECROMANTE_3);

    //Parte 2
    if (rand()%2)
    {
        posIni[1].inclui_frente(Vetor_R2::vetor(10600 , 100));
        tipoIni[1].inclui_frente(NECROMANTE_3);
    }


    //Parte 3
    for (int i = 0; i < 2; i++)
    {
        posIni[2].inclui_frente(Vetor_R2::vetor(15000 + 600*i , 100));
        tipoIni[2].inclui_frente(NECROMANTE_3);
    }

    /**Inicializando posição de Necromante**/


    /**Ponto de Parada 1**/
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(NECROMANTE_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(NECROMANTE_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(NECROMANTE_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(NECROMANTE_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(FOGO_FATUO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(ESQUELETO_3);
    tipoIni[3].inclui_frente(GOLEM_3);
    tipoIni[3].inclui_frente(GOLEM_MALIGNO_3);


    posFan = new Lista_Generica<Vetor_R2>[1];
    posFan[0].inclui_frente(Vetor_R2::vetor(7976 + (8272-7976)/2,480));
    posFan[0].inclui_frente(Vetor_R2::vetor(8386 + (8754-8386)/2,480));
    posFan[0].inclui_frente(Vetor_R2::vetor(8868 + (9216-8868)/2,480));



}

Inimigos_Fase_3::~Inimigos_Fase_3()
{

}
Lista_Generica<Vetor_R2>* Inimigos_Fase_3::getPosIni()
{
    return posIni;
}
Lista_Generica<int>* Inimigos_Fase_3::getTipoIni()
{
    return tipoIni;
}
Lista_Generica<Vetor_R2> *Inimigos_Fase_3::getPosFan()
{
    return posFan;
}
