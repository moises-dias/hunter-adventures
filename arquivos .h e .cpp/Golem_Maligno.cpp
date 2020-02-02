#include "Golem_Maligno.h"

#include "Ataque_Generico.h"
#include "Lista_Entidades.h"

Golem_Maligno::Golem_Maligno()
{

}
Golem_Maligno::Golem_Maligno(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 11200;
    vidaRand = 1120;
    danoBase = 1000;
    danoRand = 100;
    danoGolpe = 2500;
    danoGolpeRand = 250;
    score = 600;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(55, 80);

    desenhaFrame.setSpriteX(200);
    desenhaFrame.setSpriteY(200);
    desenhaFrame.setNomeImg("Golem_Maligno.png");
    al_convert_mask_to_alpha(desenhaFrame.getImagem(),al_map_rgba(0,0,0,5));

    desenhaFrame.setNumEstados(4);


    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;

    tomandoDano=false;
    funcionando=false;
    perto=false;

    vRand = (rand()%1001)/1000.0;
    vXMax=0.5+vRand;

    construiFrames();
    pisadaFrame=0;
    corEnt = al_map_rgba(255,255,255,255);

    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(GOL_ANDANDO);
    desenhaFrame.atualizaFrame();

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);


    string nome[] = {"GOLEM_MALIGNO_DANO", "GOLEM_MALIGNO_GOLPE", "GOLEM_MALIGNO_MORTE", "GOLEM_PASSO"};
    audioEnt.inicializaAudio(nome, 4);

}
Golem_Maligno::~Golem_Maligno()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete[] timerEnt;
}

void Golem_Maligno::desenha()
{
    if(funcionando)
    {
        if(!dir)
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(200, 300), dir, false, corEnt);
        else
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(220, 300), dir, false, corEnt);
    }
}
void Golem_Maligno::atualiza()
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

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();
        criaProjetil();

        if(!(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<20 &&
                (pos.getEspaco().getY()-alvo->getPos().getEspaco().getY())>70 && !tomandoDano && estado!=GOL_ATAQUE))
            pos.mover();

        if(vida <= 0 && estado!=GOL_MORRENDO)
        {
            estado=GOL_MORRENDO;
            permiteColisaoEnt=false;
            if (!audioEnt.audioTocando("GOLEM_MALIGNO_MORTE") && funcionando && desenhaFrame.getEstado()!=GOL_MORRENDO)
            {
                audioEnt.tocaAudio("GOLEM_MALIGNO_MORTE",1,ALLEGRO_PLAYMODE_ONCE,0);
                mudaScore(score);
            }


        }

        if(estado==GOL_MORRENDO && (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
                desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
        {
            resetaEntidade();
        }

        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();
    }
}
void Golem_Maligno::criaProjetil()
{
    if((estado==GOL_ATAQUE) && desenhaFrame.getFrameAtual() == 19 && desenhaFrame.getTempoEstado() == 1)
    {
        audioEnt.tocaAudio("GOLEM_MALIGNO_GOLPE",1,ALLEGRO_PLAYMODE_ONCE,0);

        for(int i=0; i<3; i++)
        {
            atkGolem[i]->setFuncionando(true);
            atkGolem[i]->setDano(danoGolpe + rand()%(danoGolpeRand*2 + 1) - danoGolpeRand);
        }

        if(dir)
        {
            atkGolem[0]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-165,-30)));
            atkGolem[1]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-120,-170)));
            atkGolem[2]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(-25,-215)));
        }
        else
        {
            atkGolem[0]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(165,-30)));
            atkGolem[1]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(120,-170)));
            atkGolem[2]->getPos().setEspaco(pos.getEspaco()+(Vetor_R2::vetor(25,-215)));
        }
    }
}

void Golem_Maligno::atualizaComportamento()
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
                pos.getEspaco().getX()+borda.getX() +100 > alvo->getPos().getEspaco().getX()-alvo->getBorda().getX()) ||
                (pos.getEspaco().getX() > alvo->getPos().getEspaco().getX()&&
                 pos.getEspaco().getX()-borda.getX() -100 < alvo->getPos().getEspaco().getX()+alvo->getBorda().getX()))&&
                (abs((alvo->getPos().getEspaco().getY())-(pos.getEspaco().getY()))<70))
            perto=true;
        else
            perto=false;


        if(estado==GOL_ATAQUE||estado==GOL_MORRENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
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

        if(perto && estado!=GOL_MORRENDO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            if(estado!=GOL_ATAQUE)
                estado=GOL_ATAQUE;
        }
        if((desenhaFrame.getFrameAtual()==5 || desenhaFrame.getFrameAtual()==1) && estado==GOL_ANDANDO)
        {
            if(pisadaFrame!=desenhaFrame.getFrameAtual())
            {
                if(audioEnt.audioTocando("GOLEM_PASSO"))
                    audioEnt.paraAudio("GOLEM_PASSO");
                audioEnt.tocaAudio("GOLEM_PASSO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
                pisadaFrame=desenhaFrame.getFrameAtual();
            }


        }
    }
    if(tomandoDano && !al_get_timer_started(timerEnt[0]))
    {
        al_start_timer(timerEnt[0]);
        if (vida>0)
            audioEnt.tocaAudio("GOLEM_MALIGNO_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
    if(al_get_timer_count(timerEnt[0])>30)
    {
        tomandoDano=false;
        al_set_timer_count(timerEnt[0],0);
        al_stop_timer(timerEnt[0]);
    }
}
void Golem_Maligno::atualizaColisoes()
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
void Golem_Maligno::atualizaEstados()
{
    if(jogadores[0]==NULL&&jogadores[1]==NULL)
    {
        estado=GOL_PARADO;
    }
    else if(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<20 &&
            (pos.getEspaco().getY()-alvo->getPos().getEspaco().getY())>70 && !tomandoDano && estado!=GOL_ATAQUE)
    {
        estado=GOL_PARADO;
    }
    else if((colisao[COLIDE_XD] || colisao[COLIDE_XE]) && estado==GOL_ANDANDO)
    {
        estado=GOL_PARADO;
    }
    else if(estado==GOL_PARADO && !(abs(pos.getEspaco().getX()-alvo->getPos().getEspaco().getX())<20))
    {
        estado=GOL_ANDANDO;
    }
    else if(perto && estado==GOL_ANDANDO)
    {
        estado=GOL_ATAQUE;
    }
    else if((estado==GOL_ATAQUE)&&(desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        if(!(perto && estado!=GOL_MORRENDO))
            estado=GOL_ANDANDO;
        else
        {
            if(alvo->getPos().getEspaco().getX()<pos.getEspaco().getX())
                dir=1;
            else
                dir=0;
        }
    }
}
void Golem_Maligno::criaEfeitos()
{

}

void Golem_Maligno::inicializaEntidade()
{
    atkGolem[0] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(33,107),20,4);
    atkGolem[1] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(50,27),20,4);
    atkGolem[2] = new Ataque_Generico(Vetor_R2::vetor(-200,-200), Vetor_R2::vetor(100,10),20,4);
    for(int i=0; i<3; i++)
    {
        atkGolem[i]->setMapaAtual(mapaAtual);
        atkGolem[i]->setPtrJ(NULL);
        atkGolem[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atkGolem[i]));
    }
}
void Golem_Maligno::resetaEntidade()
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
    pisadaFrame=0;

    alvo=NULL;

    estado=GOL_ANDANDO;
    desenhaFrame.setEstado(GOL_ANDANDO);
    desenhaFrame.atualizaFrame();
}
void Golem_Maligno::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[GOL_PARADO][0]=0;
    estFrame[GOL_PARADO][1]=0;
    estFrame[GOL_PARADO][2]=1;
    estFrame[GOL_ANDANDO][0]=1;
    estFrame[GOL_ANDANDO][1]=8;
    estFrame[GOL_ANDANDO][2]=1;
    estFrame[GOL_ATAQUE][0]=9;
    estFrame[GOL_ATAQUE][1]=25;
    estFrame[GOL_ATAQUE][2]=1;
    estFrame[GOL_MORRENDO][0]=26;
    estFrame[GOL_MORRENDO][1]=30;
    estFrame[GOL_MORRENDO][2]=0;

    for(i=0; i<8; i++)
    {
        tFrame[GOL_ANDANDO].inclui_frente(10);
    }
    for(i=0; i<5; i++)
    {
        tFrame[GOL_MORRENDO].inclui_frente(20);
    }
    tFrame[GOL_PARADO].inclui_frente(10);

    for(i=0; i<8; i++)
    {
        tFrame[GOL_ATAQUE].inclui_frente(10);
    }
    for(i=0; i<4; i++)
    {
        tFrame[GOL_ATAQUE].inclui_frente(2);
    }
    for(i=0; i<5; i++)
    {
        tFrame[GOL_ATAQUE].inclui_frente(10);
    }

}
void Golem_Maligno::paraTimer()
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
