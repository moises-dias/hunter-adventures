#include "Plataforma.h"

Plataforma::Plataforma()
{

}

Plataforma::~Plataforma()
{

}
void Plataforma::setTipo( int t)
{
    tipo = t;
}
int Plataforma::getTipo()
{
    return tipo;
}
Vetor_R2& Plataforma::getPos(int i)
{
    return posicao[i];
}
