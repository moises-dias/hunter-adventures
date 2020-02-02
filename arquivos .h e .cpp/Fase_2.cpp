#include "Fase_2.h"
#include "Fumaca.h"
#include "Barra_De_Vida.h"
#include "Jogo.h"
#include "Lava.h"

Fase_2::Fase_2()
{

}
Fase_2::Fase_2(Lista_Entidades * listEnt, bool doisJ, Controle * control)
{
    listE = listEnt;
    listGEnt = listE->getListGen();
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

    string nome[] = {"F2", "S1-F2", "S2-F2"};
    audioFase.inicializaAudio(nome, 3);

    ptrM = NULL;
    ptrJ[0] = NULL;
    ptrJ[1] = NULL;


    numPlat=31;
    numPlat += 4;
    inicializaFase(control);

}
Fase_2::~Fase_2()
{
    al_destroy_bitmap(dGlow);
    dGlow = NULL;
    al_destroy_bitmap(glow);
    glow = NULL;

    al_destroy_timer(timers[0]);
    if (timers != NULL)
        delete [] timers;
}

void Fase_2::atualiza()
{
    alpha += fade;
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

    if (!audioTocando)
    {
        audioFase.tocaAudio("F2", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("F2", 0.005, 1);
        audioTocando = true;
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
    if (ptrM->getPos().getEspaco().getX() + 1200 >= 3150 && progressao == 1)
    {
        carregaInimigos(1);
        progressao++;
    }
    else if (ptrM->getPos().getEspaco().getX() + 1200 >= 11200 && progressao == 2)
    {
        carregaInimigos(2);
        progressao++;
    }
    else if (ptrM->getPos().getEspaco().getX() + 1200 >= 14500 && progressao == 3)
    {
        carregaInimigos(3);
        progressao++;
    }
    if ((ptrM->getPos().getEspaco().getX() > 8750 || !podePassar) && pontoParada == 0)
    {
        pontoParada1();
    }
    if ((ptrM->getPos().getEspaco().getX() > 17600 || !podePassar) && pontoParada == 1)
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
                    audioFase.fadeTo("S1-F2", -0.05, 0);
                }
                else if (pontoParada == 1)
                {
                    audioFase.fadeTo("S2-F2", -0.05, 0);
                }
                if (!audioFase.audioTocando("F2"))
                    audioFase.tocaAudio("F2", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

                audioFase.fadeTo("F2", 0.01, 1);

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
                audioFase.fadeTo("S1-F2", -0.05, 0);
            }
            else if (pontoParada == 1)
            {
                audioFase.fadeTo("S2-F2", -0.05, 0);
            }
            if (!audioFase.audioTocando("F2"))
                audioFase.tocaAudio("F2", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

            audioFase.fadeTo("F2", 0.01, 1);
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

    if (posX >= 3100 && jogo->getCheckpoint() <= 0)
    {
        jogo->setCheckpoint(1);
        jogo->salvaPosicao(listE->getListGen());
        listE->salvaScore();
    }
    else if (posX >= 15690 && jogo->getCheckpoint() <= 2)
    {
        jogo->setCheckpoint(3);
        jogo->salvaPosicao(listE->getListGen());
        listE->salvaScore();
    }
    ang++;
    atualizaEDano();
    audioFase.atualizaAudio();
    ptrBV[0]->atualiza();
    if (doisJog)
        ptrBV[1]->atualiza();
}
void Fase_2::desenha()
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
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,180 + 40*Math::coss[(ang/2)%360]), 0,0,948,666, 0,0,1200,720, 0);
    else if (ptrM->getPos().getEspaco().getX() >= 11000 && ptrM->getPos().getEspaco().getX() <= 12000)
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,180 - (((int)ptrM->getPos().getEspaco().getX())%11000)/10 + 40*Math::coss[(ang/2)%360]),
                                     0,0,948,666, 0,0,1200,720, 0);
    else
        al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(255,100,100,80 + 40*Math::coss[(ang/2)%360]), 0,0,948,666, 0,0,1200,720, 0);

    if (ptrJ[0] != NULL)
    {
        al_draw_tinted_scaled_bitmap(glow, al_map_rgba(27+ 5*Math::coss[(ang+45)%360],50+ 5*Math::coss[(ang+45)%360],50+ 10*Math::coss[(ang+45)%360], 5 + 5*Math::coss[(ang)%360]), 0,0, 500,500,
                                     (ptrJ[0])->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     (ptrJ[0])->getPos().getEspaco().getY() - ptrM->getPos().getEspaco().getY() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     ((ptrJ[0])->getBorda().getY()/85.0)*500, ((ptrJ[0])->getBorda().getY()/85.0)*500, 0);
    }
    if (ptrJ[1] != NULL)
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
        audioFase.fadeTo("F2", -0.1, 0);
        audioFase.paraAudio("F2");
        passaFase(2);
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
void Fase_2::inicializaFase(Controle * control)
{
    ptrM = new Mapa(10000, 420, NULL, numPlat,2);

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
        ptrJ[1]->setMapaAtual(ptrM);
        ptrJ[1]->setListaEnt(listE);
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
void Fase_2::inicializaInimigo()
{
    Inimigo* ptrI = NULL;


    for (int i = 0; i < NUM_SLIME_FASE_2; i++)
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
    for (int i = 0; i < NUM_GOBLIN_FASE_2; i++)
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
    for (int i = 0; i < NUM_MORCEGO_FASE_2; i++)
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
    for (int i = 0; i < NUM_SLIME_LAVA_FASE_2; i++)
    {
        ptrI=new Lava_Slime(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listLavaSlime.inclui_frente(static_cast<Lava_Slime*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_ESQUELETO_FASE_2; i++)
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
    for (int i = 0; i < NUM_FOGO_FATUO_FASE_2; i++)
    {
        ptrI=new Fogo_Fatuo(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listFogoFatuo.inclui_frente(static_cast<Fogo_Fatuo*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_GOLEM_FASE_2; i++)
    {
        ptrI=new Golem(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listGolem.inclui_frente(static_cast<Golem*>(ptrI));
        ptrI->inicializaEntidade();
    }
}
void Fase_2::inicializaObstaculos()
{
    Lava* ptrLav;
    for (int i = 0; i < NUM_LAVA; i++)
    {
        ptrLav = new Lava(-200, -200);
        ptrLav->setJogador(ptrJ[0],0);
        ptrLav->setListaEnt(listE);
        ptrLav->setMapaAtual(ptrM);
        {
            ptrLav->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrLav));
        listLava.inclui_frente((ptrLav));
        ptrLav->inicializaEntidade();
    }
    resetaObstaculos();
}
void Fase_2::resetaObstaculos()
{
    Lista_Generica<Lava*>::Iterador iteL;
    iteL = listLava.inicio();

    Lista_Generica<Vetor_R2> *posL;
    posL = inimigos.getPosLava();
    Lista_Generica<Vetor_R2>::Iterador itePosL;
    itePosL = posL[0].inicio();

    for (int i = 0; i < NUM_LAVA; i++, iteL++)
    {
        if ((*iteL)->getFuncionando() == true)
        {
            (*iteL)->setFuncionando(false);
            (*iteL)->resetaEntidade();
        }
    }
    iteL = listLava.inicio();
    for (int i = 0; i < NUM_LAVA; i++, itePosL++, iteL++)
    {
        (*iteL)->getPos().setEspaco((*itePosL));
        if (i == 0)
        {
            (*iteL)->getBorda().setXY((16764 - 15918)/2, 11);
        }
        else if (i == 1)
        {
            (*iteL)->getBorda().setXY((14006 - 12662)/2, 11);
        }
        else if (i == 2)
        {
            (*iteL)->getBorda().setXY((19448 - 17498)/2, 11);
        }
        (*iteL)->setFuncionando(true);
    }
}
void Fase_2::inicializaPlataformas()
{
    tipoPlat=new int[numPlat];

    platX=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platX[i]=new int[2];
    platY=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platY[i]=new int[2];

    platX[0][0]=0;
    platY[0][0]=700;
    platX[0][1]=2006;
    platY[0][1]=840;
    tipoPlat[0]=0;

    platX[1][0]=842;
    platY[1][0]=486;
    platX[1][1]=1654;
    platY[1][1]=516;
    tipoPlat[1]=1;

    platX[2][0]=2480;
    platY[2][0]=368;
    platX[2][1]=3344;
    platY[2][1]=840;
    tipoPlat[2]=0;

    platX[3][0]=4070;
    platY[3][0]=616;
    platX[3][1]=5980;
    platY[3][1]=840;
    tipoPlat[3]=0;

    platX[4][0]=5980;
    platY[4][0]=616;
    platX[4][1]=6946;
    platY[4][1]=628;
    tipoPlat[4]=1;

    platX[5][0]=6946;
    platY[5][0]=616;
    platX[5][1]=7798;
    platY[5][1]=840;
    tipoPlat[5]=0;

    platX[6][0]=8680;
    platY[6][0]=704;
    platX[6][1]=9288;
    platY[6][1]=734;
    tipoPlat[6]=1;

    platX[7][0]=9224;
    platY[7][0]=452;
    platX[7][1]=10106;
    platY[7][1]=467;
    tipoPlat[7]=1;

    platX[8][0]=10038;
    platY[8][0]=706;
    platX[8][1]=10646;
    platY[8][1]=736;
    tipoPlat[8]=1;

    platX[9][0]=11684;
    platY[9][0]=588;
    platX[9][1]=12650;
    platY[9][1]=840;
    tipoPlat[9]=0;

    platX[10][0]=12662;
    platY[10][0]=594;
    platX[10][1]=14006;
    platY[10][1]=840;
    tipoPlat[10]=0;

    platX[11][0]=14008;
    platY[11][0]=590;
    platX[11][1]=15446;
    platY[11][1]=840;
    tipoPlat[11]=0;

    platX[12][0]=15440;
    platY[12][0]=362;
    platX[12][1]=15916;
    platY[12][1]=840;
    tipoPlat[12]=0;

    platX[13][0]=15918;
    platY[13][0]=366;
    platX[13][1]=16764;
    platY[13][1]=840;
    tipoPlat[13]=0;

    platX[14][0]=16766;
    platY[14][0]=364;
    platX[14][1]=17194;
    platY[14][1]=840;
    tipoPlat[14]=0;

    platX[15][0]=17194;
    platY[15][0]=584;
    platX[15][1]=17496;
    platY[15][1]=840;
    tipoPlat[15]=0;

    platX[16][0]=17498;
    platY[16][0]=684;
    platX[16][1]=19448;
    platY[16][1]=840;
    tipoPlat[16]=0;

    platX[17][0]=17498;
    platY[17][0]=584;
    platX[17][1]=19460;
    platY[17][1]=614;
    tipoPlat[17]=1;

    platX[18][0]=17846;
    platY[18][0]=380;
    platX[18][1]=19138;
    platY[18][1]=395;
    tipoPlat[18]=1;

    platX[19][0]=19460;
    platY[19][0]=584;
    platX[19][1]=19998;
    platY[19][1]=840;
    tipoPlat[19]=0;

    platX[20][0]=platX[0][1];
    platY[20][0]=platY[0][0]-20;
    platX[20][1]=platX[0][1]+20;
    platY[20][1]=platY[0][0];
    tipoPlat[20]=3;

    platX[21][0]=platX[2][0]-20;
    platY[21][0]=platY[2][0]-20;
    platX[21][1]=platX[2][0];
    platY[21][1]=platY[2][0];
    tipoPlat[21]=3;

    platX[22][0]=platX[2][1];
    platY[22][0]=platY[2][0]-20;
    platX[22][1]=platX[2][1]+20;
    platY[22][1]=platY[2][0];
    tipoPlat[22]=3;

    platX[23][0]=platX[3][0]-20;
    platY[23][0]=platY[3][0]-20;
    platX[23][1]=platX[3][0];
    platY[23][1]=platY[3][0];
    tipoPlat[23]=3;

    platX[24][0]=platX[5][1];
    platY[24][0]=platY[5][0]-20;
    platX[24][1]=platX[5][1]+20;
    platY[24][1]=platY[5][0];
    tipoPlat[24]=3;

    platX[25][0]=platX[6][1];
    platY[25][0]=platY[6][0]-20;
    platX[25][1]=platX[6][1]+20;
    platY[25][1]=platY[6][0];
    tipoPlat[25]=3;

    platX[26][0]=platX[6][0]-20;
    platY[26][0]=platY[6][0]-20;
    platX[26][1]=platX[6][0];
    platY[26][1]=platY[6][0];
    tipoPlat[26]=3;

    platX[27][0]=platX[7][0];
    platY[27][0]=platY[7][0]-1;
    platX[27][1]=platX[7][1];
    platY[27][1]=platY[7][1];
    tipoPlat[27]=3;

    platX[28][0]=platX[8][1];
    platY[28][0]=platY[8][0]-20;
    platX[28][1]=platX[8][1]+20;
    platY[28][1]=platY[8][0];
    tipoPlat[28]=3;

    platX[29][0]=platX[8][0]-20;
    platY[29][0]=platY[8][0]-20;
    platX[29][1]=platX[8][0];
    platY[29][1]=platY[8][0];
    tipoPlat[29]=3;

    platX[30][0]=platX[9][0]-20;
    platY[30][0]=platY[9][0]-20;
    platX[30][1]=platX[9][0];
    platY[30][1]=platY[9][0];
    tipoPlat[30]=3;

    for(int i = 0; i < numPlat - 4; i++)
    {
        for(int j = 0; j < 2; j++)
            ptrM->getPlat(i).getPos(j).setXY(platX[i][j], platY[i][j]);
        ptrM->getPlat(i).setTipo(tipoPlat[i]);
    }
}
void Fase_2::paraFase(float x1, float x2)
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
void Fase_2::carregaInimigos(int t)
{
    Lista_Generica<Vetor_R2> *posI = inimigos.getPosIni();
    Lista_Generica<int> *tipoI =  inimigos.getTipoIni();
    Lista_Generica<Vetor_R2>::Iterador itePosI;
    itePosI = posI[t].inicio();
    Lista_Generica<int>::Iterador iteTipo;
    iteTipo = tipoI[t].inicio();

    Lista_Generica<Slime*>::Iterador iteSli;
    iteSli = listSlime.inicio();
    int iS = 0;
    Lista_Generica<Goblin*>::Iterador iteGob;
    iteGob = listGoblin.inicio();
    int iGb = 0;
    Lista_Generica<Morcego*>::Iterador iteMor;
    iteMor = listMorcego.inicio();
    int iM = 0;
    Lista_Generica<Lava_Slime*>::Iterador iteLsl;
    iteLsl = listLavaSlime.inicio();
    int iSl = 0;
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    iteEsq = listEsqueleto.inicio();
    int iE = 0;
    Lista_Generica<Fogo_Fatuo*>::Iterador iteFog;
    iteFog = listFogoFatuo.inicio();
    int iF = 0;
    Lista_Generica<Golem*>::Iterador iteGol;
    iteGol = listGolem.inicio();
    int iGl = 0;

    for (int i = 0; i <  tipoI[t].tamanho(); i++, itePosI++, iteTipo++)
    {
        switch (*iteTipo)
        {
        case SLIME_2:
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
        case GOBLIN_2:
            for (; iGb < listGoblin.tamanho(); iGb++, iteGob++)
            {
                if(!(*iteGob)->getFuncionando())
                {
                    (*iteGob)->getPos().setEspaco((*itePosI));
                    (*iteGob)->setFuncionando(true);
                    iGb++;
                    iteGob++;
                    break;
                }
            }
            break;
        case MORCEGO_2:
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
        case SLIME_LAVA_2:
            for (; iSl < listLavaSlime.tamanho(); iSl++, iteLsl++)
            {
                if(!(*iteLsl)->getFuncionando())
                {
                    (*iteLsl)->getPos().setEspaco((*itePosI));
                    (*iteLsl)->setFuncionando(true);
                    iSl++;
                    iteLsl++;
                    break;
                }
            }
            break;
        case ESQUELETO_2:
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
        case FOGO_FATUO_2:
            for (; iF < listFogoFatuo.tamanho(); iF++, iteFog++)
            {
                if(!(*iteFog)->getFuncionando())
                {
                    (*iteFog)->getPos().setEspaco((*itePosI));
                    (*iteFog)->setFuncionando(true);
                    iF++;
                    iteFog++;
                    break;
                }
            }
            break;
        case GOLEM_2:
            for (; iGl < listGolem.tamanho(); iGl++, iteGol++)
            {
                if(!(*iteGol)->getFuncionando())
                {
                    (*iteGol)->getPos().setEspaco((*itePosI));
                    (*iteGol)->setFuncionando(true);
                    iGl++;
                    iteGol++;
                    break;
                }
            }
            break;
        }
    }
}
void Fase_2::invocaInimigo(int t, Vetor_R2 p)
{
    Lista_Generica<Lava_Slime*>::Iterador iteSlv;
    iteSlv = listLavaSlime.inicio();
    Lista_Generica<Goblin*>::Iterador iteGob;
    iteGob = listGoblin.inicio();
    Lista_Generica<Morcego*>::Iterador iteMor;
    iteMor = listMorcego.inicio();
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    iteEsq = listEsqueleto.inicio();
    Lista_Generica<Fogo_Fatuo*>::Iterador iteFog;
    iteFog = listFogoFatuo.inicio();
    Lista_Generica<Golem*>::Iterador iteGol;
    iteGol = listGolem.inicio();

    switch (t)
    {
    case SLIME_LAVA_2:
        for (int i = 0 ; i < listLavaSlime.tamanho(); i++, iteSlv++)
        {
            if(!(*iteSlv)->getFuncionando())
            {
                (*iteSlv)->getPos().setEspaco(p);
                (*iteSlv)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteSlv));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteSlv)->getPos().getEspaco());
                        (*iteFum)->setAreaEfetiva((*iteSlv)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case GOBLIN_2:
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
    case MORCEGO_2:
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
    case ESQUELETO_2:
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
    case FOGO_FATUO_2:
        for (int i = 0; i < listFogoFatuo.tamanho(); i++, iteFog++)
        {
            if (!(*iteFog)->getFuncionando())
            {
                (*iteFog)->getPos().setEspaco(p);
                (*iteFog)->setFuncionando(true);
                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteFog));
                break;
            }
        }
        break;
    case GOLEM_2:
        for (int i = 0 ; i < listGolem.tamanho(); i++, iteGol++)
        {
            if(!(*iteGol)->getFuncionando())
            {
                (*iteGol)->getPos().setEspaco(p);
                (*iteGol)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteGol));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteGol)->getPos().getEspaco() + Vetor_R2::vetor(-210,-270));
                        (*iteFum)->setAreaEfetiva((*iteGol)->getBorda());
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
void Fase_2::pontoParada1()
{
    paraFase(8650 , 10700);
    if (podePassar)
    {
        podePassar = false;
        al_set_timer_count(timers[0], 0);
        al_start_timer(timers[0]);
        audioFase.fadeTo("F2", -0.01, 0.001);
        audioFase.tocaAudio("S1-F2", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("S1-F2", 0.0016, 1);
    }
    if (al_get_timer_count(timers[0]) == 10*60 && !invocou)
    {

        for (int i = 0; i < 15; i++, iteP1++)
        {
            if ((*iteP1) == SLIME_LAVA_2 || (*iteP1) == GOBLIN_2 || (*iteP1) == ESQUELETO_2 || (*iteP1) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + 1850*(rand()%2), 700));
                else
                    invocaInimigo((*iteP1), Vetor_R2::vetor(9660 , 444));
            }
            else if ((*iteP1) == MORCEGO_2 || (*iteP1)== FOGO_FATUO_2)
                invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + rand()%2000, 20));
        }
        invocou = true;
    }
    if (al_get_timer_count(timers[0]) > 10*60 && al_get_timer_count(timers[0]) <= 40*60 && (al_get_timer_count(timers[0])-10*60)%180 == 0)
    {
        if ((*iteP1) == SLIME_LAVA_2 || (*iteP1) == GOBLIN_2 || (*iteP1) == ESQUELETO_2 || (*iteP1) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + 1850*(rand()%2), 700));
                else
                    invocaInimigo((*iteP1), Vetor_R2::vetor(9660 , 444));
            }
            else if ((*iteP1) == MORCEGO_2 || (*iteP1)== FOGO_FATUO_2)
                invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + rand()%2000, 20));
        iteP1++;
    }
    else if (al_get_timer_count(timers[0]) > 40*60 && al_get_timer_count(timers[0]) <= 70*60 && (al_get_timer_count(timers[0])-40*60)%90 == 0)
    {
        if ((*iteP1) == SLIME_LAVA_2 || (*iteP1) == GOBLIN_2 || (*iteP1) == ESQUELETO_2 || (*iteP1) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + 1850*(rand()%2), 700));
                else
                    invocaInimigo((*iteP1), Vetor_R2::vetor(9660 , 444));
            }
            else if ((*iteP1) == MORCEGO_2 || (*iteP1)== FOGO_FATUO_2)
                invocaInimigo((*iteP1), Vetor_R2::vetor(8700 + rand()%2000, 20));
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
                (*iteI)->getPos().getEspaco().getX() < 10700 &&
                (*iteI)->getPos().getEspaco().getX() > 8650 &&
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
            audioFase.fadeTo("F2", 0.01, 1);
            audioFase.fadeTo("S1-F2", -0.01, 0);
            jogo->setCheckpoint(2);
            jogo->salvaPosicao(listE->getListGen());
            listE->salvaScore();
        }
    }
}
void Fase_2::pontoParada2()
{
    paraFase(17500 , 19460);
    if (podePassar)
    {
        podePassar = false;
        al_set_timer_count(timers[0], 0);
        al_start_timer(timers[0]);
        audioFase.fadeTo("F2", -0.01, 0.001);
        audioFase.tocaAudio("S2-F2", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("S2-F2", 0.0016, 1);
    }
    if (al_get_timer_count(timers[0]) == 10*60 && !invocou)
    {
        for (int i = 0; i < 15; i++, iteP2++)
        {
            if ((*iteP2) == SLIME_LAVA_2 || (*iteP2) == GOBLIN_2 || (*iteP2) == ESQUELETO_2 || (*iteP2) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17600 + 1760*(rand()%2), 540));
                else
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17880 + 1200*(rand()%2), 310));
            }
            else if ((*iteP2) == MORCEGO_2 || (*iteP2)== FOGO_FATUO_2)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17500 + rand()%1960, 20));
        }
        invocou = true;
    }
    if (al_get_timer_count(timers[0]) > 10*60 && al_get_timer_count(timers[0]) <= 40*60 && (al_get_timer_count(timers[0])-5*60)%180 == 0)
    {
        if ((*iteP2) == SLIME_LAVA_2 || (*iteP2) == GOBLIN_2 || (*iteP2) == ESQUELETO_2 || (*iteP2) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17600 + 1760*(rand()%2), 540));
                else
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17880 + 1200*(rand()%2), 310));
            }
            else if ((*iteP2) == MORCEGO_2 || (*iteP2)== FOGO_FATUO_2)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17500 + rand()%1960, 20));
        iteP2++;
    }
    else if (al_get_timer_count(timers[0]) > 40*60 && al_get_timer_count(timers[0]) <= 70*60 && (al_get_timer_count(timers[0])-40*60)%60 == 0)
    {
        if ((*iteP2) == SLIME_LAVA_2 || (*iteP2) == GOBLIN_2 || (*iteP2) == ESQUELETO_2 || (*iteP2) == GOLEM_2)
            {
                if (rand()%2)
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17600 + 1760*(rand()%2), 540));
                else
                    invocaInimigo((*iteP2), Vetor_R2::vetor(17880 + 1200*(rand()%2), 310));
            }
            else if ((*iteP2) == MORCEGO_2 || (*iteP2)== FOGO_FATUO_2)
                invocaInimigo((*iteP2), Vetor_R2::vetor(17500 + rand()%1960, 20));
        iteP2++;
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
                (*iteI)->getPos().getEspaco().getX() < 19460 &&
                (*iteI)->getPos().getEspaco().getX() > 17500  &&
                (*iteI)->getPos().getEspaco().getY() >= 0 &&
                (*iteI)->getPos().getEspaco().getY() <= 840)
                break;
        }
        if (i == listIniPonto.tamanho())
        {
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);
            pontoParada++;
            podePassar = true;
            invocou = false;
            terminouInvocar = false;
            audioFase.fadeTo("F2", 0.01, 0.5);
            audioFase.fadeTo("S2-F2", -0.01, 0);
        }
    }
}
void Fase_2::atualizaCheckpoint(int c)
{
    if (c == 0)
    {
        ptrJ[0]->getPos().getEspaco().setXY(100,700 - ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(150,700 - ptrJ[0]->getBorda().getY());
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
    else if (c == 1)
    {
        ptrJ[0]->getPos().getEspaco().setXY(3100, 368 - ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(3140, 368 - ptrJ[0]->getBorda().getY());
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }

        ptrM->getPos().getEspaco().setX(3100-600);
        progressao = 2;
        pontoParada = 0;
    }
    else if (c == 2)
    {
        ptrJ[0]->getPos().getEspaco().setXY(9600,350);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(9600,350);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }

        ptrM->getPos().getEspaco().setX(9600-600);
        progressao = 2;
        pontoParada = 1;
    }
    else if (c == 3)
    {
        ptrJ[0]->getPos().getEspaco().setXY(15690, 362 - ptrJ[0]->getBorda().getY());
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(15690, 362 - ptrJ[0]->getBorda().getY());
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }

        ptrM->getPos().getEspaco().setX(15690-600);
        progressao = 4;
        pontoParada = 1;
    }
    resetaObstaculos();
    comecouFase = false;
    resetaObstaculos();
}
