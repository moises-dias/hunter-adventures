#include "Fantasma.h"

Fantasma::Fantasma()
{

}
Fantasma::Fantasma(int ix,int iy)
{
    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(70, 95);

    desenhaFrame.setSpriteX(96);
    desenhaFrame.setSpriteY(101);
    desenhaFrame.setNomeImg("Fantasma.png");

    desenhaFrame.setNumEstados(1);
    desenhaFrame.setGiraRot(false);
    corEnt=al_map_rgba(255,255,255,100);


    posEquilib=480;
    aceleracao=0.05;


    /**PARAMETROS DO OBSTACULO**/
    vidaBase = 0;
    vidaRand = 0;
    danoBase = 500;
    danoRand = 50;
    /**PARAMETROS DO OBSTACULO**/

    vida = 10;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;


    tomandoDano=false;
    funcionando=false;
    permiteColisaoEnt=true;

    construiFrames();

    desenhaFrame.setCoef(2.5);
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    string nome[] = {"GEMIDAO_DO_FANTASMA"};
    audioEnt.inicializaAudio(nome, 1);
}
Fantasma::~Fantasma()
{

}

void Fantasma::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(115, 125),
                             0, false, corEnt);
    }
}
void Fantasma::atualiza()
{
    if(funcionando)
    {
        pos.mover();

        estado = desenhaFrame.getEstado();

        atualizaComportamento();

        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();

    }
}

void Fantasma::construiFrames()
{

    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0]=0;
    estFrame[0][1]=0;
    estFrame[0][2]=1;

    tFrame[0].inclui_frente(10);
}
void Fantasma::atualizaComportamento()
{
    if(pos.getEspaco().getY()>posEquilib)
        pos.getAceleracao().setY(-aceleracao);
    else
        pos.getAceleracao().setY(aceleracao);
    for(int i=0;i<2;i++)
    {
        if(jogadores[i]!=NULL)
        {
            if(abs(jogadores[i]->getPos().getEspaco().getX()-pos.getEspaco().getX())<10)
                if(!audioEnt.audioTocando("GEMIDAO_DO_FANTASMA"))
                    audioEnt.tocaAudio("GEMIDAO_DO_FANTASMA",1,ALLEGRO_PLAYMODE_ONCE,0);
        }
    }
}
void Fantasma::atualizaEstados()
{

}

void Fantasma::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;

    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    desenhaFrame.setGiraRot(false);
}
void Fantasma::setFuncionando(bool f)
{
    funcionando=f;
    pos.getVelocidade().setY(5);
}
void Fantasma::paraTimer()
{
}
