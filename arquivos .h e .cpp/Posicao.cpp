#include "Posicao.h"

Posicao::Posicao()
{

}

Posicao::~Posicao()
{

}

void Posicao::mover()
{
    velocidade += aceleracao;
    espaco += velocidade;
}

void Posicao::setEspaco(Vetor_R2 esp)
{
    espaco = esp;
}
Vetor_R2& Posicao::getEspaco()
{
    return espaco;
}
void Posicao::setVelocidade(Vetor_R2 vel)
{
    velocidade = vel;
}
Vetor_R2& Posicao::getVelocidade()
{
    return velocidade;
}
void Posicao::setAceleracao(Vetor_R2 ace)
{
    aceleracao = ace;
}
Vetor_R2& Posicao::getAceleracao()
{
    return aceleracao;
}
void Posicao::setPlatAtual(int plat)
{
    platAtual = plat;
}
int Posicao::getPlatAtual()
{
    return platAtual;
}
