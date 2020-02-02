#include "Bola_Fogo.h"

Bola_Fogo::Bola_Fogo()
{

}
Bola_Fogo::Bola_Fogo(Vetor_R2 posI) :
    Projetil(52, 52, "Gota_Lava.png", 1)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1000000;
    vidaRand = 0;
    danoBase = 900;
    danoRand = 90;
    /**PARAMETROS DO PROJETIL**/

    permiteColisaoChao = false;
    borda.setXY(25, 40);

    ID = 16;
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();


    tomandoDano=false;
    funcionando=false;
    permiteColisaoEnt=true;

    colidePlat=false;


    desenhaFrame.setGiraRot(false);
    corEnt=al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    ptrJ = NULL;


    criaFrames();
    desenhaFrame.setEstado(0);


    desenhaFrame.setCoef(6.1);
    string nome[] = {"BOLA_FOGO"};
    audioEnt.inicializaAudio(nome, 1);

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);
}
Bola_Fogo::~Bola_Fogo()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete [] timerEnt;
}
void Bola_Fogo::desenha()
{
    if(funcionando)
    {
        if(pos.getVelocidade().getY()>0)
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(162, 120),
                                 0, false, corEnt);
        else
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(162, 200),
                                 0, false, corEnt);
    }
}
void Bola_Fogo::atualiza()
{
    if(funcionando)
    {
        pos.mover();

        estado = desenhaFrame.getEstado();

        if(pos.getVelocidade().getY()>0)
            permiteColisaoChao=true;

        if(al_get_timer_started(timerEnt[0]) && al_get_timer_count(timerEnt[0])>60)
        {
            pos.getVelocidade().setY(-10);
            pos.getAceleracao().setY(0.1);
            al_stop_timer(timerEnt[0]);
        }

        if(pos.getVelocidade().getY()<0)
            desenhaFrame.setGiraRot(true);
        else
            desenhaFrame.setGiraRot(false);
        if(colisao[COLIDE_Y])
        {
            resetaEntidade();
            if(!audioEnt.audioTocando("BOLA_FOGO"))
                audioEnt.tocaAudio("BOLA_FOGO",0.5, ALLEGRO_PLAYMODE_ONCE,0);
        }



        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();



        audioEnt.atualizaAudio();
    }
}

void Bola_Fogo::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0]=0;
    estFrame[0][1]=5;
    estFrame[0][2]=1;

    for(i=0; i<6; i++)
        tFrame[0].inclui_frente(7);
}

void Bola_Fogo::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    desenhaFrame.setGiraRot(false);
    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0],0);
}
void Bola_Fogo::setFuncionando(bool f)
{
    funcionando=f;
    pos.getVelocidade().setY(-0.5);
    permiteColisaoChao=false;
    al_start_timer(timerEnt[0]);
}
void Bola_Fogo::paraTimer()
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
