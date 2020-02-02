#include "Necromante.h"
#include "Lista_Entidades.h"
#include "Caveira_Flamejante.h"

Necromante::Necromante()
{

}
Necromante::Necromante(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 4200;
    vidaRand = 420;
    danoBase = 500;
    danoRand = 50;
    score = 500;
    /**PARAMETROS DO INIMMIGO**/

    permiteColisaoChao=true;
    colidePlat=false;
    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(20, 32);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Necromante.png");

    desenhaFrame.setNumEstados(6);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;

    delX=0;
    delY=0;
    denominador=0;

    velXDano=0;
    velYDano=0;

    distX=170+rand()%70;
    distY=150+rand()%70;
    tempoProjetil=120+rand()%45;

    vXMax=2;
    vYMax=2;

    tomandoDano=false;

    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(NEC_VOANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [2];
    timerEnt[0] = al_create_timer(1/60.0);
    timerEnt[1] = al_create_timer(1/60.0);
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);

    string nome[] = {"NECROMANTE_CAST", "NECROMANTE_DANO", "NECROMANTE_MORTE"};
    audioEnt.inicializaAudio(nome, 3);
}
Necromante::~Necromante()
{
    for(int i = 0; i < 2; i++)
        al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}
void Necromante::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 48), dir, false, corEnt);
    }
}
void Necromante::atualiza()
{
    if(funcionando)
    {
        estado = desenhaFrame.getEstado();

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();

        pos.mover();

        if (tomandoDano)
            permiteColisaoEnt = false;
        else
            permiteColisaoEnt = true;

        if(vida <= 0 && estado!=NEC_COMECA_MORTE && estado!=NEC_MORRENDO)
        {
            estado=NEC_COMECA_MORTE;
            mudaScore(score);
            audioEnt.tocaAudio("NECROMANTE_MORTE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            pos.getAceleracao().setXY(0,0);
            pos.getVelocidade().setY(0);
        }

        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();
        audioEnt.atualizaAudio();
    }
}
void Necromante::criaProjetil()
{
    if(estado == NEC_CONJURANDO && al_get_timer_count(timerEnt[0])>tempoProjetil)
    {
        if(rand()%2 && (!(esqueletos[0]->getFuncionando()) || !(esqueletos[1]->getFuncionando())) &&
           alvo->getPos().getAceleracao().getY()==0)
        {
            for(int i=0; i<2; i++)
            {
                if(!(esqueletos[i]->getFuncionando()))
                {
                    esqueletos[i]->setFuncionando(true);
                    esqueletos[i]->getPos().setEspaco(alvo->getPos().getEspaco()+Vetor_R2::vetor(0,-100));
                    esqueletos[i]->setDir(dir);
                    esqueletos[i]->setEstado(ESQ_RENASCENDO);
                    al_set_timer_count(timerEnt[0],0);
                    tempoProjetil=120+rand()%45;
                    break;
                }
            }
        }
        else
        {
            for(int i=0; i<5; i++)
            {
                if(!(caveiras[i]->getFuncionando()))
                {
                    caveiras[i]->setFuncionando(true);
                    caveiras[i]->getPos().setEspaco(pos.getEspaco());
                    caveiras[i]->setPtrJ(alvo);
                    caveiras[i]->setDir(dir);
                    al_set_timer_count(timerEnt[0],0);
                    tempoProjetil=120+rand()%45;
                    break;
                }
            }
        }
    }





}

void Necromante::atualizaComportamento()
{
    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {
        if(estado == NEC_CONJURANDO)
        {
            pos.getVelocidade().setXY(0,0);
            pos.getAceleracao().setXY(0,0);
        }
        else if(estado == NEC_DANO)
        {
            if (audioEnt.audioTocando("NECROMANTE_CAST"))
                audioEnt.paraAudio("NECROMANTE_CAST");
            pos.getVelocidade().setX(pos.getVelocidade().getX()*0.95);
            pos.getVelocidade().setY(pos.getVelocidade().getY()*0.95);

            if((desenhaFrame.getFrameAtual()==estFrame[estado][1]) &&
                    desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
            {
                pos.getVelocidade().setXY(0,0);
                pos.getAceleracao().setXY(0,0);
                estado = NEC_VOANDO;
                tomandoDano=false;
                al_set_timer_count(timerEnt[0],0);
            }
        }
        else if(estado == NEC_COMECA_MORTE)
        {
            if (audioEnt.audioTocando("NECROMANTE_CAST"))
                audioEnt.paraAudio("NECROMANTE_CAST");
            al_set_timer_count(timerEnt[0],0);
            al_stop_timer(timerEnt[0]);
            if(!colisao[COLIDE_Y])
            {
                pos.getAceleracao().setY(0.2);
            }
            else
                resetaEntidade();

            if((desenhaFrame.getFrameAtual()==estFrame[estado][1]) &&
                    desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
                estado=NEC_MORRENDO;
        }
        else if(estado == NEC_MORRENDO)
        {
            if (audioEnt.audioTocando("NECROMANTE_CAST"))
                audioEnt.paraAudio("NECROMANTE_CAST");
            if(!colisao[COLIDE_Y])
            {
                pos.getAceleracao().setY(0.2);
            }
            else
                resetaEntidade();
        }
        else if(estado == NEC_PARADO)
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



            if(x > alvo->getPos().getEspaco().getX())
            {
                dir=1;
                if(abs(alvo->getPos().getEspaco().getX() + distX - pos.getEspaco().getX())>2)
                    delX=alvo->getPos().getEspaco().getX() + distX - pos.getEspaco().getX();
                else
                    delX=0;
            }
            else
            {
                dir=0;
                if(abs(alvo->getPos().getEspaco().getX() - distX - pos.getEspaco().getX())>2)
                    delX=alvo->getPos().getEspaco().getX() - distX - pos.getEspaco().getX();
                else
                    delX=0;
            }
            if(abs(alvo->getPos().getEspaco().getY() - distY - pos.getEspaco().getY())>2)
                delY=alvo->getPos().getEspaco().getY() - distY - pos.getEspaco().getY();
            else
                delY=0;


            if(abs(delX)>abs(delY))
                denominador=abs(delX);
            else
                denominador=abs(delY);

            if(denominador!=0)
            {
                pos.getVelocidade().setX(vXMax*delX/denominador);
                pos.getVelocidade().setY(vYMax*delY/denominador);
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
void Necromante::atualizaColisoes()
{

}
void Necromante::atualizaEstados()
{
    if(tomandoDano && (estado == NEC_VOANDO || estado == NEC_CONJURANDO))
    {
        estado = NEC_DANO;
        if(vida>0)
            audioEnt.tocaAudio("NECROMANTE_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
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
        velYDano=abs(pos.getVelocidade().getY())/5;
        pos.getAceleracao().setXY(0,0);
    }
    else if(al_get_timer_count(timerEnt[0])>tempoProjetil && estado!=NEC_CONJURANDO && estado != NEC_DANO)
    {
        estado=NEC_CONJURANDO;
        al_set_timer_count(timerEnt[1],0);
        audioEnt.tocaAudio("NECROMANTE_CAST", 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
    else if(al_get_timer_count(timerEnt[1])>85 && estado==NEC_CONJURANDO)
    {
        estado=NEC_VOANDO;
        al_set_timer_count(timerEnt[0],0);
    }
}
void Necromante::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[NEC_PARADO][0]=0;
    estFrame[NEC_PARADO][1]=6;
    estFrame[NEC_PARADO][2]=1;
    estFrame[NEC_VOANDO][0]=0;
    estFrame[NEC_VOANDO][1]=6;
    estFrame[NEC_VOANDO][2]=1;
    estFrame[NEC_CONJURANDO][0]=7;
    estFrame[NEC_CONJURANDO][1]=12;
    estFrame[NEC_CONJURANDO][2]=1;
    estFrame[NEC_DANO][0]=14;
    estFrame[NEC_DANO][1]=16;
    estFrame[NEC_DANO][2]=0;
    estFrame[NEC_COMECA_MORTE][0]=14;
    estFrame[NEC_COMECA_MORTE][1]=17;
    estFrame[NEC_COMECA_MORTE][2]=0;
    estFrame[NEC_MORRENDO][0]=18;
    estFrame[NEC_MORRENDO][1]=21;
    estFrame[NEC_MORRENDO][2]=1;

    for(i=0; i<7; i++)
    {
        tFrame[NEC_PARADO].inclui_frente(10);
        tFrame[NEC_VOANDO].inclui_frente(10);
    }
    for(i=0; i<4; i++)
    {
        tFrame[NEC_COMECA_MORTE].inclui_frente(10);
        tFrame[NEC_MORRENDO].inclui_frente(10);
    }
    for(i=0; i<3; i++)
    {
        tFrame[NEC_DANO].inclui_frente(10);
    }
    for(i=0;i<6;i++)
    {
        tFrame[NEC_CONJURANDO].inclui_frente(10);
    }
}

void Necromante::criaEfeitos()
{

}
void Necromante::inicializaEntidade()
{
    for (int i = 0; i < 2; i++)
    {
        esqueletos[i] = new Esqueleto(-200,-200);
        esqueletos[i]->setJogador(jogadores[0],0);
        esqueletos[i]->setJogador(jogadores[1],1);
        esqueletos[i]->setCorEnt(al_map_rgba(180,0,0,255));
        esqueletos[i]->setMapaAtual(mapaAtual);
        esqueletos[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (esqueletos[i]));
        esqueletos[i]->inicializaEntidade();
        esqueletos[i]->setTipo(true);
    }
    for(int i=0; i<5; i++)
    {
        caveiras[i] = new Caveira_Flamejante(Vetor_R2::vetor(-200,-200));
        caveiras[i]->setMapaAtual(mapaAtual);
        caveiras[i]->setPtrJ(NULL);
        caveiras[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (caveiras[i]));
    }
}
void Necromante::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    funcionando=false;
    tomandoDano=false;

    alvo=NULL;
    estado=NEC_VOANDO;
    desenhaFrame.setEstado(NEC_VOANDO);
    desenhaFrame.atualizaFrame();

    al_stop_timer(timerEnt[0]);
    al_stop_timer(timerEnt[1]);
    al_set_timer_count(timerEnt[0],0);
    al_set_timer_count(timerEnt[1],0);

}
void Necromante::setFuncionando(bool f)
{
    funcionando=f;
    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[1]);
}
void Necromante::paraTimer()
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

