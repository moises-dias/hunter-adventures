#include "Slime.h"

Slime::Slime()
{

}
Slime::Slime(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 1400;
    vidaRand = 140;
    danoBase = 500;
    danoRand = 50;
    score = 50;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(18, 18);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Slime.png");

    desenhaFrame.setNumEstados(9);

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;
    aceleracao=0.5;

    yPlat=0;
    delXQueda=100+rand()%150;

    atirar=false;
    tomandoDano=false;

    vRand = (rand()%1001)/1000.0;
    vXMax=0.5+vRand;
    velPulo.setXY(0,0);
    vYPulo=0;
    vYPuloMedio=0;

    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(SLI_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [2];
    timerEnt[0] = al_create_timer(1/60.0);
    timerEnt[1] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);


    string nome[] = {"SLIME_DANO", "SLIME_MORTE", "SLIME_PULO", "SLIME_QUICA"};
    audioEnt.inicializaAudio(nome, 4);
}
Slime::~Slime()
{
    for(int i = 0; i < 2; i++)
        al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}

void Slime::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 58), dir, false, corEnt);
    }
}
void Slime::atualiza()
{
    if(funcionando)
    {


        estado = desenhaFrame.getEstado();
        if (colisao[COLIDE_Y])
        {
            pos.getVelocidade().setY(0);
            pos.getAceleracao().setY(0);
        }
        else if(estado!=SLI_PULANDO_DEVAGAR && estado!=SLI_PULANDO_RAPIDO)
            pos.getAceleracao().setY(1);

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();

        pos.mover();

        if (tomandoDano)
            permiteColisaoEnt = false;
        else
            permiteColisaoEnt = true;

        if(vida <= 0 && estado!=SLI_MORRENDO)
        {
            estado=SLI_MORRENDO;
            if (!audioEnt.audioTocando("SLIME_MORTE"))
                audioEnt.tocaAudio("SLIME_MORTE", 0.5, ALLEGRO_PLAYMODE_ONCE, 0);
        }


        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();
        audioEnt.atualizaAudio();
    }
}
void Slime::atualizaComportamento()
{


    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {


        float x = pos.getEspaco().getX();
        float vx = pos.getVelocidade().getX();
        float ax = pos.getAceleracao().getX();

        if(jogadores[0]==NULL||jogadores[1]==NULL)
        {
            if(jogadores[0]==NULL)
                alvo=jogadores[1];
            else
                alvo=jogadores[0];
        }
        else
        {
            float p0x = jogadores[0]->getPos().getEspaco().getX();
            float p1x = jogadores[1]->getPos().getEspaco().getX();

            if(abs(x-p0x)<abs(x-p1x))
                alvo=jogadores[0];
            else
                alvo=jogadores[1];
        }



        if(estado == SLI_CAINDO_PLAT)
        {
            pos.getAceleracao().setX(0);
            if(pos.getEspaco().getY()-getBorda().getY() > yPlat)
                permiteColisaoChao=true;
            if(colisao[COLIDE_Y])
            {
                estado=SLI_CAINDO_PULO;
            }
        }

        else if(estado==SLI_COMECA_PULO||estado==SLI_PARADO||estado==SLI_MORRENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
        }
        else if(estado==SLI_DANO)
        {
            pos.getAceleracao().setX(0);
            if(colisao[COLIDE_Y])
            {
                estado=SLI_CAINDO_PULO;
                tomandoDano=false;
                al_set_timer_count(timerEnt[0],0);
            }
        }
        else if(estado!=SLI_PULANDO_DEVAGAR && estado!=SLI_PULANDO_RAPIDO)
        {
            float alvoX=alvo->getPos().getEspaco().getX();

            if(alvoX<x)
            {
                dir=1;

                if (desenhaFrame.getFrameAtual() >= 3 && desenhaFrame.getFrameAtual() <=9)
                {
                    if(vx> - (vXMax) )
                        pos.getAceleracao().setX(-0.3);
                    else if(vx<=-(vXMax))
                    {
                        pos.getAceleracao().setX(0);
                        pos.getVelocidade().setX(-(vXMax));
                    }
                    if ((!audioEnt.audioTocando("SLIME_QUICA")) && (!audioEnt.audioTocando("SLIME_PULO")) &&
                        desenhaFrame.getFrameAtual()==9)
                        audioEnt.tocaAudio("SLIME_QUICA", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                }
                else
                {
                    pos.getAceleracao().setX(0);
                    pos.getVelocidade().setX(0);
                }
            }
            else
            {
                dir=0;
                if (desenhaFrame.getFrameAtual() >= 3 && desenhaFrame.getFrameAtual() <=9)
                {

                    if(vx<(vXMax))
                        pos.getAceleracao().setX(0.3);
                    else if(vx>=(vXMax))
                    {
                        pos.getAceleracao().setX(0);
                        pos.getVelocidade().setX((vXMax));
                    }
                    if ((!audioEnt.audioTocando("SLIME_QUICA")) && (!audioEnt.audioTocando("SLIME_PULO")) &&
                        desenhaFrame.getFrameAtual()==9)
                        audioEnt.tocaAudio("SLIME_QUICA", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                }
                else
                {
                    pos.getAceleracao().setX(0);
                    pos.getVelocidade().setX(0);
                }
            }
        }
        if(estado==SLI_COMECA_PULO)
        {
            if(atirar)
            {
                float velX,delX,velY,delYSubir,delYDescer,tempo;

                delX = alvo->getPos().getEspaco().getX() - pos.getEspaco().getX();

                if(pos.getEspaco().getY() <= alvo->getPos().getEspaco().getY())
                {
                    delYSubir=100+rand()%200;
                    delYDescer=delYSubir;
                }
                else
                {
                    delYSubir=pos.getEspaco().getY() - alvo->getPos().getEspaco().getY() + 100 + rand()%200;
                    delYDescer=delYSubir-(pos.getEspaco().getY() - alvo->getPos().getEspaco().getY());
                }
                velY=-sqrt(2.0*aceleracao*delYSubir);

                tempo=sqrt(2.0*abs(delYSubir)/aceleracao)+sqrt(2.0*abs(delYDescer)/aceleracao);
                velX=delX/tempo;

                velPulo.setXY(velX,velY);
                atirar=false;
            }
        }
        if(estado==SLI_PULANDO_DEVAGAR || estado==SLI_PULANDO_RAPIDO)
        {
            pos.getAceleracao().setY(aceleracao);
        }
        if((estado == SLI_COMECA_PULO || estado == SLI_CAINDO_PULO) && desenhaFrame.getFrameAtual()==11)
        {
            if ((!audioEnt.audioTocando("SLIME_QUICA")) && (!audioEnt.audioTocando("SLIME_PULO")))
                        audioEnt.tocaAudio("SLIME_PULO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }



}
void Slime::atualizaColisoes()
{

    if (colisao[COLIDE_XD] && pos.getVelocidade().getX() >0)
    {
        pos.getVelocidade().setX(-pos.getVelocidade().getX());
    }
    if(colisao[COLIDE_XE] && pos.getVelocidade().getX() <0)
    {
        pos.getVelocidade().setX(-pos.getVelocidade().getX());
    }

}
void Slime::atualizaEstados()
{
    if(jogadores[0]==NULL&&jogadores[1]==NULL)
    {
        estado=SLI_PARADO;
    }

    else if(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<delXQueda &&
            al_get_timer_count(timerEnt[1])>120 && estado!=SLI_CAINDO_PLAT && estado!=SLI_COMECA_PULO &&
            estado!=SLI_CAINDO_PULO && !tomandoDano &&
            colisao[COLIDE_Y] && alvo->getPos().getAceleracao().getY()==0 && ((rand()%1000)/990) &&
            alvo->getPos().getEspaco().getY()-alvo->getBorda().getY() > pos.getEspaco().getY()+getBorda().getY())
    {
        estado=SLI_CAINDO_PLAT;
        permiteColisaoChao=false;
        yPlat=pos.getEspaco().getY()+getBorda().getY()+10;
    }



    else if(tomandoDano && estado!=SLI_MORRENDO && estado!=SLI_DANO && vida>0)
    {
        estado=SLI_DANO;

        if (!audioEnt.audioTocando("SLIME_DANO"))
            audioEnt.tocaAudio("SLIME_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);

        if(posVelo.getX()>0)
            pos.getVelocidade().setXY(5,-14);

        if(posVelo.getX()<0)
            pos.getVelocidade().setXY(-5,-14);
        if(posVelo.getX()==0)
            pos.getVelocidade().setXY(0,-14);
    }
    else if((estado==SLI_ANDANDO && (al_get_timer_count(timerEnt[0]) > 60) &&
             abs((alvo->getPos().getEspaco().getX())-(pos.getEspaco().getX())) < 300 &&
             pos.getEspaco().getY()+getBorda().getY() > alvo->getPos().getEspaco().getY()-alvo->getBorda().getY() &&
             alvo->getPos().getAceleracao().getY()==0))
    {
        estado=SLI_COMECA_PULO;
        atirar=true;
    }
    else if(estado==SLI_COMECA_PULO&& (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        estado=SLI_PULANDO_RAPIDO;
        pos.setVelocidade(velPulo);
        pos.getAceleracao().setY(aceleracao);
    }
    else if(estado==SLI_PULANDO_RAPIDO && abs(pos.getVelocidade().getY())<abs(velPulo.getY()/2))
    {
        estado=SLI_PULANDO_DEVAGAR;
    }
    else if(estado==SLI_PULANDO_DEVAGAR && abs(pos.getVelocidade().getY())>abs(velPulo.getY()/2))
    {
        estado=SLI_PULANDO_RAPIDO;
    }
    else if((estado==SLI_PULANDO_RAPIDO || estado==SLI_PULANDO_DEVAGAR)&&colisao[COLIDE_Y])
    {
        estado=SLI_CAINDO_PULO;
    }
    else if(estado==SLI_CAINDO_PULO&& (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        estado=SLI_ANDANDO;
        atirar=true;
        al_set_timer_count(timerEnt[0],0);
        al_set_timer_count(timerEnt[1],rand()%60);
    }
    else if(estado==SLI_MORRENDO && (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaScore(score);
        resetaEntidade();
    }

}
void Slime::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[SLI_PARADO][0]=0;
    estFrame[SLI_PARADO][1]=9;
    estFrame[SLI_PARADO][2]=1;
    estFrame[SLI_ANDANDO][0]=0;
    estFrame[SLI_ANDANDO][1]=9;
    estFrame[SLI_ANDANDO][2]=1;
    estFrame[SLI_COMECA_PULO][0]=10;
    estFrame[SLI_COMECA_PULO][1]=12;
    estFrame[SLI_COMECA_PULO][2]=0;
    estFrame[SLI_CAINDO_PULO][0]=10;
    estFrame[SLI_CAINDO_PULO][1]=12;
    estFrame[SLI_CAINDO_PULO][2]=0;
    estFrame[SLI_PULANDO_DEVAGAR][0]=6;
    estFrame[SLI_PULANDO_DEVAGAR][1]=6;
    estFrame[SLI_PULANDO_DEVAGAR][2]=1;
    estFrame[SLI_PULANDO_RAPIDO][0]=13;
    estFrame[SLI_PULANDO_RAPIDO][1]=13;
    estFrame[SLI_PULANDO_RAPIDO][2]=1;
    estFrame[SLI_DANO][0]=14;
    estFrame[SLI_DANO][1]=14;
    estFrame[SLI_DANO][2]=1;
    estFrame[SLI_MORRENDO][0]=14;
    estFrame[SLI_MORRENDO][1]=23;
    estFrame[SLI_MORRENDO][2]=0;
    estFrame[SLI_CAINDO_PLAT][0]=4;
    estFrame[SLI_CAINDO_PLAT][1]=4;
    estFrame[SLI_CAINDO_PLAT][2]=1;

    for(i=0; i<10; i++)
    {
        tFrame[SLI_PARADO].inclui_frente(10);
        tFrame[SLI_ANDANDO].inclui_frente(10);
        tFrame[SLI_MORRENDO].inclui_frente(10);
    }
    for(i=0; i<3; i++)
    {
        tFrame[SLI_COMECA_PULO].inclui_frente(10);
        tFrame[SLI_CAINDO_PULO].inclui_frente(10);
    }

    tFrame[SLI_CAINDO_PLAT].inclui_frente(10);

    tFrame[SLI_PULANDO_RAPIDO].inclui_frente(1);
    tFrame[SLI_PULANDO_DEVAGAR].inclui_frente(1);
    tFrame[SLI_DANO].inclui_frente(60);
}
void Slime::mudaEstado(int nEstado)
{

}
void Slime::criaEfeitos()
{

}

void Slime::criaProjetil()
{

}

void Slime::inicializaEntidade()
{

}
void Slime::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    atirar=false;
    tomandoDano=false;
    funcionando=false;

    estado=SLI_ANDANDO;
    desenhaFrame.setEstado(SLI_ANDANDO);
    desenhaFrame.atualizaFrame();

    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0],0);

    al_stop_timer(timerEnt[1]);
    al_set_timer_count(timerEnt[1],0);
}
void Slime::setFuncionando(bool f)
{
    funcionando=f;
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);
}
void Slime::paraTimer()
{
    for (int i = 0; i < 2; i++)
    {
        if (al_get_timer_started(timerEnt[i]))
        {
            al_stop_timer(timerEnt[i]);
            timerParado.inclui_frente(i);
        }
    }
}
