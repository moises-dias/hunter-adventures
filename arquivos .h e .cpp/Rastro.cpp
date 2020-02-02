#include "Rastro.h"

Rastro::Rastro(Vetor_R2 p, ALLEGRO_BITMAP* img, int sX, int sY, float coef, float velDes,
               float alphaIni, float ang, float cR, float cG, float cB)
{
    ID = 32;

    permiteColisaoEnt =false;
    permiteColisaoChao = false;
    funcionando = false;
    desenhaFrame.setCoef(coef);
    borda.setXY(0, 0);
    corEnt = al_map_rgba(255*alphaIni,255*alphaIni,255*alphaIni,255*alphaIni);
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
    cRed = cR;
    cGreen = cG;
    cBlue = cB;

    pos.setEspaco(p);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    desenhaFrame.setImagem(img);
    desenhaFrame.setSpriteX(sX);
    desenhaFrame.setSpriteY(sY);

    vDes = velDes;
    alpha = alphaIni + vDes;
    angulo = ang;

    timerEnt = new ALLEGRO_TIMER*[1];

    timerEnt[0] = al_create_timer(1/4.0);


}
Rastro::Rastro()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}

Rastro::~Rastro()
{
    desenhaFrame.setImagem(NULL);
}
void Rastro::desenha()
{
    if (funcionando)
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco(), angulo, false, al_map_rgba(255*alpha*cRed,255*alpha*cGreen,255*alpha*cBlue,255*alpha) );
}
void Rastro::atualiza()
{
    if (funcionando)
    {
        if (!al_get_timer_started(timerEnt[0]))
        {
            al_start_timer(timerEnt[0]);
        }

        alpha -= vDes;
        if (alpha <= 0.05)
        {
            resetaEntidade();
        }
        pos.mover();

        if (al_get_timer_count(timerEnt[0]) != 0)
        {
            resetaEntidade();
        }
    }
}


void Rastro::setPos(Vetor_R2 p)
{
    pos.setEspaco(p);
}
void Rastro::setFrameAtual(int f)
{
    desenhaFrame.setFrameAtual(f);
}
void Rastro::setVelDes(float v)
{
    vDes = v;
    alpha += vDes;
}
void Rastro::setAng(int a)
{
    angulo = a;
}
void Rastro::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    funcionando = false;
    alpha = 1;
    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0], 0);
}

void Rastro::paraTimer()
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
