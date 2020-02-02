#include "Onda_Fogo.h"

Onda_Fogo::Onda_Fogo()
{

}
Onda_Fogo::Onda_Fogo(Vetor_R2 posI) :
    Projetil(200, 200, "Onda_Chefao.png", 1)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1000000;
    vidaRand = 0;
    danoBase = 1300;
    danoRand = 130;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt = true;
    permiteColisaoChao = false;
    borda.setXY(60, 120);
    tomandoDano = false;

    estado=0;
    desenhaFrame.setEstado(0);

    funcionando = false;
    iniciarSom=false;

    coefAce=0;
    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    ptrJ = NULL;


    criaFrames();
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    desenhaFrame.setCoef(1.3);

    string nome[] = {"ONDA_CHEFE"};
    audioEnt.inicializaAudio(nome, 1);
}
Onda_Fogo::~Onda_Fogo()
{

}
void Onda_Fogo::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(130,130), dir, false, corEnt);
    }
}
void Onda_Fogo::atualiza()
{
    if(funcionando)
    {
        if(pos.getEspaco().getX() < 18800-50 ||
                pos.getEspaco().getX() > 18800+1250)
        {
            resetaEntidade();
        }
        if(((dir && pos.getEspaco().getX()>18800+1150 && pos.getEspaco().getX()<18800+1200) ||
                (!dir && pos.getEspaco().getX()>18800 && pos.getEspaco().getX()<18800+50)) &&
                !iniciarSom)
        {
            audioEnt.tocaAudio("ONDA_CHEFE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            iniciarSom=true;
        }

        pos.getAceleracao().setX(pos.getAceleracao().getX()+coefAce);
        desenhaFrame.atualizaFrame();
        pos.mover();
        audioEnt.atualizaAudio();
    }
}
void Onda_Fogo::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 7;
    estFrame[0][1] = 13;
    estFrame[0][2] = 1;

    for(i=0; i<7; i++)
        tFrame[0].inclui_frente(5);
}
void Onda_Fogo::resetaEntidade()
{
    funcionando=false;
    tomandoDano=false;
    permiteColisaoEnt=true;
    iniciarSom=false;
    estado=0;
    coefAce=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
void Onda_Fogo::setCoefAce(float c)
{
    coefAce=c;
    if(c>0)
        dir=0;
    else
        dir=1;
}
