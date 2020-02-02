#include "Osso_Aereo.h"


Osso_Aereo::Osso_Aereo()
{

}
Osso_Aereo::Osso_Aereo(Vetor_R2 posI, Vetor_R2 velI, float aceleracao) :
    Projetil(96, 96, "Projeteis.png", 1)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1;
    vidaRand = 0;
    danoBase = 800;
    danoRand = 80;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =true;
    permiteColisaoChao = true;
    borda.setXY(18, 18);
    tomandoDano=false;
    funcionando = false;
    colidePlat=false;

    ang=0;
    coefAng=PI/6.0;
    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.setVelocidade(velI);
    pos.getAceleracao().setXY(0,aceleracao);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    string nome[] = {"OSSO_AEREO_COLISAO"};
    audioEnt.inicializaAudio(nome, 1);

    criaFrames();

}
Osso_Aereo::~Osso_Aereo()
{

}
void Osso_Aereo::desenha()
{
    if(funcionando)
    {


        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco(), ang, true, corEnt);
    }
}
void Osso_Aereo::atualiza()
{
    if(funcionando)
    {

        for(int i=0; i<3; i++)
            if(colisao[i])
            {
                resetaEntidade();
            }

        if(pos.getVelocidade().getX()>0)
            ang+=coefAng;
        else
            ang-=coefAng;

        if(vida<0)
        {
            resetaEntidade();
        }
        else if(tomandoDano)
        {
            resetaEntidade();
            audioEnt.tocaAudio("OSSO_AEREO_COLISAO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
        audioEnt.atualizaAudio();
        desenhaFrame.atualizaFrame();
        pos.mover();


    }
}

void Osso_Aereo::setFuncionando(bool f)
{
    funcionando = f;
    tomandoDano=false;
}


void Osso_Aereo::criaFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 16;
    estFrame[0][1] = 16;
    estFrame[0][2] = 1;

    tFrame[0].inclui_frente(10);
}
void Osso_Aereo::resetaEntidade()
{

    tomandoDano=false;
    funcionando = false;

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    ang=0;

}
