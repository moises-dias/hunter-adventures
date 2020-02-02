#include "Ataque_Generico.h"


Ataque_Generico::Ataque_Generico()
{

}
Ataque_Generico::Ataque_Generico(Vetor_R2 posI, Vetor_R2 borda_ataque, int dano_ataque, int tempo_ativo) :
    Projetil(0, 0, " ", 0)
{
    /**PARAMETROS DO PROJETIL**/
    danoBase = dano_ataque;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =true;
    permiteColisaoChao = false;
    funcionando = false;
    tomandoDano = false;
    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);

    borda=borda_ataque;
    dano = danoBase;



    tempoAtual=0;
    tempoMax=tempo_ativo;
}
Ataque_Generico::~Ataque_Generico()
{

}

void Ataque_Generico::desenha()
{
}
void Ataque_Generico::atualiza()
{
    if(funcionando)
    {
        if((++tempoAtual)>tempoMax)
        {
            resetaEntidade();
        }
        else if(tomandoDano)
        {
            resetaEntidade();
        }
    }
}

void Ataque_Generico::setBorda(Vetor_R2 b)
{
    borda=b;
}
void Ataque_Generico::setTempoMax(int t)
{
    tempoMax=t;
}


void Ataque_Generico::setDano(int d)
{
    dano=d;
}

void Ataque_Generico::criaFrames()
{

}
void Ataque_Generico::resetaEntidade()
{
    funcionando=false;
    tomandoDano=false;

    tempoAtual=0;
    pos.getEspaco().setXY(-200,-200);
}
