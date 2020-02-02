#include "Fogo_Fatuo.h"
#include "Lista_Entidades.h"

Fogo_Fatuo::Fogo_Fatuo()
{

}
Fogo_Fatuo::Fogo_Fatuo(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 2800;
    vidaRand = 280;
    danoBase = 800;
    danoRand = 80;
    score = 250;
    /**PARAMETROS DO INIMMIGO**/

    tipo=1;
    corNormal = al_map_rgba(255,50,50,200);
    corDano = al_map_rgba(255,50,50,100);

    tempoInvoca=180+rand()%121;

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    alvo=NULL;

    borda.setXY(20, 20);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Fogo_Fatuo.png");

    desenhaFrame.setNumEstados(4);

    permiteColisaoChao=true;

    funcionando=false;
    tomandoDano=false;
    colidePlat=false;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;
    delX=0;
    delY=0;
    denominador=0;

    vRand = (rand()%1001)/1000.0;
    vXMax=1.2+vRand;
    velXDano=0;
    velYDano=0;

    construiFrames();

    corEnt = corNormal;


    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(FOG_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);

    string nome[] = {"FOGO_DANO", "FOGO_INVOCAR", "FOGO_MORTE"};
    audioEnt.inicializaAudio(nome, 3);
}
Fogo_Fatuo::Fogo_Fatuo(int ix,int iy, bool t)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 1400;
    vidaRand = 140;
    danoBase = 800;
    danoRand = 80;
    score = 100;
    /**PARAMETROS DO INIMMIGO**/
    tipo=t;
    corNormal = al_map_rgba(255,75,75,100);
    corDano = al_map_rgba(255,75,75,25);

    tempoInvoca=0;

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    alvo=NULL;

    permiteColisaoChao=true;

    borda.setXY(20, 20);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Fogo_Fatuo.png");

    desenhaFrame.setNumEstados(4);

    funcionando=false;
    tomandoDano=false;
    colidePlat=false;

    vida=20;
    dano=12;
    dir=0;
    delX=0;
    delY=0;
    denominador=0;

    vRand = (rand()%1001)/1000.0;
    vXMax=0.7+vRand;
    velXDano=0;
    velYDano=0;

    construiFrames();

    corEnt = corNormal;

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(FOG_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);

    string nome[] = {"FOGO_DANO", "FOGO_INVOCAR", "FOGO_MORTE"};
    audioEnt.inicializaAudio(nome, 3);
}
Fogo_Fatuo::~Fogo_Fatuo()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete [] timerEnt;
}
void Fogo_Fatuo::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 46), dir, false, corEnt);
    }
}
void Fogo_Fatuo::atualiza()
{
    if(funcionando)
    {
        estado = desenhaFrame.getEstado();

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();

        if(tipo)
            pos.mover();

        if((tipo==0 && al_get_timer_count(timerEnt[0])>180))
            pos.mover();

        if(tipo==0 && al_get_timer_started(timerEnt[0]) && al_get_timer_count(timerEnt[0])>180)
            al_stop_timer(timerEnt[0]);

        if (tomandoDano)
            permiteColisaoEnt = false;
        else
            permiteColisaoEnt = true;

        if(vida <= 0 && estado!=FOG_MORRENDO)
        {
            estado=FOG_MORRENDO;
            if(!audioEnt.audioTocando("FOGO_MORTE"))
                audioEnt.tocaAudio("FOGO_MORTE", 1,ALLEGRO_PLAYMODE_ONCE,0);
        }

        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();
        audioEnt.atualizaAudio();
    }
}
void Fogo_Fatuo::atualizaComportamento()
{
    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {
        if(estado == FOG_DANO)
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
                estado = FOG_ANDANDO;
                tomandoDano=false;
                corEnt=corNormal;
            }
        }
        else if(estado == FOG_MORRENDO)
        {
            pos.getVelocidade().setX(pos.getVelocidade().getX()*0.95);
            pos.getVelocidade().setY(pos.getVelocidade().getY()*0.95);

            if((desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                    desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
            {
                mudaScore(score);
                resetaEntidade();
            }

        }
        else if(estado == FOG_PARADO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setY(0);
            pos.getVelocidade().setY(0);
        }
        else
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

            delX=alvo->getPos().getEspaco().getX() - pos.getEspaco().getX();
            delY=alvo->getPos().getEspaco().getY() - pos.getEspaco().getY();

            if(abs(delX)>abs(delY))
                denominador=abs(delX);
            else
                denominador=abs(delY);

            if(denominador!=0)
            {
                pos.getVelocidade().setX(vXMax*delX/denominador);
                pos.getVelocidade().setY(vXMax*delY/denominador);
            }
            else
            {
                pos.getVelocidade().setX(0);
                pos.getVelocidade().setY(0);
            }
            if(abs(pos.getAceleracao().getX())>0.5)
            {
                if(pos.getAceleracao().getX()>0)
                    pos.getAceleracao().setX(pos.getAceleracao().getX()-0.2);
                else
                    pos.getAceleracao().setX(pos.getAceleracao().getX()+0.2);
            }
            else
                pos.getAceleracao().setX(0);
            if(abs(pos.getAceleracao().getY())>0.5)
            {
                if(pos.getAceleracao().getY()>0)
                    pos.getAceleracao().setY(pos.getAceleracao().getY()-0.2);
                else
                    pos.getAceleracao().setY(pos.getAceleracao().getY()+0.2);
            }
            else
                pos.getAceleracao().setY(0);
        }
    }
}
void Fogo_Fatuo::atualizaColisoes()
{

}
void Fogo_Fatuo::atualizaEstados()
{
    if(tomandoDano && !(estado==FOG_DANO) && !(estado==FOG_MORRENDO))
    {
        if(!audioEnt.audioTocando("FOGO_DANO"))
            audioEnt.tocaAudio("FOGO_DANO",1,ALLEGRO_PLAYMODE_ONCE,0);
        estado = FOG_DANO;
        corEnt=corDano;
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
    }
}
void Fogo_Fatuo::criaEfeitos()
{

}
void Fogo_Fatuo::inicializaEntidade()
{
    if(tipo)
    {
        for (int i = 0; i < 4; i++)
        {
            fogos[i] = new Fogo_Fatuo(-200,-200,false);
            fogos[i]->setJogador(jogadores[0],0);
            fogos[i]->setJogador(jogadores[1],1);
            fogos[i]->setMapaAtual(mapaAtual);
            fogos[i]->setListaEnt(listEnt);
            listEnt->incluiEntidade(static_cast<Entidade*> (fogos[i]));
        }
    }
}
void Fogo_Fatuo::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;

    alvo=NULL;
    if(tipo)
        vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    else
        vida = (vidaBase + rand()%(vidaRand*2 + 1) - vidaRand)/3;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    corEnt=corNormal;
    estado=FOG_ANDANDO;
    desenhaFrame.setEstado(FOG_ANDANDO);
    desenhaFrame.atualizaFrame();
    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0],0);

}
void Fogo_Fatuo::criaProjetil()
{

    if(tipo)
    {
        if(al_get_timer_count(timerEnt[0])>tempoInvoca)
        {
            for(int i=0; i<4; i++)
            {
                if(!fogos[i]->getFuncionando())
                {
                    if(!audioEnt.audioTocando("FOGO_INVOCAR"))
                        audioEnt.tocaAudio("FOGO_INVOCAR",1,ALLEGRO_PLAYMODE_ONCE,0);
                    fogos[i]->setFuncionando(true);
                    fogos[i]->getPos().setEspaco(pos.getEspaco());
                    al_set_timer_count(timerEnt[0],0);
                    tempoInvoca=180+rand()%121;
                    break;
                }
            }
        }
    }
}
void Fogo_Fatuo::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[FOG_PARADO][0]=0;
    estFrame[FOG_PARADO][1]=12;
    estFrame[FOG_PARADO][2]=1;
    estFrame[FOG_ANDANDO][0]=0;
    estFrame[FOG_ANDANDO][1]=12;
    estFrame[FOG_ANDANDO][2]=1;
    estFrame[FOG_DANO][0]=0;
    estFrame[FOG_DANO][1]=12;
    estFrame[FOG_DANO][2]=1;
    estFrame[FOG_MORRENDO][0]=13;
    estFrame[FOG_MORRENDO][1]=18;
    estFrame[FOG_MORRENDO][2]=0;

    for(i=0; i<13; i++)
    {
        tFrame[FOG_PARADO].inclui_frente(10);
        tFrame[FOG_ANDANDO].inclui_frente(10);
        tFrame[FOG_DANO].inclui_frente(10);
        if(i<6)
            tFrame[FOG_MORRENDO].inclui_frente(10);
    }
}
void Fogo_Fatuo::setFuncionando(bool f)
{
    funcionando=f;
    al_start_timer(timerEnt[0]);
}
void Fogo_Fatuo::paraTimer()
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
