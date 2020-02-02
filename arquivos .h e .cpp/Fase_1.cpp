#include "Fase_1.h"
#include "Fumaca.h"
#include "Barra_De_Vida.h"
#include "Jogo.h"
#include "Armadilha_Urso.h"

Fase_1::Fase_1()
{

}
Fase_1::Fase_1(Lista_Entidades * listEnt, bool doisJ, Controle * control)
{

    listE = listEnt;
    listGEnt = listE->getListGen();
    contr = control;
    doisJog = doisJ;
    pontoParada = 0;
    podePassar = true;
    invocou = false;
    terminouInvocar = false;
    audioTocando = false;
    progressao = 0;
    ang = 0;
    iteP1 = (inimigos.getTipoIni())[4].inicio();
    iteP2 = (inimigos.getTipoIni())[5].inicio();
    ptrBV = new Barra_De_Vida*[1 + doisJog];
    glow = al_load_bitmap("glow.png");
    dGlow = al_load_bitmap("darkGlow2.png");

    timers= new ALLEGRO_TIMER*[1];
    timers[0] = al_create_timer(1.0/60);

    ptrM = NULL;
    ptrJ[0] = NULL;
    ptrJ[1] = NULL;


    numPlat=35;
    numPlat += 4;
    inicializaFase(control);

    string nome[] = {"F1", "S1-F1", "S2-F1", "ventoArvores"};
    audioFase.inicializaAudio(nome, 4);

}
Fase_1::~Fase_1()
{
    al_destroy_timer(timers[0]);
    if (timers != NULL)
        delete []timers;
    al_destroy_bitmap(dGlow);
    dGlow = NULL;
    al_destroy_bitmap(glow);
    glow = NULL;

}

void Fase_1::atualiza()
{
    if (!audioFase.audioTocando("F1") && podePassar && pontoParada >= 1)
    {
        audioFase.tocaAudio("F1", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("F1", 0.01, 1);
    }

    alpha += fade;
    ang++;
    if (alpha >= 255 && fade == 1)
    {
        fade = 0;
        alpha = 255;
    }
    else if (alpha <= 0 && fade == -1)
    {
        fade = 0;
        alpha = 0;
    }

    if (doisJog && !ptrJ[1]->getMorreu())
    {
        if (!ptrJ[0]->getMorreu())
        {
            posX = (ptrJ[0]->getPos().getEspaco().getX() + ptrJ[1]->getPos().getEspaco().getX())/2;
        }
        else
        {
            posX = ptrJ[1]->getPos().getEspaco().getX();
        }
    }
    else
    {
        posX = ptrJ[0]->getPos().getEspaco().getX();
    }

    Lista_Generica<Fumaca*>::Iterador iteFum;
    iteFum = listFum.inicio();
    for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
    {
        if ((*iteFum)->getFuncionando())
        {
            (*iteFum)->atualiza();
        }
    }
    if (progressao == 0)
    {
        carregaInimigos(0);
        progressao++;
    }
    if (ptrM->getPos().getEspaco().getX() + 1200 >= 3332 && progressao == 1)
    {
        carregaInimigos(1);
        progressao++;
    }
    else if (ptrM->getPos().getEspaco().getX() + 1200 >= 7400 && progressao == 2)
    {
        carregaInimigos(2);
        progressao++;
    }
    else if (ptrM->getPos().getEspaco().getX() + 1200 >= 11800 && progressao == 3)
    {
        carregaInimigos(3);
        progressao++;
    }




    if ((ptrM->getPos().getEspaco().getX() > 900 || !podePassar) && pontoParada == 0)
    {
        pontoParada1();
    }
    if ((ptrM->getPos().getEspaco().getX() > 17300 || !podePassar) && pontoParada == 1)
    {
        pontoParada2();
    }

    if (ptrJ[0]->getMorreu() && (alpha == 255 || (doisJog && !ptrJ[1]->getMorreu())))
    {
        listE->mataJogador(0);
        ptrJ[0]->getPos().getEspaco().setXY(-1000, -1000);
        ptrJ[0]->setFuncionando(false);
    }
    if (doisJog && ptrJ[1]->getMorreu())
    {
        listE->mataJogador(1);
        ptrJ[1]->getPos().getEspaco().setXY(-1000, -1000);
        ptrJ[1]->setFuncionando(false);
    }
    if (doisJog)
    {
        if (ptrJ[0]->getMorreu() && ptrJ[1]->getMorreu() && alpha == 255)
        {
            if (!podePassar)
            {
                listIniPonto.limpar();
                al_stop_timer(timers[0]);
                al_set_timer_count(timers[0], 0);
                podePassar = true;
                invocou = false;
                terminouInvocar = false;
                if (pontoParada == 0)
                {
                    audioFase.fadeTo("S1-F1", -0.05, 0);
                }
                else if (pontoParada == 1)
                {
                    audioFase.fadeTo("S2-F1", -0.05, 0);
                }
                if (jogo->getCheckpoint() != 0)
                {
                    if (!audioFase.audioTocando("F1"))
                        audioFase.tocaAudio("F1", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

                    audioFase.fadeTo("F1", 0.01, 1);
                }
            }
            iteP1 = (inimigos.getTipoIni())[4].inicio();
            iteP2 = (inimigos.getTipoIni())[5].inicio();

            listE->resetaLista();
            listE->resetaJogador(ptrJ[0], 0);
            listE->resetaJogador(ptrJ[1], 1);
            listE->resetaScore();
            if (jogo->getCheckpoint() != 0)
                jogo->carregaPosicao(listE->getListGen());
            atualizaCheckpoint(jogo->getCheckpoint());
        }
        else if (ptrJ[0]->getMorreu() && ptrJ[1]->getMorreu() && alpha != 255)
        {
            fade = 1;
        }
    }
    else if (ptrJ[0]->getMorreu() && !doisJog && alpha ==255)
    {
        if (!podePassar)
            {
            listIniPonto.limpar();
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);

            invocou = false;
            terminouInvocar = false;
            if (pontoParada == 0)
            {
                audioFase.fadeTo("S1-F1", -0.05, 0);
            }
            else if (pontoParada == 1)
            {
                audioFase.fadeTo("S2-F1", -0.05, 0);
            }
            if (jogo->getCheckpoint() != 0)
            {
                if (!audioFase.audioTocando("F1"))
                    audioFase.tocaAudio("F1", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

                audioFase.fadeTo("F1", 0.01, 1);
            }
            podePassar = true;
        }
        iteP1 = (inimigos.getTipoIni())[4].inicio();
        iteP2 = (inimigos.getTipoIni())[5].inicio();

        listE->resetaLista();
        listE->resetaJogador(ptrJ[0], 0);
        listE->resetaScore();
        if (jogo->getCheckpoint() != 0)
            jogo->carregaPosicao(listE->getListGen());
        atualizaCheckpoint(jogo->getCheckpoint());
    }
    else if (ptrJ[0]->getMorreu() && !doisJog && alpha != 255)
    {
        fade = 1;
    }

    if (posX >= 7200 && jogo->getCheckpoint() <= 1)
    {
        jogo->setCheckpoint(2);
        listE->salvaScore();
        jogo->salvaPosicao(listE->getListGen());
    }
    else if (posX >= 11000 && jogo->getCheckpoint() <= 2)
    {
        jogo->setCheckpoint(3);
        listE->salvaScore();
        jogo->salvaPosicao(listE->getListGen());
    }
    else if (posX >= 16140 && jogo->getCheckpoint() <= 3)
    {
        jogo->setCheckpoint(4);
        listE->salvaScore();
        jogo->salvaPosicao(listE->getListGen());
    }


    atualizaEDano();
    ptrBV[0]->atualiza();
    if (doisJog)
        ptrBV[1]->atualiza();
    audioFase.atualizaAudio();
}
void Fase_1::paraFase(float x1, float x2)
{
    if (ptrM->getPos().getEspaco().getX() < x1)
    {
        ptrM->getPos().getEspaco().setX(x1);
    }
    else if (ptrM->getPos().getEspaco().getX() > x2 - 1200)
    {
        ptrM->getPos().getEspaco().setX(x2 - 1200);
    }
}
void Fase_1::desenha()
{

    Lista_Generica<Fumaca*>::Iterador iteFum;
    iteFum = listFum.inicio();
    for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
    {
        if ((*iteFum)->getFuncionando())
        {
            (*iteFum)->desenha();
        }
    }
    if (ptrM->getPos().getEspaco().getX() < 11000)
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,80 + 40*Math::coss[(ang/2)%360]), 0,0,948,666, 0,0,1200,720, 0);
    else if (ptrM->getPos().getEspaco().getX() >= 11000 && ptrM->getPos().getEspaco().getX() <= 12000)
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,80 + (((int)ptrM->getPos().getEspaco().getX())%11000)/10 + 40*Math::coss[(ang/2)%360]),
                                     0,0,948,666, 0,0,1200,720, 0);
    else
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,180 + 40*Math::coss[(ang/2)%360]), 0,0,948,666, 0,0,1200,720, 0);

    if (!ptrJ[0]->getMorreu())
    {
        al_draw_tinted_scaled_bitmap(glow, al_map_rgba(27+ 5*Math::coss[(ang+45)%360],50+ 5*Math::coss[(ang+45)%360],50+ 10*Math::coss[(ang+45)%360], 5 + 5*Math::coss[(ang)%360]), 0,0, 500,500,
                                     (ptrJ[0])->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     (ptrJ[0])->getPos().getEspaco().getY() - ptrM->getPos().getEspaco().getY() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     ((ptrJ[0])->getBorda().getY()/85.0)*500, ((ptrJ[0])->getBorda().getY()/85.0)*500, 0);
    }
    if (doisJog && !ptrJ[1]->getMorreu())
    {
        al_draw_tinted_scaled_bitmap(glow, al_map_rgba(27+ 5*Math::coss[(ang+45)%360],50+ 5*Math::coss[(ang+45)%360],50+ 10*Math::coss[(ang+45)%360], 5 + 5*Math::coss[(ang)%360]), 0,0, 500,500,
                                     (ptrJ[1])->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() - 250*((ptrJ[1])->getBorda().getY()/85.0),
                                     (ptrJ[1])->getPos().getEspaco().getY() - ptrM->getPos().getEspaco().getY() - 250*((ptrJ[1])->getBorda().getY()/85.0),
                                     ((ptrJ[1])->getBorda().getY()/85.0)*500, ((ptrJ[1])->getBorda().getY()/85.0)*500, 0);
    }


    desenhaEDano();
    ptrBV[0]->desenha();
    if (doisJog)
        ptrBV[1]->desenha();

    al_draw_filled_rectangle(0,0,1200,720, al_map_rgba(0,0,0,alpha));

    if (posX >= 19800 && alpha == 255)
    {
        audioFase.fadeTo("F1", -0.1, 0);
        audioFase.paraAudio("F1");
        passaFase(1);
    }
    else if (posX >= 19800 && alpha != 255)
    {
        fade = 1;
    }
    else if (posX < 19800 && alpha != 0)
    {

        if (doisJog)
        {
            if (!ptrJ[0]->getMorreu() || !ptrJ[0]->getMorreu())
                fade = -1;
        }
        else if (!ptrJ[0]->getMorreu())
            fade = -1;
    }



}

void Fase_1::inicializaFase(Controle * control)
{

    ptrM = new Mapa(10000, 420, "fase1.png", numPlat,1);

    listE->incluiEntidade(static_cast<Entidade*>(ptrM));
    listE->setMapa(ptrM);


    ptrJ[0] = new Jogador(0);
    ptrJ[0]->setControle(control);
    ptrJ[0]->setListaEnt(listE);
    ptrJ[0]->setMapaAtual(ptrM);
    ptrM->setJogador(ptrJ[0], 0);

    ptrBV[0] = new Barra_De_Vida();
    ptrBV[0]->setPtrJ(ptrJ[0]);
    ptrBV[0]->inicializa();

    if (doisJog)
    {

        ptrJ[1] = new Jogador(1);
        ptrJ[1]->setControle(control);
        ptrJ[1]->setListaEnt(listE);
        ptrJ[1]->setMapaAtual(ptrM);
        ptrM->setJogador(ptrJ[1], 1);

        ptrBV[1] = new Barra_De_Vida();
        ptrBV[1]->setPtrJ(ptrJ[1]);
        ptrBV[1]->inicializa();
    }


    listE->incluiEntidade(static_cast<Entidade*>(ptrJ[0]));
    ptrJ[0]->inicializaEntidade();
    if (doisJog)
    {
        listE->incluiEntidade(static_cast<Entidade*>(ptrJ[1]));
        ptrJ[1]->inicializaEntidade();
    }
    inicializaInimigo();
    inicializaObstaculos();
    inicializaPlataformas();
    inicializaFumaca();
    inicializaEDano();

}
void Fase_1::inicializaInimigo()
{
    Inimigo *ptrI = NULL;

    for (int i = 0; i < NUM_MORCEGO_FASE_1; i ++)
    {
        ptrI=new Morcego(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listMorcego.inclui_frente(static_cast<Morcego*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_BESOURO_FASE_1; i ++)
    {
        ptrI=new Besouro(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listBesouro.inclui_frente(static_cast<Besouro*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_GOBLIN_FASE_1; i ++)
    {
        ptrI=new Goblin(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listGoblin.inclui_frente(static_cast<Goblin*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_ESQUELETO_FASE_1; i ++)
    {
        ptrI=new Esqueleto(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listEsqueleto.inclui_frente(static_cast<Esqueleto*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_SLIME_FASE_1; i ++)
    {
        ptrI=new Slime(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listSlime.inclui_frente(static_cast<Slime*>(ptrI));
        ptrI->inicializaEntidade();
    }
}
void Fase_1::carregaInimigos(int t)
{
    Lista_Generica<Vetor_R2> *posI = inimigos.getPosIni();
    Lista_Generica<int> *tipoI =  inimigos.getTipoIni();
    Lista_Generica<Vetor_R2>::Iterador itePosI;
    itePosI = posI[t].inicio();
    Lista_Generica<int>::Iterador iteTipo;
    iteTipo = tipoI[t].inicio();

    Lista_Generica<Slime*>::Iterador iteSli;
    int iS = 0;
    iteSli = listSlime.inicio();
    Lista_Generica<Besouro*>::Iterador iteBes;
    int iB = 0;
    iteBes = listBesouro.inicio();
    Lista_Generica<Goblin*>::Iterador iteGob;
    int iG = 0;
    iteGob = listGoblin.inicio();
    Lista_Generica<Morcego*>::Iterador iteMor;
    int iM = 0;
    iteMor = listMorcego.inicio();
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    int iE = 0;
    iteEsq = listEsqueleto.inicio();

    for (int i = 0; i <  tipoI[t].tamanho(); i++, itePosI++, iteTipo++)
    {
        switch (*iteTipo)
        {
        case SLIME_1:
            for (; iS < listSlime.tamanho(); iS++, iteSli++)
            {
                if(!(*iteSli)->getFuncionando())
                {
                    (*iteSli)->getPos().setEspaco((*itePosI));
                    (*iteSli)->setFuncionando(true);
                    iS++;
                    iteSli++;
                    break;
                }
            }
            break;
        case BESOURO_1:
            for (; iB < listBesouro.tamanho(); iB++, iteBes++)
            {
                if(!(*iteBes)->getFuncionando())
                {
                    (*iteBes)->getPos().setEspaco((*itePosI));
                    (*iteBes)->setFuncionando(true);
                    iB++;
                    iteBes++;
                    break;
                }
            }
            break;
        case GOBLIN_1:
            for (; iG < listGoblin.tamanho(); iG++, iteGob++)
            {
                if(!(*iteGob)->getFuncionando())
                {
                    (*iteGob)->getPos().setEspaco((*itePosI));
                    (*iteGob)->setFuncionando(true);
                    iG++;
                    iteGob++;
                    break;
                }
            }
            break;
        case MORCEGO_1:
            for (; iM < listMorcego.tamanho(); iM++, iteMor++)
            {
                if(!(*iteMor)->getFuncionando())
                {
                    (*iteMor)->getPos().setEspaco((*itePosI));
                    (*iteMor)->setFuncionando(true);
                    iM++;
                    iteMor++;
                    break;
                }
            }
            break;
        case ESQUELETO_1:
            for (; iE < listEsqueleto.tamanho(); iE++, iteEsq++)
            {
                if(!(*iteEsq)->getFuncionando())
                {
                    (*iteEsq)->getPos().setEspaco((*itePosI));
                    (*iteEsq)->setFuncionando(true);
                    iE++;
                    iteEsq++;
                    break;
                }
            }
            break;
        }
    }
}
void Fase_1::inicializaObstaculos()
{
    Obstaculo* ptrArm;
    for (int i = 0; i < NUM_ARMADILHAS; i++)
    {
        ptrArm = new Armadilha_Urso(-200, -200);
        ptrArm->setJogador(ptrJ[0],0);
        ptrArm->setMapaAtual(ptrM);
        {
            ptrArm->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrArm));
        listArmadilhas.inclui_frente(static_cast<Armadilha_Urso*>(ptrArm));
    }
    resetaArmadilhas();
}
void Fase_1::resetaArmadilhas()
{
    Lista_Generica<Armadilha_Urso*>::Iterador iteA;
    iteA = listArmadilhas.inicio();

    Lista_Generica<Vetor_R2> *posA;
    posA = inimigos.getPosArmadilhas();
    Lista_Generica<Vetor_R2>::Iterador itePosA;
    itePosA = posA[0].inicio();

    iteA = listArmadilhas.inicio();
    for (int i = 0; i < posA[0].tamanho(); i++, itePosA++, iteA++)
    {
        (*iteA)->getPos().setEspaco((*itePosA));
        (*iteA)->setFuncionando(true);
    }

}
void Fase_1::inicializaPlataformas()
{

    tipoPlat=new int[numPlat];

    platX=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platX[i]=new int[2];
    platY=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platY[i]=new int[2];
    platX[0][0]=0;
    platY[0][0]=674;
    platX[0][1]=3334;
    platY[0][1]=840;
    tipoPlat[0]=0;
    platX[1][0]=888;
    platY[1][0]=512;
    platX[1][1]=2114;
    platY[1][1]=546;
    tipoPlat[1]=1;
    platX[2][0]=1168;
    platY[2][0]=324;
    platX[2][1]=1782;
    platY[2][1]=340;
    tipoPlat[2]=1;
    platX[3][0]=3334;
    platY[3][0]=400;
    platX[3][1]=6440;
    platY[3][1]=840;
    tipoPlat[3]=0;
    platX[4][0]=6440;
    platY[4][0]=618;
    platX[4][1]=7260;
    platY[4][1]=840;
    tipoPlat[4]=0;
    platX[5][0]=7940;
    platY[5][0]=690;
    platX[5][1]=8276;
    platY[5][1]=840;
    tipoPlat[5]=0;
    platX[6][0]=8582;
    platY[6][0]=690;
    platX[6][1]=8918;
    platY[6][1]=840;
    tipoPlat[6]=0;
    platX[7][0]=9360;
    platY[7][0]=700;
    platX[7][1]=9804;
    platY[7][1]=724;
    tipoPlat[7]=1;
    platX[8][0]=9360;
    platY[8][0]=404;
    platX[8][1]=9804;
    platY[8][1]=428;
    tipoPlat[8]=1;
    platX[9][0]=9804;
    platY[9][0]=282;
    platX[9][1]=11482;
    platY[9][1]=840;
    tipoPlat[9]=0;
    platX[10][0]=11996;
    platY[10][0]=704;
    platX[10][1]=14902;
    platY[10][1]=840;
    tipoPlat[10]=0;
    platX[11][0]=15576;
    platY[11][0]=534;
    platX[11][1]=16272;
    platY[11][1]=840;
    tipoPlat[11]=0;
    platX[12][0]=17655;
    platY[12][0]=716;
    platX[12][1]=17886;
    platY[12][1]=840;
    tipoPlat[12]=0;
    platX[13][0]=18552;
    platY[13][0]=716;
    platX[13][1]=18784;
    platY[13][1]=840;
    tipoPlat[13]=0;
    platX[14][0]=17240;
    platY[14][0]=536;
    platX[14][1]=19146;
    platY[14][1]=560;
    tipoPlat[14]=1;
    platX[15][0]=17570;
    platY[15][0]=362;
    platX[15][1]=18866;
    platY[15][1]=378;
    tipoPlat[15]=1;
    platX[16][0]=17776;
    platY[16][0]=184;
    platX[16][1]=18662;
    platY[16][1]=200;
    tipoPlat[16]=1;
    platX[17][0]=19296;
    platY[17][0]=704;
    platX[17][1]=19896;
    platY[17][1]=840;
    tipoPlat[17]=0;
    platX[18][0]=2832;
    platY[18][0]=529;
    platX[18][1]=3334;
    platY[18][1]=545;
    tipoPlat[18]=1;
    platX[19][0]=12795;
    platY[19][0]=514;
    platX[19][1]=13949;
    platY[19][1]=548;
    tipoPlat[19]=1;


    platX[20][0]=platX[4][1];
    platY[20][0]=platY[4][0]-20;
    platX[20][1]=platX[4][1]+20;
    platY[20][1]=platY[4][0];
    tipoPlat[20]=3;



    platX[21][0]=platX[9][0]-20;
    platY[21][0]=platY[9][0]-20;
    platX[21][1]=platX[9][0];
    platY[21][1]=platY[9][0];
    tipoPlat[21]=3;


    platX[22][0]=platX[9][1];
    platY[22][0]=platY[9][0]-20;
    platX[22][1]=platX[9][1]+20;
    platY[22][1]=platY[9][0];
    tipoPlat[22]=3;
    platX[23][0]=platX[10][0]-20;
    platY[23][0]=platY[10][0]-20;
    platX[23][1]=platX[10][0];
    platY[23][1]=platY[10][0];
    tipoPlat[23]=3;
    platX[24][0]=platX[10][1];
    platY[24][0]=platY[10][0]-20;
    platX[24][1]=platX[10][1]+20;
    platY[24][1]=platY[10][0];
    tipoPlat[24]=3;

    platX[25][0]=platX[11][0]-20;
    platY[25][0]=platY[11][0]-20;
    platX[25][1]=platX[11][0];
    platY[25][1]=platY[11][0];
    tipoPlat[25]=3;
    platX[26][0]=platX[11][1];
    platY[26][0]=platY[11][0]-20;
    platX[26][1]=platX[11][1]+20;
    platY[26][1]=platY[11][0];
    tipoPlat[26]=3;

    platX[27][0]=platX[12][0]-20;
    platY[27][0]=platY[12][0]-20;
    platX[27][1]=platX[12][0];
    platY[27][1]=platY[12][0];
    tipoPlat[27]=3;
    platX[28][0]=platX[12][1];
    platY[28][0]=platY[12][0]-20;
    platX[28][1]=platX[12][1]+20;
    platY[28][1]=platY[12][0];
    tipoPlat[28]=3;

    platX[29][0]=platX[13][0]-20;
    platY[29][0]=platY[13][0]-20;
    platX[29][1]=platX[13][0];
    platY[29][1]=platY[13][0];
    tipoPlat[29]=3;
    platX[30][0]=platX[13][1];
    platY[30][0]=platY[13][0]-20;
    platX[30][1]=platX[13][1]+20;
    platY[30][1]=platY[13][0];
    tipoPlat[30]=3;

    platX[31][0]=platX[14][0]-20;
    platY[31][0]=platY[14][0]-20;
    platX[31][1]=platX[14][0];
    platY[31][1]=platY[14][0];
    tipoPlat[31]=3;
    platX[32][0]=platX[14][1];
    platY[32][0]=platY[14][0]-20;
    platX[32][1]=platX[14][1]+20;
    platY[32][1]=platY[14][0];
    tipoPlat[32]=3;

    platX[33][0]=platX[14][0];
    platY[33][0]=platY[14][0]-1;
    platX[33][1]=platX[14][1];
    platY[33][1]=platY[14][1];
    tipoPlat[33]=3;

    platX[34][0]=platX[17][0]-20;
    platY[34][0]=platY[17][0]-20;
    platX[34][1]=platX[17][0];
    platY[34][1]=platY[17][0];
    tipoPlat[34]=3;

    for(int i = 0; i < numPlat - 4; i++)
    {
        for(int j = 0; j < 2; j++)
            ptrM->getPlat(i).getPos(j).setXY(platX[i][j], platY[i][j]);
        ptrM->getPlat(i).setTipo(tipoPlat[i]);
    }
}
void Fase_1::pontoParada1()
{
    paraFase(800, 2200);
    if (podePassar)
    {
        podePassar = false;
        al_set_timer_count(timers[0], 0);
        al_start_timer(timers[0]);
        audioFase.tocaAudio("S1-F1", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("S1-F1", 0.0016, 1);
    }
    if (al_get_timer_count(timers[0]) == 10*60 && !invocou)
    {
        for (int i = 0; i < 10; i++, iteP1++)
        {
            if ((*iteP1) == SLIME_1 || (*iteP1) == GOBLIN_1)
                invocaInimigo((*iteP1), Vetor_R2::vetor(800 + 1200*(rand()%2), 670));
            else if ((*iteP1) == BESOURO_1)
                invocaInimigo((*iteP1), Vetor_R2::vetor(800 + rand()%1200, 20));
        }
        invocou = true;
    }
    if (al_get_timer_count(timers[0]) > 10*60 && al_get_timer_count(timers[0]) <= 40*60 && (al_get_timer_count(timers[0])-5*60)%180 == 0)
    {
        if ((*iteP1) == SLIME_1 || (*iteP1) == GOBLIN_1)
            invocaInimigo((*iteP1), Vetor_R2::vetor(900 + 1200*(rand()%2), 670));
        else if ((*iteP1) == BESOURO_1)
            invocaInimigo((*iteP1), Vetor_R2::vetor(800 + rand()%1400, 20));
        iteP1++;
    }
    else if (al_get_timer_count(timers[0]) > 40*60 && al_get_timer_count(timers[0]) <= 70*60 && (al_get_timer_count(timers[0])-40*60)%90 == 0)
    {
        if ((*iteP1) == SLIME_1 || (*iteP1) == GOBLIN_1)
            invocaInimigo((*iteP1), Vetor_R2::vetor(900 + 1200*(rand()%2), 670));
        else if ((*iteP1) == BESOURO_1)
            invocaInimigo((*iteP1), Vetor_R2::vetor(800 + rand()%1400, 20));
        iteP1++;
    }
    else if (al_get_timer_count(timers[0]) > 70*60)
    {
        terminouInvocar = true;
    }
    if (terminouInvocar)
    {
        int i;
        Lista_Generica<Inimigo*>::Iterador iteI;
        iteI = listIniPonto.inicio();
        for (i = 0; i < listIniPonto.tamanho(); i++, iteI++)
        {
            if ((*iteI)->getFuncionando() &&
                (*iteI)->getPos().getEspaco().getX() < 2200 &&
                (*iteI)->getPos().getEspaco().getX() > 800  &&
                (*iteI)->getPos().getEspaco().getY() >= 0 &&
                (*iteI)->getPos().getEspaco().getY() <= 840)
                break;
        }
        if (i == listIniPonto.tamanho())
        {
            listIniPonto.limpar();
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);
            pontoParada++;
            podePassar = true;
            invocou = false;
            terminouInvocar = false;
            audioFase.tocaAudio("F1", 0.001, ALLEGRO_PLAYMODE_LOOP, 0.0);
            audioFase.fadeTo("F1", 0.01, 1);
            audioFase.fadeTo("S1-F1", -0.01, 0);
            jogo->setCheckpoint(1);
            jogo->salvaPosicao(listE->getListGen());
            listE->salvaScore();
        }
    }
}
void Fase_1::pontoParada2()
{
    paraFase(17200 , 19200);
    if (podePassar)
    {
        podePassar = false;
        al_set_timer_count(timers[0], 0);
        al_start_timer(timers[0]);
        audioFase.fadeTo("F1", -0.01, 0.001);
        audioFase.tocaAudio("S2-F1", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("S2-F1", 0.0016, 1);
    }
    if (al_get_timer_count(timers[0]) == 10*60 && !invocou)
    {
        for (int i = 0; i < 10; i++, iteP2++)
        {
            if ((*iteP2) == SLIME_1 || (*iteP2) == GOBLIN_1 || (*iteP2) == ESQUELETO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17300 + 1800*(rand()%2), 520));
            else if ((*iteP2) == BESOURO_1 || (*iteP2) == MORCEGO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17200 + rand()%2000, 20));
        }
        invocou = true;
    }
    if (al_get_timer_count(timers[0]) > 10*60 && al_get_timer_count(timers[0]) <= 40*60 && (al_get_timer_count(timers[0])-5*60)%180 == 0)
    {
        if ((*iteP2) == SLIME_1 || (*iteP2) == GOBLIN_1 || (*iteP2) == ESQUELETO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17300 + 1800*(rand()%2), 520));
            else if ((*iteP2) == BESOURO_1 || (*iteP2) == MORCEGO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17200 + rand()%2000, 20));
        iteP2++;
    }
    else if (al_get_timer_count(timers[0]) > 40*60 && al_get_timer_count(timers[0]) <= 75*60 && (al_get_timer_count(timers[0])-40*60)%84 == 0)
    {
        if ((*iteP2) == SLIME_1 || (*iteP2) == GOBLIN_1 || (*iteP2) == ESQUELETO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17300 + 1800*(rand()%2), 520));
            else if ((*iteP2) == BESOURO_1 || (*iteP2) == MORCEGO_1)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17200 + rand()%2000, 20));
        iteP2++;
    }
    else if (al_get_timer_count(timers[0]) > 75*60)
    {
        terminouInvocar = true;
    }
    if (terminouInvocar)
    {
        int i;
        Lista_Generica<Inimigo*>::Iterador iteI;
        iteI = listIniPonto.inicio();
        for (i = 0; i < listIniPonto.tamanho(); i++, iteI++)
        {
            if ((*iteI)->getFuncionando()&&
                (*iteI)->getPos().getEspaco().getX() < 19200 &&
                (*iteI)->getPos().getEspaco().getX() > 17200  &&
                (*iteI)->getPos().getEspaco().getY() >= 0 &&
                (*iteI)->getPos().getEspaco().getY() <= 840)
                break;
        }
        if (i == listIniPonto.tamanho())
        {
            listIniPonto.limpar();
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);
            pontoParada++;
            podePassar = true;
            invocou = false;
            terminouInvocar = false;
            audioFase.fadeTo("F1", 0.01, 1);
            audioFase.fadeTo("S2-F1", -0.01, 0);

        }
    }
}
void Fase_1::invocaInimigo(int t, Vetor_R2 p)
{
    Lista_Generica<Slime*>::Iterador iteSli;
    iteSli = listSlime.inicio();
    Lista_Generica<Besouro*>::Iterador iteBes;
    iteBes = listBesouro.inicio();
    Lista_Generica<Goblin*>::Iterador iteGob;
    iteGob = listGoblin.inicio();
    Lista_Generica<Morcego*>::Iterador iteMor;
    iteMor = listMorcego.inicio();
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    iteEsq = listEsqueleto.inicio();

    switch (t)
    {
    case SLIME_1:
        for (int i = 0 ; i < listSlime.tamanho(); i++, iteSli++)
        {
            if(!(*iteSli)->getFuncionando())
            {
                (*iteSli)->getPos().setEspaco(p);
                (*iteSli)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteSli));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteSli)->getPos().getEspaco());
                        (*iteFum)->setAreaEfetiva((*iteSli)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case BESOURO_1:
        for (int i = 0 ; i < listBesouro.tamanho(); i++, iteBes++)
        {
            if(!(*iteBes)->getFuncionando())
            {
                (*iteBes)->getPos().setEspaco(p);
                (*iteBes)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteBes));

                break;
            }
        }
        break;
    case GOBLIN_1:
        for (int i = 0 ; i < listGoblin.tamanho(); i++, iteGob++)
        {
            if(!(*iteGob)->getFuncionando())
            {
                (*iteGob)->getPos().setEspaco(p);
                (*iteGob)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteGob));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteGob)->getPos().getEspaco() + Vetor_R2::vetor(-75,-70));
                        (*iteFum)->setAreaEfetiva((*iteGob)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case MORCEGO_1:
        for (int i = 0 ; i < listMorcego.tamanho(); i++, iteMor++)
        {
            if(!(*iteMor)->getFuncionando())
            {
                (*iteMor)->getPos().setEspaco(p);
                (*iteMor)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteMor));

                break;
            }
        }
        break;
    case ESQUELETO_1:
        for (int i = 0 ; i < listEsqueleto.tamanho(); i++, iteEsq++)
        {
            if(!(*iteEsq)->getFuncionando())
            {
                (*iteEsq)->getPos().setEspaco(p);
                (*iteEsq)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteEsq));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteEsq)->getPos().getEspaco() + Vetor_R2::vetor(-75,-70));
                        (*iteFum)->setAreaEfetiva((*iteEsq)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    }
}
void Fase_1::atualizaCheckpoint(int c)
{
    if (c == 0)
    {
        ptrJ[0]->getPos().getEspaco().setXY(100,560);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(150,560);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }


        ptrM->getPos().getEspaco().setX(0);
        progressao = 0;
        pontoParada = 0;
    }
    if (c == 1)
    {
        ptrJ[0]->getPos().getEspaco().setXY(1460,630);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(1460,630);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }


        ptrM->getPos().getEspaco().setX(1460-600);
        progressao = 1;
        pontoParada = 1;
    }
    else if (c == 2)
    {
        ptrJ[0]->getPos().getEspaco().setXY(7200,618 - ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(7200,618 - ptrJ[1]->getBorda().getY());
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }


        ptrM->getPos().getEspaco().setX(7200-600);
        progressao = 3;
        pontoParada = 1;
    }
    else if (c == 3)
    {
        ptrJ[0]->getPos().getEspaco().setXY(11000, 282 - ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(11000, 282 - ptrJ[0]->getBorda().getY());
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }


        ptrM->getPos().getEspaco().setX(11000-600);
        progressao = 3;
        pontoParada = 1;
    }
    else if (c == 4)
    {
        ptrJ[0]->getPos().getEspaco().setXY(16140,534- ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(16140,534- ptrJ[0]->getBorda().getY());
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }


        ptrM->getPos().getEspaco().setX(16140-600);
        progressao = 4;
        pontoParada = 1;
    }
    resetaArmadilhas();
    comecouFase = false;
}

