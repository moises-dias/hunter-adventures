#include "Inimigo.h"

Inimigo::Inimigo()
{
    ID=4;
    permiteColisaoChao = true;
    permiteColisaoEnt = true;

    funcionando=false;

    estado = PARADO;
    jogadores[0] = NULL;
    jogadores[1] = NULL;
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
    danoGolpe=0;
    danoGolpeRand=0;

}
Inimigo::~Inimigo()
{

}
void Inimigo::setJogador(Jogador* j, int i)
{
    jogadores[i] = j;
}
void Inimigo::setMapaAtual(Mapa* m)
{
    mapaAtual = m;
}
