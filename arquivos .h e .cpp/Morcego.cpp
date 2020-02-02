#include "Morcego.h"
#include "Lista_Entidades.h"
Morcego::Morcego()
{

}
Morcego::Morcego(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 2800;
    vidaRand = 280;
    danoBase = 1500;
    danoRand = 150;
    score = 300;
    /**PARAMETROS DO INIMMIGO**/

    permiteColisaoChao=true;
    colidePlat=false;

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0.5, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(28, 35);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Morcego.png");

    desenhaFrame.setNumEstados(7);

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;
    vmax=4;

    delX=0;
    delY=0;
    velX=0;
    velY=0;
    tempo=0;
    yInic=0;

    distY=100+rand()%200;
    velXDano=0;
    velYDano=0;

    tempoCarrega = 30 + rand()%50;
    delayRasante = 220 + rand()%80;

    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);

    aceleracao=0.5;
    atirar=false;
    funcionando=false;
    tomandoDano=false;

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(MOR_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [2];
    timerEnt[0] = al_create_timer(1/60.0);
    timerEnt[1] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[1]);

    string nome[] = {"MORCEGO_DANO", "MORCEGO_MORRENDO", "MORCEGO_RASANTE"};
    audioEnt.inicializaAudio(nome, 3);
}
Morcego::~Morcego()
{
    for(int i = 0; i < 2; i++)
        al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}

void Morcego::desenha()
{
    if(funcionando)
    {
        if(!(estado == MOR_RASANTE_DESCENDO))
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 55),
                                 dir, false, corEnt);
        else
            desenhaFrame.desenha(pos.getEspaco()+Vetor_R2::vetor(52,52),
                                 mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 65),ang, true, corEnt);
    }

}
void Morcego::atualiza()
{
    if(funcionando)
    {
        estado = desenhaFrame.getEstado();

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();

        pos.mover();

        if(vida <= 0 && funcionando)
        {
            if(estado!=MOR_MORTO && estado!=MOR_MORRENDO && pos.getEspaco().getY()>-200 && pos.getEspaco().getY()<700)
            {
                audioEnt.tocaAudio("MORCEGO_MORRENDO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                mudaScore(score);
            }

            if(estado!=MOR_MORTO)
            {
                estado = MOR_MORRENDO;
            }

        }
        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();
        audioEnt.atualizaAudio();
    }
}
void Morcego::criaProjetil()
{

}
void Morcego::atualizaComportamento()
{
    if(pos.getEspaco().getX()-mapaAtual->getPos().getEspaco().getX()+pos.getVelocidade().getX() > 1250 ||
            pos.getEspaco().getX()-mapaAtual->getPos().getEspaco().getX()+pos.getVelocidade().getX() < -50)
    {
        pos.getVelocidade().setX(0);
        estado=MOR_ANDANDO;
    }
    if(!estado == MOR_DANO)
    {
        if(pos.getVelocidade().getX()>0)
            dir=0;
        else
            dir=1;
    }

    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {
        float x = pos.getEspaco().getX();
        float vx = pos.getVelocidade().getX();
        float ax = pos.getAceleracao().getX();
        float mapX=mapaAtual->getPos().getEspaco().getX();

        if(estado==MOR_ANDANDO)
        {
            if(pos.getVelocidade().getX()>vmax)
                pos.getVelocidade().setX(vmax);
            else if(pos.getVelocidade().getX()<-vmax)
                pos.getVelocidade().setX(-vmax);

            if(vx>0)
                dir=0;
            else
                dir=1;

            if(jogadores[0]==NULL||jogadores[1]==NULL)
            {
                if(jogadores[0]==NULL)
                {
                    alvo=jogadores[1];
                }
                else
                {
                    alvo=jogadores[0];
                }
            }
            else
            {
                float p0x = jogadores[0]->getPos().getEspaco().getX();
                float p1x = jogadores[1]->getPos().getEspaco().getX();

                if((x > p0x)&&(x > p1x))
                {
                    if(p0x>p1x)
                        alvo=jogadores[0];
                    else
                        alvo=jogadores[1];
                }
                else if((x > p0x && x < p1x)||(x < p0x && x > p1x))
                {
                    if(p0x<p1x)
                    {
                        if(dir == 1)
                            alvo=jogadores[0];
                        else
                            alvo=jogadores[1];
                    }
                    else
                    {
                        if(dir == 1)
                            alvo=jogadores[1];
                        else
                            alvo=jogadores[0];
                    }
                }
                else
                {
                    if(p0x>p1x)
                        alvo=jogadores[1];
                    else
                        alvo=jogadores[0];
                }
            }


            if(dir == 0 && vx<vmax)
                pos.getAceleracao().setX(0.1);
            else if(dir == 1 && vx>-vmax)
                pos.getAceleracao().setX(-0.1);
            else
                pos.getAceleracao().setX(0);



            if (abs(pos.getEspaco().getY() - (alvo->getPos().getEspaco().getY()-distY))<=2)
                pos.getVelocidade().setY(0);
            else
            {
                if(pos.getEspaco().getY() < alvo->getPos().getEspaco().getY()-distY)
                    pos.getVelocidade().setY(2);
                else if (pos.getEspaco().getY() > alvo->getPos().getEspaco().getY()-distY)
                    pos.getVelocidade().setY(-2);
            }


            if(x - mapX > 1000)
                pos.getAceleracao().setX(-0.1);
            else if((x - mapX < 200))
                pos.getAceleracao().setX(0.1);
        }
        else if(estado == MOR_PARADO)
        {

        }
        else if(estado == MOR_RASANTE_DESCENDO)
        {
            if(atirar)
            {
                audioEnt.tocaAudio("MORCEGO_RASANTE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                pos.getVelocidade()=velRasante;
                pos.getAceleracao().setY(-aceleracao);
                ang=0;
                atirar=false;
                if(pos.getVelocidade().getX() > 0)
                    desenhaFrame.setGiraRot(false);
                else
                    desenhaFrame.setGiraRot(true);
            }
            ang=atan(pos.getVelocidade().getY()/pos.getVelocidade().getX());
        }
    }
}
void Morcego::atualizaColisoes()
{

}
void Morcego::atualizaEstados()
{
    if(!alvo==NULL)
    {
        if(estado == MOR_ANDANDO&&al_get_timer_count(timerEnt[1])>delayRasante && !tomandoDano)
        {
            if(abs(alvo->getPos().getEspaco().getX() - pos.getEspaco().getX()) > 200 &&
                    alvo->getPos().getEspaco().getY() - pos.getEspaco().getY() > 100 &&
                    sqrt((alvo->getPos().getEspaco().getY() - pos.getEspaco().getY()) *
                    (alvo->getPos().getEspaco().getY() - pos.getEspaco().getY()) +
                    (alvo->getPos().getEspaco().getX() - pos.getEspaco().getX()) *
                    (alvo->getPos().getEspaco().getX() - pos.getEspaco().getX())) <400)
            {
                estado = MOR_PARADO;
                al_set_timer_count(timerEnt[1],0);
                delayRasante = 220 + rand()%80;
            }

        }
        else if(estado==MOR_PARADO && !tomandoDano)
        {
            if(!al_get_timer_started(timerEnt[0]))
            {
                al_start_timer(timerEnt[0]);
                pos.getVelocidade().setXY(0,0);
                pos.getAceleracao().setXY(0,0);

                delX = 2*(alvo->getPos().getEspaco().getX() - pos.getEspaco().getX());
                delY = alvo->getPos().getEspaco().getY() - pos.getEspaco().getY();
                velY=sqrt(2.0*aceleracao*delY);
                tempo=2.0*sqrt(2.0*delY/aceleracao);
                velX=delX/tempo;
                velRasante.setXY(velX,velY);
                yInic=pos.getEspaco().getY();
            }
            if(velX>0)
                dir=0;
            else
                dir=1;

            if(al_get_timer_count(timerEnt[0])>tempoCarrega)
            {
                estado=MOR_RASANTE_DESCENDO;
                al_stop_timer(timerEnt[0]);
                al_set_timer_count(timerEnt[0],0);
                atirar=true;

                tempoCarrega = 30 + rand()%50;
            }
        }
        else if(estado==MOR_RASANTE_DESCENDO && pos.getVelocidade().getY() < 0)
        {
            estado = MOR_RASANTE_SUBINDO;
            float ax;

            if (dir == 1)
                ax=(-vmax-velX)/(tempo/2);
            else
                ax=(vmax-velX)/(tempo/2);

            pos.getAceleracao().setX(ax);
            desenhaFrame.setGiraRot(false);
        }
        else if(estado == MOR_RASANTE_SUBINDO && pos.getEspaco().getY()+pos.getVelocidade().getY() <= yInic)
        {
            estado = MOR_ANDANDO;
            pos.getAceleracao().setXY(0,0);
            pos.getVelocidade().setY(0);
            pos.getEspaco().setY(yInic);
        }
        else if(tomandoDano && vida>0 && estado!=MOR_DANO)
        {
            estado = MOR_DANO;
            audioEnt.tocaAudio("MORCEGO_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);

            pos.getVelocidade().setX((pos.getEspaco().getX()-posDano.getX())/5);
            pos.getVelocidade().setY((pos.getEspaco().getY()-posDano.getY())/5);

            if(posVelo.getX()>0)
                if(pos.getVelocidade().getX()<0)
                    pos.getVelocidade().setX(-pos.getVelocidade().getX());

            if(posVelo.getX()<0)
                if(pos.getVelocidade().getX()>0)
                    pos.getVelocidade().setX(-pos.getVelocidade().getX());

            if(abs(pos.getVelocidade().getX())<2)
            {
                if(pos.getVelocidade().getX()>0)
                    pos.getVelocidade().setX(2);
                else if(pos.getVelocidade().getX()<0)
                    pos.getVelocidade().setX(-2);
            }



            velXDano=abs(pos.getVelocidade().getX())/5;
            velYDano=abs(pos.getVelocidade().getX())/5;
            pos.getAceleracao().setXY(0,0);



            desenhaFrame.setGiraRot(false);
        }
        else if(estado == MOR_DANO)
        {
            if((abs(pos.getVelocidade().getX())>velXDano) || (abs(pos.getVelocidade().getY())>velYDano))
            {
                pos.getVelocidade().setX(pos.getVelocidade().getX()*0.95);
                pos.getVelocidade().setY(pos.getVelocidade().getY()*0.95);
            }
            else
            {
                pos.getVelocidade().setXY(0,0);
                pos.getAceleracao().setXY(0,0);
                estado = MOR_ANDANDO;
                tomandoDano=false;
            }

        }
        else if(estado == MOR_MORRENDO)
        {
            if(permiteColisaoEnt)
            {
                permiteColisaoChao=true;
                permiteColisaoEnt = false;
            }
            if(!colisao[COLIDE_Y])
            {
                pos.getAceleracao().setXY(0,0.5);
            }
            else
            {
                estado = MOR_MORTO;
            }

        }
        if(estado == MOR_MORTO)
        {
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setXY(0,0);
            if((desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                    desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
                resetaEntidade();
        }
        if(pos.getEspaco().getY()>=860)
            resetaEntidade();
    }
}
void Morcego::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[MOR_PARADO][0]=0;
    estFrame[MOR_PARADO][1]=5;
    estFrame[MOR_PARADO][2]=1;
    estFrame[MOR_ANDANDO][0]=0;
    estFrame[MOR_ANDANDO][1]=5;
    estFrame[MOR_ANDANDO][2]=1;
    estFrame[MOR_RASANTE_DESCENDO][0]=2;
    estFrame[MOR_RASANTE_DESCENDO][1]=2;
    estFrame[MOR_RASANTE_DESCENDO][2]=1;
    estFrame[MOR_RASANTE_SUBINDO][0]=0;
    estFrame[MOR_RASANTE_SUBINDO][1]=5;
    estFrame[MOR_RASANTE_SUBINDO][2]=1;
    estFrame[MOR_DANO][0]=6;
    estFrame[MOR_DANO][1]=6;
    estFrame[MOR_DANO][2]=0;
    estFrame[MOR_MORRENDO][0]=7;
    estFrame[MOR_MORRENDO][1]=8;
    estFrame[MOR_MORRENDO][2]=1;
    estFrame[MOR_MORTO][0]=9;
    estFrame[MOR_MORTO][1]=9;
    estFrame[MOR_MORTO][2]=0;

    for(i=0; i<5; i++)
    {
        tFrame[MOR_PARADO].inclui_frente(2);
        tFrame[MOR_ANDANDO].inclui_frente(5);
        tFrame[MOR_RASANTE_SUBINDO].inclui_frente(2);
    }
    tFrame[MOR_RASANTE_DESCENDO].inclui_frente(1);
    tFrame[MOR_DANO].inclui_frente(60);
    tFrame[MOR_MORRENDO].inclui_frente(5);
    tFrame[MOR_MORTO].inclui_frente(30);
}

void Morcego::criaEfeitos()
{

}

void Morcego::inicializaEntidade()
{

}
void Morcego::resetaEntidade()
{

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0.5, 0);
    pos.getAceleracao().setXY(0, 0);

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    atirar=false;
    funcionando=false;
    tomandoDano=false;
    permiteColisaoEnt=true;

    desenhaFrame.setGiraRot(false);

    estado=MOR_ANDANDO;
    desenhaFrame.setEstado(MOR_ANDANDO);
    desenhaFrame.atualizaFrame();

    al_stop_timer(timerEnt[0]);
    al_stop_timer(timerEnt[1]);
    al_set_timer_count(timerEnt[0],0);
    al_set_timer_count(timerEnt[1],0);

}
void Morcego::setFuncionando(bool f)
{
    funcionando=f;
    al_start_timer(timerEnt[1]);
}
void Morcego::paraTimer()
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

