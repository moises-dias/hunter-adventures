#include "Projetil.h"

Projetil::Projetil()
{

}
Projetil::Projetil(int sX, int sY, char* nomeImg, int numEstados):
    Entidade(sX, sY, nomeImg, numEstados)
{
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
}
Projetil::~Projetil()
{

}
void Projetil::setPtrJ(Jogador* ptr)
{
    ptrJ = ptr;
}
Jogador* Projetil::getPtrJ()
{
    return  ptrJ;
}
void Projetil::setMapaAtual(Mapa * ptr)
{
    mapaAtual = ptr;
}
