#include "Ataque_Cima_Jogador.h"

Ataque_Cima_Jogador::Ataque_Cima_Jogador()
{

}
Ataque_Cima_Jogador::Ataque_Cima_Jogador(Vetor_R2 posI):
    Projetil(96,96, "Projeteis.png", 2)
{

    /**PARAMETROS DO PROJETIL**/
    vidaBase = 100;
    vidaRand = 0;
    danoBase = 2200;
    danoRand = 300;
    /**PARAMETROS DO PROJETIL**/


    ID = 8;
    permiteColisaoEnt =true;
    permiteColisaoChao = false;
    funcionando = false;
    desenhaFrame.setCoef(2);
    desenhaFrame.setGiraRot(0);
    borda.setXY(18, 40);

    alpha = 1.0f;

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,-20);
    pos.getAceleracao().setXY(0, 1);


    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    criaFrames();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/5.5);
}

Ataque_Cima_Jogador::~Ataque_Cima_Jogador()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete []timerEnt;
}
void Ataque_Cima_Jogador::desenha()
{
    if (funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(desenhaFrame.getSpriteX(), desenhaFrame.getSpriteY())
                         , dir, false, al_map_rgba(255*alpha,255*alpha,255*alpha,255*alpha));
    }
}
void Ataque_Cima_Jogador::atualiza()
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
            pos.getEspaco().setXY(-200,-200);
            pos.getVelocidade().setY(-20);
            alpha = 1;
            funcionando = false;
        }
        desenhaFrame.atualizaFrame();
        pos.mover();
    }

}
void Ataque_Cima_Jogador::criaFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 17;
    estFrame[0][1] = 18;
    estFrame[0][2] = 1;
    estFrame[1][0] = 19;
    estFrame[1][1] = 21;
    estFrame[1][2] = 0;

    tFrame[0].inclui_frente(5);
    tFrame[0].inclui_frente(5);
    tFrame[1].inclui_frente(4);
    tFrame[1].inclui_frente(4);
    tFrame[1].inclui_frente(4);
}
void Ataque_Cima_Jogador::setFuncionando(bool f)
{
    funcionando = f;
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    desenhaFrame.setEstado(0);
    al_start_timer(timerEnt[0]);
}
void Ataque_Cima_Jogador::setPos(Vetor_R2 p)
{
    pos.setEspaco(p);
}
void Ataque_Cima_Jogador::resetaEntidade()
{
    al_set_timer_count(timerEnt[0], 0);
    al_stop_timer(timerEnt[0]);
    alpha = 1;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    pos.getEspaco().setXY(-200,-200);
    funcionando = false;
}
void Ataque_Cima_Jogador::paraTimer()
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
