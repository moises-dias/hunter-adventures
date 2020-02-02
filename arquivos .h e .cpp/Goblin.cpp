#include "Goblin.h"
#include "Ataque_Generico.h"
#include "Lista_Entidades.h"

Goblin::Goblin()
{

}
Goblin::Goblin(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 2800;
    vidaRand = 280;
    danoBase = 800;
    danoRand = 80;
    danoGolpe = 1500;
    danoGolpeRand = 150;
    score = 200;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);
    pos.setPlatAtual(0);

    borda.setXY(15, 30);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Goblin.png");

    desenhaFrame.setNumEstados(7);

    yPlat=0;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;

    delXPulo= 150 + rand()%150;

    tomandoDano=false;
    funcionando=false;
    perto=false;

    vRand = (rand()%1001)/1000.0;
    vXMax=2+vRand;

    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(GOB_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[0]);

    string nome[] = {"GOBLIN_ATAQUE","GOBLIN_DANO","GOBLIN_MORRENDO"};
    audioEnt.inicializaAudio(nome, 3);
}
Goblin::~Goblin()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete [] timerEnt;
}

void Goblin::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 55), dir, false, corEnt);
    }
}
void Goblin::atualiza()
{
    if(funcionando)
    {
        if(tomandoDano)
            permiteColisaoEnt=false;
        else
            permiteColisaoEnt=true;

        estado = desenhaFrame.getEstado();
        if (colisao[COLIDE_Y])
        {
            pos.getVelocidade().setY(0);
            pos.getAceleracao().setY(0);
        }
        else
            pos.getAceleracao().setY(1);

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();

        pos.mover();

        if(vida <= 0 && estado!=GOB_MORRENDO)
        {
            if (!audioEnt.audioTocando("GOBLIN_MORRENDO"))
            {
                audioEnt.tocaAudio("GOBLIN_MORRENDO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                mudaScore(score);
            }

             estado=GOB_MORRENDO;
        }


        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();
    }
}
void Goblin::criaProjetil()
{
    if((estado==GOB_ATACANDO) && desenhaFrame.getFrameAtual() == 10 && desenhaFrame.getTempoEstado() == 1)
    {
        for(int i=0; i<2; i++)
        {
            if(!(atkGoblin[i]->getFuncionando()))
            {
                atkGoblin[i]->setFuncionando(true);
                if(dir)
                    atkGoblin[i]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-20,0)));
                else
                    atkGoblin[i]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(20,0)));

                atkGoblin[i]->setDano(danoGolpe + rand()%(danoGolpeRand*2 + 1) - danoGolpeRand);
                if (!audioEnt.audioTocando("GOBLIN_ATAQUE"))
                        audioEnt.tocaAudio("GOBLIN_ATAQUE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
        }
    }
}
void Goblin::atualizaComportamento()
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

        if(((pos.getEspaco().getX() < alvo->getPos().getEspaco().getX()&&
                pos.getEspaco().getX()+borda.getX() +5 > alvo->getPos().getEspaco().getX()-alvo->getBorda().getX()) ||
                (pos.getEspaco().getX() > alvo->getPos().getEspaco().getX()&&
                 pos.getEspaco().getX()-borda.getX() -5 < alvo->getPos().getEspaco().getX()+alvo->getBorda().getX()))&&
                (abs((alvo->getPos().getEspaco().getY())-(pos.getEspaco().getY()))<70))
            perto=true;
        else
            perto=false;


        if(estado==GOB_ATACANDO||estado==GOB_MORRENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
        }
        else if(estado==GOB_DANO)
        {
            pos.getAceleracao().setX(0);
            if(colisao[COLIDE_Y])
            {
                estado=GOB_ANDANDO;
                tomandoDano=false;
            }
        }
        else if(estado == GOB_PULANDO || estado == GOB_CAINDO)
        {
            pos.getAceleracao().setX(0);
            if(estado == GOB_CAINDO && pos.getEspaco().getY()-getBorda().getY() > yPlat)
                permiteColisaoChao=true;
            if(colisao[COLIDE_Y])
            {
                estado=GOB_ANDANDO;
                al_set_timer_count(timerEnt[0],0);
            }


        }
        else
        {
            float alvoX=alvo->getPos().getEspaco().getX();

            if(alvoX<x)
            {
                dir=1;
                if(vx> - (vXMax) )
                    pos.getAceleracao().setX(-0.3);
                else if(vx<=-(vXMax))
                {
                    pos.getAceleracao().setX(0);
                    pos.getVelocidade().setX(-(vXMax));
                }
            }
            else
            {
                dir=0;
                if(vx<(vXMax))
                    pos.getAceleracao().setX(0.3);
                else if(vx>=(vXMax))
                {
                    pos.getAceleracao().setX(0);
                    pos.getVelocidade().setX((vXMax));
                }
            }
        }

        if(perto && estado!=GOB_DANO && estado!=GOB_MORRENDO && estado!=GOB_PULANDO && estado!=GOB_CAINDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            if(estado!=GOB_ATACANDO)
                estado=GOB_ATACANDO;
        }
    }
}
void Goblin::atualizaColisoes()
{
    if (colisao[COLIDE_XD] && pos.getVelocidade().getX() >0)
    {
        pos.getVelocidade().setX(0);
    }
    if(colisao[COLIDE_XE] && pos.getVelocidade().getX() <0)
    {
        pos.getVelocidade().setX(0);
    }
}
void Goblin::atualizaEstados()
{
    if(jogadores[0]==NULL&&jogadores[1]==NULL)
    {
        estado=GOB_PARADO;
    }
    else if((colisao[COLIDE_XD] || colisao[COLIDE_XE]) && estado == GOB_ANDANDO)
    {
        estado=GOB_PARADO;
    }
    else if(estado == GOB_PARADO && !(colisao[COLIDE_XD] || colisao[COLIDE_XE]))
    {
        estado=GOB_ANDANDO;
    }
    else if(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<delXPulo &&
            al_get_timer_count(timerEnt[0])>120 && estado!=GOB_PULANDO && estado!=GOB_CAINDO && !tomandoDano &&
            colisao[COLIDE_Y] && alvo->getPos().getAceleracao().getY()==0 && ((rand()%1000)/990) &&
            (abs(getBorda().getY())+abs(alvo->getBorda().getY())) <
            (abs(pos.getEspaco().getY()-alvo->getPos().getEspaco().getY())))
    {
        if(pos.getEspaco().getY() > alvo->getPos().getEspaco().getY())
        {
            estado=GOB_PULANDO;
            pos.getVelocidade().setY(-sqrt(2*(pos.getEspaco().getY() - alvo->getPos().getEspaco().getY() + 50)));
        }
        if((pos.getEspaco().getY() < alvo->getPos().getEspaco().getY()) &&
           mapaAtual->getPlat(pos.getPlatAtual()).getTipo()==1)
        {
            estado=GOB_CAINDO;
            permiteColisaoChao=false;
            yPlat=pos.getEspaco().getY()+getBorda().getY()+10;
        }

    }
    else if(perto && estado==GOB_ANDANDO)
    {
        estado=GOB_ATACANDO;
    }
    else if((estado==GOB_ATACANDO)&&(desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        if(!(perto && estado!=GOB_DANO && estado!=GOB_MORRENDO))
            estado=GOB_ANDANDO;
    }
    else if(tomandoDano && estado!=GOB_MORRENDO && estado!=GOB_DANO && vida>0)
    {
        estado=GOB_DANO;

        if (!audioEnt.audioTocando("GOBLIN_DANO"))
                        audioEnt.tocaAudio("GOBLIN_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);

        if(posVelo.getX()>0)
            pos.getVelocidade().setXY(4,-10);

        if(posVelo.getX()<0)
            pos.getVelocidade().setXY(-4,-10);

        if(posVelo.getX()==0)
            pos.getVelocidade().setXY(0,-10);
    }
    else if(estado==GOB_MORRENDO && (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        resetaEntidade();
    }
}
void Goblin::criaEfeitos()
{

}

void Goblin::inicializaEntidade()
{
    for (int i = 0; i < 2; i++)
    {
        atkGoblin[i] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(20,20),20,20);
        atkGoblin[i]->setMapaAtual(mapaAtual);
        atkGoblin[i]->setPtrJ(NULL);
        atkGoblin[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atkGoblin[i]));
    }
}
void Goblin::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    funcionando=false;
    tomandoDano=false;
    perto=false;
    permiteColisaoEnt=true;

    alvo=NULL;

    estado=GOB_ANDANDO;
    desenhaFrame.setEstado(GOB_ANDANDO);
    desenhaFrame.atualizaFrame();
}
void Goblin::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[GOB_PARADO][0]=0;
    estFrame[GOB_PARADO][1]=0;
    estFrame[GOB_PARADO][2]=1;
    estFrame[GOB_ANDANDO][0]=1;
    estFrame[GOB_ANDANDO][1]=4;
    estFrame[GOB_ANDANDO][2]=1;
    estFrame[GOB_ATACANDO][0]=5;
    estFrame[GOB_ATACANDO][1]=14;
    estFrame[GOB_ATACANDO][2]=1;
    estFrame[GOB_DANO][0]=15;
    estFrame[GOB_DANO][1]=15;
    estFrame[GOB_DANO][2]=1;
    estFrame[GOB_MORRENDO][0]=15;
    estFrame[GOB_MORRENDO][1]=17;
    estFrame[GOB_MORRENDO][2]=0;
    estFrame[GOB_PULANDO][0]=3;
    estFrame[GOB_PULANDO][1]=3;
    estFrame[GOB_PULANDO][2]=1;
    estFrame[GOB_CAINDO][0]=3;
    estFrame[GOB_CAINDO][1]=3;
    estFrame[GOB_CAINDO][2]=1;

    tFrame[GOB_PARADO].inclui_frente(5);
    for(i=0; i<5; i++)
        tFrame[GOB_ANDANDO].inclui_frente(10);

    tFrame[GOB_MORRENDO].inclui_frente(20);
    tFrame[GOB_MORRENDO].inclui_frente(20);
    tFrame[GOB_MORRENDO].inclui_frente(40);

    tFrame[GOB_DANO].inclui_frente(5);

    tFrame[GOB_PULANDO].inclui_frente(5);

    tFrame[GOB_CAINDO].inclui_frente(5);

    tFrame[GOB_ATACANDO].inclui_frente(5);
    tFrame[GOB_ATACANDO].inclui_frente(5);
    tFrame[GOB_ATACANDO].inclui_frente(3);
    tFrame[GOB_ATACANDO].inclui_frente(3);
    tFrame[GOB_ATACANDO].inclui_frente(3);
    tFrame[GOB_ATACANDO].inclui_frente(4);
    tFrame[GOB_ATACANDO].inclui_frente(4);
    tFrame[GOB_ATACANDO].inclui_frente(4);
    tFrame[GOB_ATACANDO].inclui_frente(3);
    tFrame[GOB_ATACANDO].inclui_frente(3);
}
void Goblin::paraTimer()
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
