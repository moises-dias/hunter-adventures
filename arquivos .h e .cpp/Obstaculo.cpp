#include "Obstaculo.h"

Obstaculo::Obstaculo()
{
    ID=64;
    permiteColisaoChao = true;
    permiteColisaoEnt = true;

    funcionando=false;

    estado = PARADO;
    jogadores[0] = NULL;
    jogadores[1] = NULL;
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
}
Obstaculo::~Obstaculo()
{

}
void Obstaculo::setJogador(Jogador* j, int i)
{
    jogadores[i] = j;
}
void Obstaculo::setMapaAtual(Mapa* m)
{
    mapaAtual = m;
}
