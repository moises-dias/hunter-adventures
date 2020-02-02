#include "Gota_Lava.h"

Gota_Lava::Gota_Lava()
{

}
Gota_Lava::Gota_Lava(Vetor_R2 posI) :
    Projetil(52, 52, "Gota_Lava.png", 2)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1;
    vidaRand = 0;
    danoBase = 1000;
    danoRand = 100;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =true;
    permiteColisaoChao = true;
    borda.setXY(20, 24);
    tomandoDano=false;

    estado=0;
    desenhaFrame.setEstado(0);

    funcionando=false;


    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0.2);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    ptrJ = NULL;


    criaFrames();
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    desenhaFrame.setCoef(3);

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);

    string nome[] = {"GOTA_LAVA_CHAO"};
    audioEnt.inicializaAudio(nome, 1);
}
Gota_Lava::~Gota_Lava()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}
void Gota_Lava::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(80,55), 0, false, corEnt);
    }
}
void Gota_Lava::atualiza()
{
    if(funcionando)
    {
        if(estado && !al_get_timer_started(timerEnt[0]))
        {
            al_start_timer(timerEnt[0]);
            if (!audioEnt.audioTocando("GOTA_LAVA_CHAO"))
                        audioEnt.tocaAudio("GOTA_LAVA_CHAO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }

        if(estado)
            if(al_get_timer_count(timerEnt[0])>240)
            {
                resetaEntidade();
            }

        if(colisao[COLIDE_Y])
        {
            desenhaFrame.setEstado(1);
            estado=1;
        }


        if(vida<0 || tomandoDano)
        {
            resetaEntidade();
        }

        desenhaFrame.atualizaFrame();
        if(!estado)
            pos.mover();
        audioEnt.atualizaAudio();
    }
}
void Gota_Lava::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 0;
    estFrame[0][1] = 5;
    estFrame[0][2] = 1;
    estFrame[1][0] = 6;
    estFrame[1][1] = 18;
    estFrame[1][2] = 1;

    for(i=0; i<6; i++)
        tFrame[0].inclui_frente(5);
    for(i=0; i<13; i++)
        tFrame[1].inclui_frente(7);

}
void Gota_Lava::setEst(int e)
{
    estado=e;
    desenhaFrame.setEstado(e);
}
void Gota_Lava::resetaEntidade()
{
    al_set_timer_count(timerEnt[0],0);
    al_stop_timer(timerEnt[0]);
    funcionando=false;
    tomandoDano=false;
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0.2);
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
void Gota_Lava::paraTimer()
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
