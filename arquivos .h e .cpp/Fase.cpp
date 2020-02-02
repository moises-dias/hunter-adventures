#include "Fase.h"
#include "Lista_Entidades.h"
#include "Fumaca.h"
#include "Jogo.h"
#include "Efeito_Dano.h"

Fase::Fase()
{
    posX = 0;
    comecouFase = false;
    alpha = 255;
    timerParado = false;
    fade = -1;
}

Fase::~Fase()
{
    while(listFum.tamanho())
        delete listFum.retira_frente();
    while (listEDano.tamanho())
        delete listEDano.retira_frente();
}
void Fase::inicializaFumaca()
{
    Fumaca * ptrF;
    for(int i = 0; i < NUM_FUMACA; i++)
    {
        ptrF = new Fumaca();
        ptrF->setMapaAtual(ptrM);
        listFum.inclui_frente(ptrF);
    }
}
void Fase::passaFase(int i)
{
    jogo->resetaSave(i, 0);
}
void Fase::setJogo(Jogo* j)
{
    jogo = j;
}
void Fase::atualizaEDano()
{
    Lista_Generica<Efeito_Dano*>::Iterador iteE;
    iteE = listEDano.inicio();
    for (int i = 0; i < NUM_EFEITO_DANO; i++, iteE++)
    {
        if ((*iteE)->getFuncionando())
        {
            if ((*iteE)->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() > -150 &&
                    (*iteE)->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() <  1350)
            {
                (*iteE)->atualiza();
            }
            else
            {
                (*iteE)->setFuncionando(false);
            }
        }
    }
}
void Fase::desenhaEDano()
{
    Lista_Generica<Efeito_Dano*>::Iterador iteE;
    iteE = listEDano.inicio();
    for (int i = 0; i < NUM_EFEITO_DANO; i++, iteE++)
    {
        if ((*iteE)->getFuncionando())
        {
            (*iteE)->desenha();
        }
    }
}
void Fase::inicializaEDano()
{
    Efeito_Dano * ptrE = NULL;
    for(int i = 0; i < NUM_EFEITO_DANO; i++)
    {
        ptrE = new Efeito_Dano();
        ptrE->setMapaAtual(ptrM);
        listEDano.inclui_frente(ptrE);
    }
}
void Fase::criaDano(Vetor_R2 esp, Vetor_R2 b, int dano)
{
    Lista_Generica<Efeito_Dano*>::Iterador iteE;
    iteE = listEDano.inicio();
    for ( int i = 0 ; i < listEDano.tamanho(); i++, iteE++)
    {
        if (!(*iteE)->getFuncionando())
        {
            (*iteE)->getPos().getEspaco().setXY
            (esp.getX() - b.getX() + rand()%( 2*((int)b.getX())), esp.getY() - b.getY() + rand()%(((int)b.getY())/4));
            (*iteE)->setDano(dano);
            (*iteE)->setFuncionando(true);
            break;
        }
    }
}
Jogador* Fase::getJog(int i)
{
    return ptrJ[i];
}
void Fase::paraMusica()
{
    string s;
    char* num = "123";
    for (int i = 0; i < 3; i++)
    {
        s = "F" + num[i];
        if (audioFase.audioTocando(s))
            audioFase.paraAudio(s);
        for (int j = 0; j < 2; j++)
        {
            s = "S" + num[j];
            s += "-F" + num[i];
            if (audioFase.audioTocando(s))
            {
                audioFase.paraAudio(s);
            }
        }
    }
    if (jogo->getFase() == 2 && audioFase.audioTocando("boss"))
    {
        audioFase.paraAudio("boss");
    }
}
void Fase::paraTimer()
{
    for (int i = 0; i < 1; i++)
    {
        if (al_get_timer_started(timers[i]))
        {
            al_stop_timer(timers[i]);
            timersParados.inclui_frente(i);
        }
    }

}
void Fase::ativaTimer()
{
    while(timersParados.tamanho())
    {
        al_start_timer(timers[timersParados.retira_frente()]);
    }
}

