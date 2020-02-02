#include "Coluna_Fogo.h"

Coluna_Fogo::Coluna_Fogo()
{

}
Coluna_Fogo::Coluna_Fogo(Vetor_R2 posI) :
    Projetil(100, 420, "Coluna_Chefe.png", 2)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1000000;
    vidaRand = 0;
    danoBase = 1500;
    danoRand = 150;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =false;
    permiteColisaoChao = false;
    borda.setXY(40, 290);
    tomandoDano=false;

    estado=0;
    desenhaFrame.setEstado(0);

    funcionando=false;
    colidePlat=false;

    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    ptrJ = NULL;


    criaFrames();
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    desenhaFrame.setCoef(1.5);

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);
    string nome[] = {"COLUNA_EXPLOSAO"};
    audioEnt.inicializaAudio(nome, 1);
}
Coluna_Fogo::~Coluna_Fogo()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete[]timerEnt;
}

void Coluna_Fogo::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(77,340), 0, false, corEnt);
    }
}
void Coluna_Fogo::atualiza()
{
    if(funcionando)
    {
        if(!estado &&
                (desenhaFrame.getFrameAtual()==estFrame[0][1]) &&
                desenhaFrame.getTempoEstado()>=tFrame[0][desenhaFrame.getFrameAtual() - estFrame[estado][0]] &&
                !permiteColisaoEnt)
        {
            audioEnt.tocaAudio("COLUNA_EXPLOSAO", 0.5, ALLEGRO_PLAYMODE_ONCE, 0);
            estado=1;
            desenhaFrame.setEstado(1);
            permiteColisaoEnt=true;
            al_start_timer(timerEnt[0]);
        }
        if(al_get_timer_count(timerEnt[0])>300)
        {
            resetaEntidade();
        }

        desenhaFrame.atualizaFrame();
        audioEnt.atualizaAudio();
    }
}
void Coluna_Fogo::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 0;
    estFrame[0][1] = 5;
    estFrame[0][2] = 0;
    estFrame[1][0] = 6;
    estFrame[1][1] = 8;
    estFrame[1][2] = 1;

    for(i=0; i<6; i++)
        tFrame[0].inclui_frente(12);
    for(i=0; i<3; i++)
        tFrame[1].inclui_frente(7);
}
void Coluna_Fogo::resetaEntidade()
{
    funcionando=false;
    tomandoDano=false;
    permiteColisaoEnt=false;
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();
    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0],0);

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
void Coluna_Fogo::paraTimer()
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
