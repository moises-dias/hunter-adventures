#include "Lava.h"
#include "Bola_Fogo.h"
#include "Lista_Entidades.h"
Lava::Lava()
{

}
Lava::Lava(int ix,int iy)
{
    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(550, 40);

    desenhaFrame.setSpriteX(0);
    desenhaFrame.setSpriteY(0);
    desenhaFrame.setNomeImg("");

    desenhaFrame.setNumEstados(0);


    tempoInvocar=100;

    /**PARAMETROS DO OBSTACULO**/
    vidaBase = 0;
    vidaRand = 0;
    danoBase = 0;
    danoRand = 0;
    /**PARAMETROS DO OBSTACULO**/

    vida = 10;
    dano = 999999;



    tomandoDano=false;
    funcionando=false;
    permiteColisaoEnt=true;
    permiteColisaoChao=false;

    timerEnt = new ALLEGRO_TIMER* [1];
    timerEnt[0] = al_create_timer(1/60.0);
}
Lava::~Lava()
{
    al_destroy_timer(timerEnt[0]);
    if (timerEnt != NULL)
        delete [] timerEnt;
}

void Lava::desenha()
{
    if(funcionando)
    {
    }
}
void Lava::atualiza()
{
    if(funcionando)
    {
        atualizaComportamento();
    }
}
void Lava::inicializaEntidade()
{
    for(int i=0; i<5; i++)
    {
        bolas[i] = new Bola_Fogo(Vetor_R2::vetor(-200,-200));
        bolas[i]->setMapaAtual(mapaAtual);
        bolas[i]->setPtrJ(NULL);
        bolas[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (bolas[i]));
    }
}
void Lava::construiFrames()
{

}
void Lava::atualizaComportamento()
{
    if(al_get_timer_count(timerEnt[0])>tempoInvocar)
    {
        for(int i=0; i<5; i++)
        {
            if(!bolas[i]->getFuncionando())
            {
                bolas[i]->setFuncionando(true);
                bolas[i]->getPos().setEspaco(Vetor_R2::vetor(pos.getEspaco().getX()-borda.getX()+rand()%(2*(int)borda.getX()),
                                             pos.getEspaco().getY()));
                al_set_timer_count(timerEnt[0],0);
                tempoInvocar=70+rand()%60;
                break;
            }
        }
    }
}
void Lava::atualizaEstados()
{

}

void Lava::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;


    al_stop_timer(timerEnt[0]);
    al_set_timer_count(timerEnt[0],0);
}
void Lava::setFuncionando(bool f)
{
    funcionando=f;
    al_start_timer(timerEnt[0]);
}
void Lava::paraTimer()
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
