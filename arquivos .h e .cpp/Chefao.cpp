#include "Chefao.h"
#include "Chuva_Fogo.h"
#include "Ataque_Generico.h"
#include "Onda_Fogo.h"
#include "Lista_Entidades.h"
#include "Coluna_Fogo.h"
#include "Rastro.h"


Chefao::Chefao()
{

}
Chefao::Chefao(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 28000;
    vidaRand = 0;
    danoBase = 1200;
    danoRand = 120;
    danoGolpe = 3000;
    danoGolpeRand = 300;
    danoDash=1100;
    danoRandDash=110;
    score = 5000;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    alvo=NULL;

    borda.setXY(32, 62);

    desenhaFrame.setSpriteX(200);
    desenhaFrame.setSpriteY(200);
    desenhaFrame.setNomeImg("Chefao.png");

    desenhaFrame.setNumEstados(8);

    permiteColisaoChao=false;
    permiteColisaoEnt=false;

    funcionando=false;
    tomandoDano=false;
    teleport=false;
    teleportConj=false;
    iniciaDash=false;
    perto=false;
    colidePlat=false;
    fade=255;
    especiaisChefao=0;
    iniciarEspecial=false;
    tempoDash=180;

    corEnt=al_map_rgba(fade,fade,fade,fade);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;

    dirDestino=0;
    destino=0;
    destinoY=0;

    delX=0;
    delY=0;
    denominador=0;



    vXMax=3;

    velXDano=0;
    velYDano=0;

    construiFrames();

    desenhaFrame.setCoef(1.5);
    desenhaFrame.setEstado(CHE_VOANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [5];
    timerEnt[0] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[0]);

    timerEnt[1] = al_create_timer(1/60.0);

    timerEnt[2] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[2]);

    timerEnt[3] = al_create_timer(1/60.0);

    timerEnt[4] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[4]);

    string nome[] = {"CHEFE_ATAQUE", "CHEFE_CAST", "CHEFE_TELEPORT", "CHEFE_DASH"};
    audioEnt.inicializaAudio(nome, 4);
}
Chefao::~Chefao()
{
    for (int i = 0; i < 5; i++)
        al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}
void Chefao::desenha()
{
    if(funcionando)
    {
        corEnt=al_map_rgba(fade,fade,fade,fade);
        if(estado == CHE_DASH)
            borda.setXY(62, 32);
        else
            borda.setXY(32, 62);

        if(!(estado==CHE_DASH && pos.getVelocidade().getX()==0))
            desenhaFrame.desenha(pos.getEspaco(),mapaAtual->getPos().getEspaco()+Vetor_R2::vetor(145, 180), dir, false, corEnt);
    }
}
void Chefao::atualiza()
{
    if(funcionando)
    {
        if(estado!=CHE_MORRENDO && estado!=CHE_DASH && estado!=CHE_CONJURANDO && fade==255)
        {
            if(!atkChefe[6]->getFuncionando())
                atkChefe[6]->setFuncionando(true);
        }
        else
            atkChefe[6]->resetaEntidade();
        if(!al_get_timer_started(timerEnt[3]))
        {
            if((estado == CHE_CONJURANDO || estado == CHE_DASH) && !teleport && !teleportConj)
                permiteColisaoEnt=true;
            else
                permiteColisaoEnt=false;
        }
        if(tomandoDano && vida>0)
        {
            tomandoDano=false;
            permiteColisaoEnt=false;
            al_start_timer(timerEnt[3]);
        }
        if(al_get_timer_started(timerEnt[3]) && al_get_timer_count(timerEnt[3])>30)
        {
            if(estado == CHE_CONJURANDO || estado == CHE_DASH)
                permiteColisaoEnt=true;
            al_stop_timer(timerEnt[3]);
            al_set_timer_count(timerEnt[3],0);
        }
        if(!teleport && !teleportConj)
            estado = desenhaFrame.getEstado();
        if(!tomandoDano && (estado == CHE_VOANDO || estado == CHE_ATACANDO) && !iniciaDash )
            desviar();

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();
        criaEfeitos();
        if(!teleport && !teleportConj && estado!=CHE_ATACANDO)
            pos.mover();

        if(vida <= 0)
        {
            estado=CHE_MORRENDO;
        }
        if(!teleport && !teleportConj)
        {
            desenhaFrame.setEstado(estado);
            desenhaFrame.atualizaFrame();
        }

        if(atkChefe[6]->getFuncionando())
            atkChefe[6]->getPos().setEspaco(pos.getEspaco());

        audioEnt.atualizaAudio();
    }
}

void Chefao::atualizaComportamento()
{
    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {
        if(iniciaDash)
        {
            pos.getVelocidade().setXY(0,-3);
            pos.getAceleracao().setXY(0,0);
            permiteColisaoChao=false;
            if(pos.getEspaco().getY()<-30)
            {
                pos.getVelocidade().setY(0);
                iniciaDash=false;
                estado=CHE_DASH;
                atkChefe[5]->setDano(danoDash + rand()%(danoRandDash*2 + 1) - danoRandDash);
                al_start_timer(timerEnt[1]);

                if(rand()%2)
                    pos.getEspaco().setX(18800-100);
                else
                    pos.getEspaco().setX(18800+1300);
                pos.getEspaco().setY(300+rand()%300);
            }
        }
        else if(estado == CHE_DASH)
        {
            if(pos.getVelocidade().getX()!=0)
                al_set_timer_count(timerEnt[1],0);
            if(al_get_timer_count(timerEnt[1])>tempoDash)
            {
                al_set_timer_count(timerEnt[1],0);
                tempoDash-=20;
                if(tempoDash<=0)
                {
                    estado=CHE_VOANDO;
                    tempoDash=180;
                    pos.getEspaco().setXY(18800+600,0);
                    al_start_timer(timerEnt[0]);
                }
                else
                {
                    if(pos.getEspaco().getX()<=18800-100)
                    {
                        pos.getVelocidade().setX(25);
                        dir=0;
                    }
                    else if(pos.getEspaco().getX()>=18800+1300)
                    {
                        pos.getVelocidade().setX(-25);
                        dir=1;
                    }
                    pos.getEspaco().setY(300+rand()%300);

                    audioEnt.tocaAudio("CHEFE_DASH", 1, ALLEGRO_PLAYMODE_ONCE, 0);

                }
            }
            if(pos.getVelocidade().getX()>0 &&
                    pos.getEspaco().getX()>=18800+1300)
                pos.getVelocidade().setX(0);

            else if(pos.getVelocidade().getX()<0 &&
                    pos.getEspaco().getX()<=18800-100)
                pos.getVelocidade().setX(0);
        }
        else if(estado == CHE_DANO)
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
                estado = CHE_VOANDO;
                tomandoDano=false;
            }
        }
        else if(estado == CHE_MORRENDO)
        {

            pos.getVelocidade().setXY(0,0);/**parar chefe quando o mesmo morrer**/
            //pos.getVelocidade().setX(pos.getVelocidade().getX()*0.95);
            //pos.getVelocidade().setY(pos.getVelocidade().getY()*0.95);

            if((desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                    desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
            {
                mudaScore(score);
                resetaEntidade();
            }

        }
        else if(teleport || teleportConj)
        {
            if(fade>0 && pos.getEspaco().getX() != destino)
            {
                fade-=10;
                if(fade<0 && teleport)
                {
                    fade=0;
                    dir=dirDestino;
                }
                if(fade<0 && teleportConj)
                {
                    fade=0;
                    dir=dirDestino;
                }
            }
            else if(teleport)
            {
                pos.getEspaco().setX(destino);
                pos.getEspaco().setY(destinoY);
                fade+=10;
                if(fade>=255)
                {
                    fade=255;
                    teleport=false;
                    if(estado!=CHE_CONJURANDO)
                        estado=CHE_ATACANDO;
                }
            }
            else if(teleportConj)
            {
                pos.getAceleracao().setX(0);
                pos.getVelocidade().setX(0);
                pos.getAceleracao().setY(0);
                pos.getVelocidade().setY(0);
                pos.getEspaco().setX(destino);
                pos.getEspaco().setY(300);
                fade+=10;
                if(fade>=255)
                {
                    fade=255;
                    teleportConj=false;
                }
            }

        }
        else if(estado==CHE_ATACANDO || estado==CHE_CONJURANDO || estado==CHE_COMECA_CONJURACAO ||
                estado==CHE_TERMINA_CONJURACAO)
        {
            if(estado==CHE_CONJURANDO && !audioEnt.audioTocando("CHEFE_CAST"))
            {
                audioEnt.tocaAudio("CHEFE_CAST", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            }
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setY(0);
            pos.getVelocidade().setY(0);
        }
        else if(rand()%300==0 && !teleport && estado == CHE_VOANDO)
        {

            audioEnt.tocaAudio("CHEFE_TELEPORT", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            if(alvo->getDir() == 1)
            {
                teleport=true;
                destino=alvo->getPos().getEspaco().getX()+130;
                destinoY=alvo->getPos().getEspaco().getY();
                dirDestino=1;
            }
            else if(alvo->getDir() == 0)
            {
                teleport=true;
                destino=alvo->getPos().getEspaco().getX()-130;
                destinoY=alvo->getPos().getEspaco().getY();
                dirDestino=0;
            }
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

            if(((pos.getEspaco().getX() < alvo->getPos().getEspaco().getX()&&
                    pos.getEspaco().getX()+borda.getX() +90 > alvo->getPos().getEspaco().getX()-alvo->getBorda().getX()) ||
                    (pos.getEspaco().getX() > alvo->getPos().getEspaco().getX()&&
                     pos.getEspaco().getX()-borda.getX() -90 < alvo->getPos().getEspaco().getX()+alvo->getBorda().getX()))&&
                    (abs((alvo->getPos().getEspaco().getY())-(pos.getEspaco().getY()))<70))
                perto=true;
            else
                perto=false;

            delX=alvo->getPos().getEspaco().getX() - pos.getEspaco().getX();
            delY=alvo->getPos().getEspaco().getY() - pos.getEspaco().getY();

            if(delX>0)
                dir=0;
            else
                dir=1;


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
        if(perto && (estado == CHE_VOANDO || estado == CHE_ATACANDO) &&
                !teleport && !teleportConj && !iniciaDash)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setY(0);
            pos.getVelocidade().setY(0);
            if(estado!=CHE_ATACANDO)
                estado=CHE_ATACANDO;
        }

    }
}
void Chefao::atualizaColisoes()
{

}
void Chefao::atualizaEstados()
{
    if(al_get_timer_count(timerEnt[0])>300 && !iniciaDash && !teleport && !teleportConj && estado==CHE_VOANDO)
    {
        iniciarEspecial=true;
        especiaisChefao=rand()%4;
        if(especiaisChefao==ONDA)
        {
            al_set_timer_count(timerEnt[2],0);
        }
        al_stop_timer(timerEnt[0]);
        al_set_timer_count(timerEnt[0],0);
    }
    else if(iniciaDash)
    {

    }
    else if(estado == CHE_DASH)
    {

    }
    else if(tomandoDano && !(estado==CHE_DANO) && !(estado==CHE_MORRENDO) && vida>=0)/**parar chefe quando o mesmo morrer**/
    {
        estado = CHE_DANO;
        al_set_timer_count(timerEnt[0],0);
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
    else if((estado==CHE_ATACANDO)&&(desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        estado=CHE_VOANDO;
        if(al_get_timer_count(timerEnt[0])>250)
            al_set_timer_count(timerEnt[0],250);
    }
    else if(iniciarEspecial && especiaisChefao == DASH)
    {
        iniciaDash = true;
        iniciarEspecial=false;
    }
    else if(iniciarEspecial)
    {
        al_start_timer(timerEnt[0]);
        estado=CHE_COMECA_CONJURACAO;
        if((desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
        {
            audioEnt.tocaAudio("CHEFE_TELEPORT", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            estado=CHE_CONJURANDO;
            teleportConj=true;
            desenhaFrame.setEstado(estado);
            desenhaFrame.atualizaFrame();
            iniciarEspecial=false;

            if(rand()%2)
            {
                dirDestino=0;
                destino=18800+100;
            }
            else
            {
                dirDestino=1;
                destino=18800+1100;
            }
        }

    }
    else if(estado==CHE_CONJURANDO && al_get_timer_count(timerEnt[0])>361)
    {
        estado=CHE_TERMINA_CONJURACAO;
        al_set_timer_count(timerEnt[0],0);
    }
    else if(estado == CHE_TERMINA_CONJURACAO)
    {
        if((desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
        {
            estado=CHE_VOANDO;
            al_set_timer_count(timerEnt[0],0);
        }
    }

}
void Chefao::setFuncionando(bool f)
{
    funcionando=f;
}
void Chefao::inicializaEntidade()
{
    atkChefe[0] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(57,10),20,5);
    atkChefe[1] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(20,12),20,5);
    atkChefe[2] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(45,82),20,5);
    atkChefe[3] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(27,13),20,5);
    atkChefe[4] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(50,6),20,5);
    atkChefe[5] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(110,30),50,1000);
    atkChefe[6] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), borda,50,1000);
    atkChefe[6]->setDano(danoBase + rand()%(danoRand*2 + 1) - danoRand);

    for (int i = 0; i < 7; i++)
    {
        atkChefe[i]->setMapaAtual(mapaAtual);
        atkChefe[i]->setPtrJ(NULL);
        atkChefe[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atkChefe[i]));
    }
    for(int i=0; i<15; i++)
    {
        chuvaProj[i] = new Chuva_Fogo(Vetor_R2::vetor(-200,-200));
        chuvaProj[i]->setMapaAtual(mapaAtual);
        chuvaProj[i]->setPtrJ(NULL);
        chuvaProj[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (chuvaProj[i]));
    }
    for(int i=0; i<7; i++)
    {
        ondaProj[i] = new Onda_Fogo(Vetor_R2::vetor(-200,-200));
        ondaProj[i]->setMapaAtual(mapaAtual);
        ondaProj[i]->setPtrJ(NULL);
        ondaProj[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (ondaProj[i]));
    }
    for(int i=0; i<10; i++)
    {
        colunaProj[i] = new Coluna_Fogo(Vetor_R2::vetor(-200,-200));
        colunaProj[i]->setMapaAtual(mapaAtual);
        colunaProj[i]->setPtrJ(NULL);
        colunaProj[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (colunaProj[i]));
    }
    for (int i = 0; i < 10; i++)
    {
        rast[i] = new Rastro(Vetor_R2::vetor(-200,-200), desenhaFrame.getImagem(), desenhaFrame.getSpriteX(),
                             desenhaFrame.getSpriteY(), 1.5, 0.01, 1, dir, 1,1,1);
        rast[i]->setMapaAtual(mapaAtual);
        colunaProj[i]->setPtrJ(NULL);
        colunaProj[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (rast[i]));
    }
}
void Chefao::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    for(int i=0;i<5;i++)
    {
        if(vida <=0 && al_get_timer_started(timerEnt[i]))
        {
            al_stop_timer(timerEnt[i]);
            al_set_timer_count(timerEnt[i],0);
        }
    }

    estado=CHE_VOANDO;
    desenhaFrame.setEstado(CHE_VOANDO);
    desenhaFrame.atualizaFrame();
}
void Chefao::criaProjetil()
{
    if(estado==CHE_DASH && pos.getVelocidade().getX()!=0 && !atkChefe[5]->getFuncionando())
        atkChefe[5]->setFuncionando(true);
    else if(estado!=CHE_DASH || (estado==CHE_DASH && pos.getVelocidade().getX()==0 && atkChefe[5]->getFuncionando()))
        atkChefe[5]->resetaEntidade();
    if(atkChefe[5]->getFuncionando())
    {
        if(dir)
            atkChefe[5]->getPos().setEspaco(pos.getEspaco()+ Vetor_R2::vetor(-60,-20));
        else
            atkChefe[5]->getPos().setEspaco(pos.getEspaco()+ Vetor_R2::vetor(70,-20));
    }
    if((estado==CHE_ATACANDO) && desenhaFrame.getFrameAtual() == 14 && desenhaFrame.getTempoEstado() == 1)
    {
        audioEnt.tocaAudio("CHEFE_ATAQUE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        for(int i=0; i<5; i++)
        {
            atkChefe[i]->setDano(danoGolpe + rand()%(danoGolpeRand*2 + 1) - danoGolpeRand);
            atkChefe[i]->setFuncionando(true);
        }

        if(dir)
        {
            atkChefe[0]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(0,-150)));
            atkChefe[1]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-72,-125)));
            atkChefe[2]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-90,-30)));
            atkChefe[3]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-65,67)));
            atkChefe[4]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(0,90)));
        }
        else
        {
            atkChefe[0]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(0,-150)));
            atkChefe[1]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(72,-125)));
            atkChefe[2]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(90,-30)));
            atkChefe[3]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(65,67)));
            atkChefe[4]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(0,90)));
        }
    }
    if(estado==CHE_CONJURANDO && pos.getEspaco().getX()==destino && especiaisChefao == COLUNA)
    {
        if(rand()%25==1 || al_get_timer_count(timerEnt[0])%24==1)
        {
            for(int i=0; i<10; i++)
            {
                if(!colunaProj[i]->getFuncionando())
                {
                    colunaProj[i]->setFuncionando(true);
                    colunaProj[i]->getPos().getEspaco().setX(18800+rand()%1200);
                    colunaProj[i]->getPos().getEspaco().setY(400);
                    break;
                }
            }
        }

    }
    else if(estado==CHE_CONJURANDO && pos.getEspaco().getX()==destino && especiaisChefao == ONDA)
    {
        if(al_get_timer_count(timerEnt[2])%72==0)
        {
            for(int i=0; i<7; i++)
            {
                if(!ondaProj[i]->getFuncionando())
                {
                    al_set_timer_count(timerEnt[2],al_get_timer_count(timerEnt[2])+1);
                    if(dir)
                    {
                        ondaProj[i]->getPos().getEspaco().setX(18800+1230);
                        ondaProj[i]->setCoefAce(-0.05);
                    }
                    else
                    {
                        ondaProj[i]->getPos().getEspaco().setX(18800-30);
                        ondaProj[i]->setCoefAce(0.05);
                    }

                    if(rand()%2)
                        ondaProj[i]->getPos().getEspaco().setY(180);
                    else
                        ondaProj[i]->getPos().getEspaco().setY(540);
                    ondaProj[i]->setFuncionando(true);
                    break;
                }
            }
        }
    }
    else if(estado==CHE_CONJURANDO && pos.getEspaco().getX()==destino && especiaisChefao == CHUVA)
    {
        if(rand()%25==1 || al_get_timer_count(timerEnt[0])%24==1)
        {
            for(int i=0; i<15; i++)
            {
                if(!chuvaProj[i]->getFuncionando())
                {
                    chuvaProj[i]->setFuncionando(true);
                    chuvaProj[i]->getPos().getEspaco().setX(18800+ rand()%1200);
                    chuvaProj[i]->getPos().getEspaco().setY(0);
                    break;
                }
            }
        }
    }
}
void Chefao::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[CHE_VOANDO][0]=0;
    estFrame[CHE_VOANDO][1]=5;
    estFrame[CHE_VOANDO][2]=1;
    estFrame[CHE_ATACANDO][0]=6;
    estFrame[CHE_ATACANDO][1]=21;
    estFrame[CHE_ATACANDO][2]=0;
    estFrame[CHE_DANO][0]=22;
    estFrame[CHE_DANO][1]=22;
    estFrame[CHE_DANO][2]=1;
    estFrame[CHE_COMECA_CONJURACAO][0]=23;
    estFrame[CHE_COMECA_CONJURACAO][1]=26;
    estFrame[CHE_COMECA_CONJURACAO][2]=0;
    estFrame[CHE_CONJURANDO][0]=27;
    estFrame[CHE_CONJURANDO][1]=32;
    estFrame[CHE_CONJURANDO][2]=1;
    estFrame[CHE_TERMINA_CONJURACAO][0]=33;
    estFrame[CHE_TERMINA_CONJURACAO][1]=36;
    estFrame[CHE_TERMINA_CONJURACAO][2]=0;
    estFrame[CHE_DASH][0]=37;
    estFrame[CHE_DASH][1]=39;
    estFrame[CHE_DASH][2]=1;
    estFrame[CHE_MORRENDO][0]=40;
    estFrame[CHE_MORRENDO][1]=59;
    estFrame[CHE_MORRENDO][2]=0;


    tFrame[CHE_DANO].inclui_frente(10);
    for(i=0; i<6; i++)
    {
        tFrame[CHE_VOANDO].inclui_frente(10);
        tFrame[CHE_CONJURANDO].inclui_frente(10);
        if(i<4)
        {
            tFrame[CHE_COMECA_CONJURACAO].inclui_frente(10);
            tFrame[CHE_TERMINA_CONJURACAO].inclui_frente(10);
        }
        if(i<3)
            tFrame[CHE_DASH].inclui_frente(10);
    }

    for(i=0; i<16; i++)
    {
        if(i<4)
            tFrame[CHE_ATACANDO].inclui_frente(7);
        else if(i<13)
            tFrame[CHE_ATACANDO].inclui_frente(5);
        else if(i<16)
            tFrame[CHE_ATACANDO].inclui_frente(8);
    }
    for(i=0; i<20; i++)
        tFrame[CHE_MORRENDO].inclui_frente(10+(i/2));



}
void Chefao::criaEfeitos()
{
    if (estado == CHE_DASH && al_get_timer_count(timerEnt[4])%6 == 0 && pos.getVelocidade().getX() != 0)
    {
        for (int i = 0; i < 10; i++)
        {
            if (!rast[i]->getFuncionando())
            {
                rast[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(-145, -180));
                rast[i]->setVelDes(0.1);
                rast[i]->setFrameAtual(desenhaFrame.getFrameAtual());
                rast[i]->setAng(dir);
                rast[i]->setFuncionando(true);
                break;
            }
        }
    }
}
bool Chefao::desviar()
{
    for(int i=0; i<2; i++)
    {
        if(jogadores[i]!=NULL && !teleport)
        {
            if(jogadores[i]->getEstadoJogador()==6 ||
                    jogadores[i]->getEstadoJogador()==7 ||
                    jogadores[i]->getEstadoJogador()==8 ||
                    jogadores[i]->getEstadoJogador()==10)
            {
                if(jogadores[i]->getDir() == 1 &&
                        jogadores[i]->getPos().getEspaco().getX() > pos.getEspaco().getX() &&
                        jogadores[i]->getPos().getEspaco().getX() - pos.getEspaco().getX() < 220 &&
                        abs(jogadores[i]->getPos().getEspaco().getY() - pos.getEspaco().getY()) < 140)
                {
                    audioEnt.tocaAudio("CHEFE_TELEPORT", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                    teleport=true;
                    destino=jogadores[i]->getPos().getEspaco().getX()+130;
                    destinoY=alvo->getPos().getEspaco().getY();
                }
                else if(jogadores[i]->getDir() == 0 &&
                        pos.getEspaco().getX() > jogadores[i]->getPos().getEspaco().getX() &&
                        pos.getEspaco().getX() - jogadores[i]->getPos().getEspaco().getX() < 220   &&
                        abs(jogadores[i]->getPos().getEspaco().getY() - pos.getEspaco().getY()) < 140)
                {
                    audioEnt.tocaAudio("CHEFE_TELEPORT", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                    teleport=true;
                    destino=jogadores[i]->getPos().getEspaco().getX()-130;
                    destinoY=alvo->getPos().getEspaco().getY();
                }
            }
            else if(jogadores[i]->getEstadoJogador()==11 ||
                    jogadores[i]->getEstadoJogador()==12)
            {
                if(abs(jogadores[i]->getPos().getEspaco().getX() - pos.getEspaco().getX()) < 100 &&
                        jogadores[i]->getPos().getEspaco().getY() > pos.getEspaco().getY())
                {
                    audioEnt.tocaAudio("CHEFE_TELEPORT", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                    teleport=true;
                    if(jogadores[i]->getPos().getEspaco().getX() > pos.getEspaco().getX())
                    {
                        destino=jogadores[i]->getPos().getEspaco().getX()+130;
                        destinoY=alvo->getPos().getEspaco().getY();
                    }
                    else
                    {
                        destino=jogadores[i]->getPos().getEspaco().getX()-130;
                        destinoY=alvo->getPos().getEspaco().getY();
                    }
                }
            }
            alvo=jogadores[i];
            if(alvo->getPos().getEspaco().getX()>pos.getEspaco().getX())
                dirDestino=1;
            else
                dirDestino=0;
        }
    }
}
void Chefao::paraTimer()
{
    for (int i = 0; i < 5; i++)
    {
        if (al_get_timer_started(timerEnt[i]))
        {
            al_stop_timer(timerEnt[i]);
            timerParado.inclui_frente(i);
        }
    }
}




