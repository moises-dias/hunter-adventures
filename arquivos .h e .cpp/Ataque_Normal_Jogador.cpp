#include "Ataque_Normal_Jogador.h"
#include "Lista_Entidades.h"
#include "Rastro.h"


Ataque_Normal_Jogador::Ataque_Normal_Jogador(Vetor_R2 posI, int di, bool ve):
    Projetil( 96, 96, "Projeteis.png", 2)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 100;
    vidaRand = 0;
    danoBase=1750;
    danoRand = 250;
    /**PARAMETROS DO PROJETIL**/

    ID = 8;
    permiteColisaoEnt =true;
    permiteColisaoChao = false;
    funcionando = false;
    coefVel = 0.1;
    desenhaFrame.setCoef(1.4);
    desenhaFrame.setGiraRot(ve);
    borda.setXY(20, 35);


    alpha = 1.0f;

    dir = di;
    pos.setEspaco(posI);
    pos.getVelocidade().setXY(20*(1 - 2*dir),0);
    pos.getAceleracao().setXY((2*dir - 1),0);


    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    criaFrames();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/5.5);


}
Ataque_Normal_Jogador::Ataque_Normal_Jogador()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete [] timerEnt;
}

Ataque_Normal_Jogador::~Ataque_Normal_Jogador()
{
    al_destroy_timer(timerEnt[0]);
}
void Ataque_Normal_Jogador::desenha()
{
    if (funcionando)
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(77 - 20*dir, 66),
                             dir, false, al_map_rgba(255*alpha,255*alpha,255*alpha,255*alpha));

}
void Ataque_Normal_Jogador::atualiza()
{
    if (funcionando)
    {
        alpha -= 0.05;
        if (al_get_timer_count(timerEnt[0]))
        {
            al_set_timer_count(timerEnt[0], 0);
            al_stop_timer(timerEnt[0]);
            desenhaFrame.setEstado(1);
        }
        if (desenhaFrame.getEstado() == 1 && desenhaFrame.getFrameAtual() == estFrame[1][1] && desenhaFrame.getContTempo() >= tFrame[1][desenhaFrame.getFrameAtual() - estFrame[1][0]])
        {
            alpha = 1;
            pos.getEspaco().setXY(-200,-200);
            funcionando = false;
        }
        desenhaFrame.atualizaFrame();
        pos.mover();
    }

}
void Ataque_Normal_Jogador::criaFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[0][0] = 0;
    estFrame[0][1] = 0;
    estFrame[0][2] = 1;
    estFrame[1][0] = 1;
    estFrame[1][1] = 3;
    estFrame[1][2] = 0;

    tFrame[0].inclui_frente(1);
    tFrame[1].inclui_frente(2);
    tFrame[1].inclui_frente(2);
    tFrame[1].inclui_frente(2);
}
void Ataque_Normal_Jogador::setFuncionando(bool f)
{
    funcionando = f;
    vida = 100;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    desenhaFrame.setEstado(0);
    al_start_timer(timerEnt[0]);
}
void Ataque_Normal_Jogador::setPos(Vetor_R2 p)
{
    pos.setEspaco(p);
}
void Ataque_Normal_Jogador::setDi(int d)
{
    pos.getVelocidade().setXY(20*(1 - 2*d),0);
    pos.getAceleracao().setXY((2*d - 1),0);
    dir = d;
}
void Ataque_Normal_Jogador::setVer(bool v)
{
    desenhaFrame.setGiraRot(v);
}
void Ataque_Normal_Jogador::resetaEntidade()
{
    al_set_timer_count(timerEnt[0], 0);
    al_stop_timer(timerEnt[0]);
    alpha = 1;
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    pos.getEspaco().setXY(-200,-200);
    funcionando = false;
}
void Ataque_Normal_Jogador::paraTimer()
{
    for (int i = 0; i < 1; i++)
    {
        if (al_get_timer_started(timerEnt[i]))
        {
            al_stop_timer(timerEnt[i]);
            timerParado.inclui_frente(i);
        }
    }
}
