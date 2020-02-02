#include "Caveira_Flamejante.h"

Caveira_Flamejante::Caveira_Flamejante()
{

}
Caveira_Flamejante::Caveira_Flamejante(Vetor_R2 posI) :
    Projetil(96, 96, "Caveira_Flamejante.png", 1)
{

    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1;
    vidaRand = 0;
    danoBase = 1500;
    danoRand = 150;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =true;
    permiteColisaoChao = true;
    colidePlat=false;
    borda.setXY(25, 22);
    tomandoDano=false;
    fade=0;

    estado=0;
    desenhaFrame.setEstado(0);

    funcionando=false;


    corEnt = al_map_rgba(0,0,0,0);

    pos.setEspaco(posI);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    criaFrames();
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    desenhaFrame.setCoef(1.5);

    string nome[] = {"CAVEIRA_FLAMEJANTE"};
    audioEnt.inicializaAudio(nome, 1);

}
Caveira_Flamejante::~Caveira_Flamejante()
{

}
void Caveira_Flamejante::desenha()
{
    if(funcionando)
    {
        if(!dir)
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(60,70), dir, false, corEnt);
        else
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(85,70), dir, false, corEnt);
    }
}
void Caveira_Flamejante::atualiza()
{
    if(funcionando)
    {
        for(int i=0; i<3; i++)
            if(colisao[i])
            {
                resetaEntidade();
            }

        if(vida<0)
        {
            resetaEntidade();
        }
        else if(tomandoDano)
        {
            resetaEntidade();
        }

            if(ptrJ!=NULL)
            {
                pos.getAceleracao().setXY((ptrJ->getPos().getEspaco().getX()-pos.getEspaco().getX())/800.0,
                                          (ptrJ->getPos().getEspaco().getY()-pos.getEspaco().getY())/800.0);

                if((pos.getEspaco().getX() > ptrJ->getPos().getEspaco().getX() && pos.getVelocidade().getX() > 0) ||
                        (pos.getEspaco().getX() < ptrJ->getPos().getEspaco().getX() && pos.getVelocidade().getX() < 0))
                {
                    pos.getAceleracao().setX(pos.getAceleracao().getX()*3);
                }
                if((pos.getEspaco().getY() > ptrJ->getPos().getEspaco().getY() && pos.getVelocidade().getY() > 0) ||
                        (pos.getEspaco().getY() < ptrJ->getPos().getEspaco().getY() && pos.getVelocidade().getY() < 0))
                {
                    pos.getAceleracao().setY(pos.getAceleracao().getY()*3);
                }
            }
            else
                pos.getAceleracao().setXY(0,0);



        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();
        if(fade<255)
        {
            fade+=3;
            corEnt = al_map_rgba(fade,fade,fade,fade);

            if(pos.getAceleracao().getX()>0)
                dir=0;
            else
                dir=1;
            if(fade>=255)
                audioEnt.tocaAudio("CAVEIRA_FLAMEJANTE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
        else if(fade>255)
        {
            fade=255;
            pos.mover();

            if(pos.getVelocidade().getX()>0)
                dir=0;
            else
                dir=1;
        }
        else
        {
            pos.mover();

            if(pos.getVelocidade().getX()>0)
                dir=0;
            else
                dir=1;
        }
    }
}
void Caveira_Flamejante::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 0;
    estFrame[0][1] = 9;
    estFrame[0][2] = 1;

    for(i=0; i<10; i++)
        tFrame[0].inclui_frente(5);
}
void Caveira_Flamejante::resetaEntidade()
{
    funcionando=false;
    tomandoDano=false;
    fade=0;
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
void Caveira_Flamejante::setDir(float d)
{
    dir=d;
}
