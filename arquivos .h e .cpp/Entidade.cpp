#include "Entidade.h"
#include "Lista_Entidades.h"

ALLEGRO_FONT* Entidade::font16 = NULL;

Entidade::Entidade()
{
    tomandoDano = false;
    funcionando=false;
    colisao = NULL;
    dano = 0;
    timerEnt = NULL;
    posDano = Vetor_R2::vetor(-100,-100);
    posVelo = Vetor_R2::vetor(0,0);
    colidePlat=true;
}

Entidade::Entidade(int sX, int sY, char* nomeImg, int numEstados) :
    desenhaFrame(sX,sY,nomeImg, numEstados)
{
    tomandoDano = false;
    funcionando = false;
    colisao = NULL;
    dano = 0;
    timerEnt = NULL;
    posDano = Vetor_R2::vetor(-100,-100);
    posVelo = Vetor_R2::vetor(0,0);
    colidePlat=true;
    vidaBase = 100;
    vidaRand = 0;
    danoRand = 30;
    danoBase = 0;

}
Entidade::~Entidade()
{
    if (colisao != NULL)
        delete []colisao;
    colisao = NULL;
}
void Entidade::mudaScore(int s)
{
    listEnt->mudaScore(s);
}
void Entidade::diminuiVida(int d)
{
    if (!tomandoDano)
        vida -= d;
}
int Entidade::getDano()
{
    return dano;
}
int Entidade::getVida()
{
    return vida;
}
Vetor_R2 Entidade::getPosDano()
{
    return posDano;
}
void Entidade::setPosDano(Vetor_R2 d)
{
    posDano = d;
}



Vetor_R2 Entidade::getPosVelo()
{
    return posVelo;
}

void Entidade::setPosVelo(Vetor_R2 v)
{
    posVelo = v;
}
int Entidade::getID()
{
    return ID;
}
void Entidade::setProj(int i)
{
    proj=i;
}
int Entidade::getProj()
{
    return proj;
}
void Entidade::setEstado(int e)
{
    estado = e;
}
int Entidade::getEstado()
{
    return estado;
}
void Entidade::setPermiteColisaoEnt(bool c)
{
    permiteColisaoEnt = c;
}
bool Entidade::getPermiteColisaoEnt()
{
    return permiteColisaoEnt;
}
void Entidade::setPermiteColisaoChao(bool c)
{
    permiteColisaoChao = c;
}
bool Entidade::getPermiteColisaoChao()
{
    return permiteColisaoChao;
}
void Entidade::setTomandoDano(bool t)
{
    tomandoDano = t;
}
bool Entidade::getTomandoDano()
{
    return tomandoDano;
}
void Entidade::setColisao(bool c, int i)
{
    colisao[i] = c;
}
void Entidade::setListaEnt(Lista_Entidades * l)
{
    listEnt = l;
}
Posicao& Entidade::getPos()
{
    return pos;
}
Vetor_R2& Entidade::getBorda()
{
    return borda;
}
Sprite& Entidade::getSprite()
{
    return desenhaFrame;
}
void Entidade::setFuncionando(bool f)
{
    funcionando=f;
}
bool Entidade::getFuncionando()
{
    return funcionando;
}
bool Entidade::getColidePlat()
{
    return colidePlat;
}
float Entidade::getDir()
{
    return dir;
}
void Entidade::setCorEnt(ALLEGRO_COLOR c)
{
    corEnt = c;
}
ALLEGRO_COLOR Entidade::getCorEnt()
{
    return corEnt;
}
void Entidade::resetaEntidade()
{

}
void Entidade::ativaTimer()
{
    while(timerParado.tamanho())
    {
        al_start_timer(timerEnt[timerParado.retira_frente()]);
    }
}
