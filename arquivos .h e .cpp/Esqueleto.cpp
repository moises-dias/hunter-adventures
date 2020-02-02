#include "Esqueleto.h"
#include "Osso_Aereo.h"
#include "Ataque_Generico.h"
#include "Lista_Entidades.h"

Esqueleto::Esqueleto()
{

}
Esqueleto::Esqueleto(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 4200;
    vidaRand = 420;
    danoBase = 1000;
    danoRand = 100;
    danoGolpe = 1500;
    danoGolpeRand=150;
    score = 300;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(20, 40);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Esqueleto.png");

    desenhaFrame.setNumEstados(9);


    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;
    tipo=0;

    aceleracao = 0.5;

    atirar=false;
    tomandoDano=false;
    funcionando=false;
    perto=false;


    vRand = (rand()%1001)/1000.0;
    vXMax=1+vRand;
    yPlat=0;
    delXPulo=150+rand()%150;


    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(ESQ_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [2];
    timerEnt[0] = al_create_timer(1/60.0);
    timerEnt[1] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);

    string nome[] = {"ESQUELETO_DANO", "ESQUELETO_GOLPE", "ESQUELETO_JOGAR", "ESQUELETO_MORTE"};
    audioEnt.inicializaAudio(nome, 4);
}
Esqueleto::~Esqueleto()
{
    for (int i = 0; i < 2; i++)
        al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete[]timerEnt;
}
void Esqueleto::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[ESQ_PARADO][0]=0;
    estFrame[ESQ_PARADO][1]=0;
    estFrame[ESQ_PARADO][2]=1;
    estFrame[ESQ_ANDANDO][0]=1;
    estFrame[ESQ_ANDANDO][1]=4;
    estFrame[ESQ_ANDANDO][2]=1;
    estFrame[ESQ_ATAQUE_LONGO][0]=11;
    estFrame[ESQ_ATAQUE_LONGO][1]=11;
    estFrame[ESQ_ATAQUE_LONGO][2]=1;
    estFrame[ESQ_ATAQUE_PERTO][0]=6;
    estFrame[ESQ_ATAQUE_PERTO][1]=10;
    estFrame[ESQ_ATAQUE_PERTO][2]=1;
    estFrame[ESQ_DANO][0]=13;
    estFrame[ESQ_DANO][1]=13;
    estFrame[ESQ_DANO][2]=1;
    estFrame[ESQ_MORRENDO][0]=14;
    estFrame[ESQ_MORRENDO][1]=22;
    estFrame[ESQ_MORRENDO][2]=0;
    estFrame[ESQ_RENASCENDO][0]=23;
    estFrame[ESQ_RENASCENDO][1]=30;
    estFrame[ESQ_RENASCENDO][2]=0;


    estFrame[ESQ_PULANDO][0]=1;
    estFrame[ESQ_PULANDO][1]=1;
    estFrame[ESQ_PULANDO][2]=1;

    estFrame[ESQ_CAINDO][0]=1;
    estFrame[ESQ_CAINDO][1]=1;
    estFrame[ESQ_CAINDO][2]=1;

    tFrame[ESQ_PARADO].inclui_frente(10);

    tFrame[ESQ_PULANDO].inclui_frente(10);

    tFrame[ESQ_CAINDO].inclui_frente(10);

    for(i=0; i<4; i++)
        tFrame[ESQ_ANDANDO].inclui_frente(10);

    tFrame[ESQ_ATAQUE_PERTO].inclui_frente(6);
    tFrame[ESQ_ATAQUE_PERTO].inclui_frente(5);
    tFrame[ESQ_ATAQUE_PERTO].inclui_frente(5);
    tFrame[ESQ_ATAQUE_PERTO].inclui_frente(5);
    tFrame[ESQ_ATAQUE_PERTO].inclui_frente(10);

    tFrame[ESQ_ATAQUE_LONGO].inclui_frente(1000);

    tFrame[ESQ_DANO].inclui_frente(30);

    for(i=0; i<9; i++)
    {
        tFrame[ESQ_MORRENDO].inclui_frente(5);
        if(i<7)
            tFrame[ESQ_RENASCENDO].inclui_frente(10);
    }
    tFrame[ESQ_RENASCENDO].inclui_frente(30);

}
void Esqueleto::desenha()
{
    if(funcionando && (estado!=ESQ_RENASCENDO || colisao[COLIDE_Y]))
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 65), dir, false, corEnt);
    }
}
void Esqueleto::atualiza()
{

    if(funcionando)
    {
        estado = desenhaFrame.getEstado();
        if (colisao[COLIDE_Y])
        {
            pos.getVelocidade().setY(0);
            pos.getAceleracao().setY(0);
        }
        else
            pos.getAceleracao().setY(1);

        if(estado!=ESQ_RENASCENDO || colisao[COLIDE_Y])
        {
            atualizaComportamento();
            atualizaColisoes();
            atualizaEstados();
            criaProjetil();
        }
        if(estado == ESQ_RENASCENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            permiteColisaoEnt=false;
        }
        else if(estado!=ESQ_MORRENDO && !permiteColisaoEnt)
            permiteColisaoEnt=true;
        pos.mover();

        if(vida <= 0 && estado!=ESQ_MORRENDO)
        {
            estado=ESQ_MORRENDO;
                audioEnt.tocaAudio("ESQUELETO_MORTE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }


        if(estado!=ESQ_RENASCENDO || colisao[COLIDE_Y])
        {
            desenhaFrame.setEstado(estado);
            desenhaFrame.atualizaFrame();
        }
        audioEnt.atualizaAudio();
    }
}
void Esqueleto::criaProjetil()
{
    if((estado==ESQ_ATAQUE_LONGO)&&atirar)
    {
        float velX,delX,velY,delYSubir,delYDescer,tempo;

        delX = alvo->getPos().getEspaco().getX() - pos.getEspaco().getX();

        if(pos.getEspaco().getY() <= alvo->getPos().getEspaco().getY())
        {
            delYSubir=200+rand()%200;
            delYDescer=delYSubir;
        }
        else
        {
            delYSubir=pos.getEspaco().getY() - alvo->getPos().getEspaco().getY() + 200 + rand()%200;
            delYDescer=delYSubir-(pos.getEspaco().getY() - alvo->getPos().getEspaco().getY());
        }
        velY=-sqrt(2.0*aceleracao*delYSubir);

        tempo=sqrt(2.0*abs(delYSubir)/aceleracao)+sqrt(2.0*abs(delYDescer)/aceleracao);
        velX=delX/tempo;

        Vetor_R2 velOsso;
        velOsso.setXY(velX,velY);

        for(int i=0; i<3; i++)
        {
            if(!(ossos[i]->getFuncionando()))
            {
                        audioEnt.tocaAudio("ESQUELETO_JOGAR", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                ossos[i]->setFuncionando(true);
                ossos[i]->getPos().setEspaco(pos.getEspaco());
                ossos[i]->getPos().setVelocidade(velOsso);
                ossos[i]->getPos().getAceleracao().setXY(0,aceleracao);
                break;
            }
        }
        atirar=false;
    }
    if((estado==ESQ_ATAQUE_PERTO) && desenhaFrame.getFrameAtual() == 8 && desenhaFrame.getTempoEstado() == 1)
    {
        for(int i=0; i<2; i++)
        {
            if(!(atkOsso[i]->getFuncionando()))
            {
                audioEnt.tocaAudio("ESQUELETO_GOLPE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                atkOsso[i]->setFuncionando(true);
                if(dir)
                    atkOsso[i]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-40,0)));
                else
                    atkOsso[i]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(40,0)));
                atkOsso[i]->setDano(danoGolpe + rand()%(danoGolpeRand*2 + 1) - danoGolpeRand);
                break;
            }
        }
    }
}
void Esqueleto::atualizaComportamento()
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
                pos.getEspaco().getX()+borda.getX() +20 > alvo->getPos().getEspaco().getX()-alvo->getBorda().getX()) ||
                (pos.getEspaco().getX() > alvo->getPos().getEspaco().getX()&&
                 pos.getEspaco().getX()-borda.getX() -20 < alvo->getPos().getEspaco().getX()+alvo->getBorda().getX()))&&
                (abs((alvo->getPos().getEspaco().getY())-(pos.getEspaco().getY()))<70))
            perto=true;
        else
            perto=false;

        if(estado == ESQ_RENASCENDO)
        {
            permiteColisaoEnt=false;
        }
        else
        {
            permiteColisaoEnt=true;
        }
        if(estado==ESQ_ATAQUE_LONGO||estado==ESQ_ATAQUE_PERTO||estado==ESQ_MORRENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
        }
        else if(estado==ESQ_DANO)
        {
            pos.getAceleracao().setX(0);
            if(colisao[COLIDE_Y])
            {
                estado=ESQ_ANDANDO;
                tomandoDano=false;
            }
        }
        else if(estado == ESQ_PULANDO || estado == ESQ_CAINDO)
        {
            pos.getAceleracao().setX(0);
            if(estado == ESQ_CAINDO && pos.getEspaco().getY()-getBorda().getY() > yPlat)
                permiteColisaoChao=true;
            if(colisao[COLIDE_Y])
            {
                estado=ESQ_ANDANDO;
                al_set_timer_count(timerEnt[1],0);
            }


        }
        else if(estado!=ESQ_RENASCENDO)
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

        if(perto && estado!=ESQ_DANO && estado!=ESQ_MORRENDO &&
                estado!=ESQ_CAINDO && estado!=ESQ_PULANDO && estado!=ESQ_RENASCENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            if(estado!=ESQ_ATAQUE_PERTO)
                estado=ESQ_ATAQUE_PERTO;
        }
    }
}
void Esqueleto::atualizaColisoes()
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
void Esqueleto::atualizaEstados()
{
    if(jogadores[0]==NULL&&jogadores[1]==NULL)
    {
        estado=ESQ_PARADO;
        al_stop_timer(timerEnt[0]);
    }
    else if((colisao[COLIDE_XD] || colisao[COLIDE_XE]) && estado==ESQ_ANDANDO)
    {
        estado=ESQ_PARADO;
        al_set_timer_count(timerEnt[0],0);
    }
    else if(estado == ESQ_PARADO && !(colisao[COLIDE_XD] || colisao[COLIDE_XE]))
    {
        estado=ESQ_ANDANDO;
    }
    else if((estado==ESQ_RENASCENDO)&&(desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        estado=ESQ_ANDANDO;
        al_set_timer_count(timerEnt[0],0);
        al_set_timer_count(timerEnt[1],0);
    }
    else if(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<delXPulo &&
            al_get_timer_count(timerEnt[1])>120 && estado!=ESQ_PULANDO && estado!=ESQ_CAINDO && !tomandoDano &&
            colisao[COLIDE_Y] && alvo->getPos().getAceleracao().getY()==0 && ((rand()%1000)/990) &&
            (abs(getBorda().getY())+abs(alvo->getBorda().getY())) <
            abs(pos.getEspaco().getY()-alvo->getPos().getEspaco().getY()))
    {
        if(pos.getEspaco().getY() > alvo->getPos().getEspaco().getY())
        {
            estado=ESQ_PULANDO;
            pos.getVelocidade().setY(-sqrt(2*(pos.getEspaco().getY() - alvo->getPos().getEspaco().getY() + 50)));
        }
        if(pos.getEspaco().getY() < alvo->getPos().getEspaco().getY())
        {
            estado=ESQ_CAINDO;
            permiteColisaoChao=false;
            yPlat=pos.getEspaco().getY()+getBorda().getY()+10;
        }

    }
    else if(perto && estado==ESQ_ANDANDO)
    {
        estado=ESQ_ATAQUE_PERTO;
    }
    else if((estado==ESQ_ANDANDO&&(al_get_timer_count(timerEnt[0])>120)&&
             abs((alvo->getPos().getEspaco().getX())-(pos.getEspaco().getX()))>250 &&
             abs((alvo->getPos().getEspaco().getX())-(pos.getEspaco().getX()))<500))
    {
        estado=ESQ_ATAQUE_LONGO;
        atirar=true;

    }
    else if((estado==ESQ_ATAQUE_LONGO)&&(desenhaFrame.getTempoEstado()>30))
    {
        estado=ESQ_ANDANDO;
        al_set_timer_count(timerEnt[0],0);
    }
    else if((estado==ESQ_ATAQUE_PERTO)&&(desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        if(!(perto && estado!=ESQ_DANO && estado!=ESQ_MORRENDO))
            estado=ESQ_ANDANDO;
    }
    else if(tomandoDano && estado!=ESQ_MORRENDO && estado!=ESQ_DANO && vida>0)
    {
        estado=ESQ_DANO;
        audioEnt.tocaAudio("ESQUELETO_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);

        if(posVelo.getX()>0)
            pos.getVelocidade().setXY(4,-10);

        if(posVelo.getX()<0)
            pos.getVelocidade().setXY(-4,-10);

        if(posVelo.getX()==0)
            pos.getVelocidade().setXY(0,-10);
    }
    else if((estado==ESQ_MORRENDO && (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]]) ||
            pos.getEspaco().getY()>=860)
    {
        if(vida<=0)
            mudaScore(score);
        resetaEntidade();
        permiteColisaoEnt=true;
    }
    if(estado == ESQ_MORRENDO)
    {
        permiteColisaoEnt=false;
    }
}
void Esqueleto::inicializaEntidade()
{
    for (int i = 0; i < 3; i++)
    {
        ossos[i] = new Osso_Aereo(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(0,0), 0);
        ossos[i]->setMapaAtual(mapaAtual);
        ossos[i]->setPtrJ(NULL);
        ossos[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (ossos[i]));
    }
    for (int i = 0; i < 2; i++)
    {
        atkOsso[i] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(20,20),20,20);
        atkOsso[i]->setMapaAtual(mapaAtual);
        atkOsso[i]->setPtrJ(NULL);
        atkOsso[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atkOsso[i]));
    }
}
void Esqueleto::criaEfeitos()
{

}
void Esqueleto::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    funcionando=false;
    tomandoDano=false;
    atirar=false;
    perto=false;
    permiteColisaoChao=true;

    alvo=NULL;

    if(tipo)
    {
        estado=ESQ_RENASCENDO;
        desenhaFrame.setEstado(ESQ_RENASCENDO);
    }
    else
    {
        estado = ESQ_ANDANDO;
        desenhaFrame.setEstado(ESQ_ANDANDO);
    }

    desenhaFrame.atualizaFrame();

    al_stop_timer(timerEnt[0]);
    al_stop_timer(timerEnt[1]);
    al_set_timer_count(timerEnt[0],0);
    al_set_timer_count(timerEnt[1],0);
}
void Esqueleto::setFuncionando(bool f)
{
    funcionando = f;
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);
}
void Esqueleto::setDir(float d)
{
    dir=d;
}
void Esqueleto::setTipo(bool t)
{
    tipo=t;
    desenhaFrame.setEstado(ESQ_RENASCENDO);
    desenhaFrame.atualizaFrame();
    for(int i=0;i<3;i++)
        ossos[i]->setCorEnt(al_map_rgba(180,0,0,255));
}
void Esqueleto::paraTimer()
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
