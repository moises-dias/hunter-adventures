#include "Efeito.h"

Efeito::Efeito()
{
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
}

Efeito::~Efeito()
{

}
void Efeito::setPtrJ(Jogador* ptr)
{
    ptrJ = ptr;
}
void Efeito::setMapaAtual(Mapa * ptr)
{
    mapaAtual = ptr;
}
