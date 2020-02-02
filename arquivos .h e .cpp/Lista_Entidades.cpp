#include "Lista_Entidades.h"
#include "Fase.h"

Lista_Entidades::Lista_Entidades()
{
    ptrFase = NULL;
    ptrMapa = NULL;
    score = 0;
    scoreMudanca = 0;
    velScore = 0;


}
void Lista_Entidades::inicializaTimer()
{
    timerL = al_create_timer(1.0);
}

Lista_Entidades::~Lista_Entidades()
{
    al_destroy_timer(timerL);
    timerL = NULL;
}
void Lista_Entidades::colisoesChao()
{
    Entidade * ent;
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();

    int numPlataformas = ptrMapa->getNumPlat();
    float platX[numPlataformas][2];
    float platY[numPlataformas][2];
    int tipoPlat[numPlataformas];

    for(int i = 0; i < numPlataformas; i++)
    {
        platX[i][0] = ptrMapa->getPlat(i).getPos(0).getX();
        platX[i][1] = ptrMapa->getPlat(i).getPos(1).getX();
        platY[i][0] = ptrMapa->getPlat(i).getPos(0).getY();
        platY[i][1] = ptrMapa->getPlat(i).getPos(1).getY();
        tipoPlat[i] = ptrMapa->getPlat(i).getTipo();
    }

    int id;
    float playerX, playerBX, playerY, playerBY, velY;
    float medPlatX, medPlatY;
    float tgTheta, c;

    bool flagCol[] = {false, false, false};

    ite++;
    for(int i = 1; i < listaE.tamanho(); ite++, i++)
    {
        if ((*ite)!= NULL)
        {

            ent = (*ite);
            id = (ent)->getID();

            if (id == 32)
                continue;
            for(int i = 0; i < 3; i++)
                flagCol[i] = false;
            if(ent->getPermiteColisaoChao() && ent->getFuncionando() &&
                    ((ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) <= 1350 &&
                     (ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) >= -150))
            {
                for(int i = 0; i < numPlataformas; i++)
                {
                    playerX = ent->getPos().getEspaco().getX();
                    playerY = ent->getPos().getEspaco().getY();
                    velY = ent->getPos().getVelocidade().getY();
                    playerBX = ent->getBorda().getX();
                    playerBY = ent->getBorda().getY();
                    medPlatY = (platY[i][1] + platY[i][0]) / 2;
                    medPlatX = (platX[i][1] + platX[i][0]) / 2;


                    if (tipoPlat[i] == 0)
                    {
                        if (playerX <= (platX[i][0] + platX[i][1])/2)
                        {

                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][0]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][0]) / (medPlatX - platX[i][0]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX - playerBX <= medPlatX  &&
                                        playerX + playerBX >= platX[i][0] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    ent->getPos().getEspaco().setX(platX[i][0] - playerBX);
                                    (ent)->setColisao(true, COLIDE_XD);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XD] = true;
                                }

                            }
                            else if (playerY + playerBY - velY < tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                        else
                        {

                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][1]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][1]) / (medPlatX - platX[i][1]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX - playerBX) + c)
                            {
                                if (playerX - playerBX <= platX[i][1] &&
                                        playerX + playerBX >= medPlatX &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    (ent)->getPos().getEspaco().setX(platX[i][1] + playerBX);
                                    (ent)->setColisao(true, COLIDE_XE);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XE] = true;
                                }
                            }
                            else
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                    }
                    else if (tipoPlat[i] == 1)
                    {
                        if (playerY < platY[i][0])
                        {

                            if (
                                ent->getColidePlat() &&
                                playerX + playerBX >= platX[i][0] &&
                                playerX - playerBX <= platX[i][1] &&
                                playerY + playerBY >= platY[i][0] &&
                                playerY - playerBY <= platY[i][1] &&
                                velY >= 0 && (ent->getPos().getPlatAtual() != i || ent->getSprite().getEstado() != CAINDO))
                            {
                                (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                (ent)->setColisao(true, COLIDE_Y);
                                (ent)->getPos().setPlatAtual(i);
                                flagCol[COLIDE_Y] = true;

                            }
                        }
                    }
                    else if (tipoPlat[i] == 2 && id == 2)
                    {
                        if (playerX <= (platX[i][0] + platX[i][1])/2)
                        {
                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][0]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][0]) / (medPlatX - platX[i][0]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX - playerBX <= medPlatX  &&
                                        playerX + playerBX >= platX[i][0] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    (ent)->getPos().getEspaco().setX(platX[i][0] - playerBX);
                                    (ent)->setColisao(true, COLIDE_XD);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XD] = true;

                                }
                            }
                            else if (playerY + playerBY - velY < tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                        else
                        {

                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][1]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][1]) / (medPlatX - platX[i][1]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX - playerBX) + c)
                            {
                                if (playerX - playerBX <= platX[i][1] &&
                                        playerX + playerBX >= medPlatX &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    (ent)->getPos().getEspaco().setX(platX[i][1] + playerBX);
                                    (ent)->setColisao(true, COLIDE_XE);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XE] = true;
                                }
                            }
                            else
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                    }
                    else if (tipoPlat[i] == 3 && id == 4 && ent->getColidePlat())
                    {
                        if (playerX <= (platX[i][0] + platX[i][1])/2)
                        {
                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][0]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][0]) / (medPlatX - platX[i][0]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX - playerBX <= medPlatX  &&
                                        playerX + playerBX >= platX[i][0] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    (ent)->getPos().getEspaco().setX(platX[i][0] - playerBX);
                                    (ent)->setColisao(true, COLIDE_XD);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XD] = true;

                                }
                            }
                            else if (playerY + playerBY - velY < tgTheta*(playerX + playerBX) + c)
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                        else
                        {

                            tgTheta = (medPlatY - platY[i][0]) / (medPlatX - platX[i][1]);
                            c = (medPlatX*platY[i][0] - medPlatY*platX[i][1]) / (medPlatX - platX[i][1]);
                            if (playerY + playerBY - velY >= tgTheta*(playerX - playerBX) + c)
                            {
                                if (playerX - playerBX <= platX[i][1] &&
                                        playerX + playerBX >= medPlatX &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= platY[i][1])
                                {
                                    (ent)->getPos().getEspaco().setX(platX[i][1] + playerBX);
                                    (ent)->setColisao(true, COLIDE_XE);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_XE] = true;
                                }
                            }
                            else
                            {
                                if (playerX + playerBX >= platX[i][0] &&
                                        playerX - playerBX <= platX[i][1] &&
                                        playerY + playerBY >= platY[i][0] &&
                                        playerY - playerBY <= medPlatY)
                                {
                                    (ent)->getPos().getEspaco().setY(platY[i][0] - playerBY);
                                    (ent)->setColisao(true, COLIDE_Y);
                                    (ent)->getPos().setPlatAtual(i);
                                    flagCol[COLIDE_Y] = true;

                                }
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < NUM_COLISOES; i++)
            {
                if (!flagCol[i])
                    (ent)->setColisao(false, i);
            }
        }
    }



}
void Lista_Entidades::colisoesEnt()
{
    Entidade* aux1, *aux2;
    int i;
    Lista_Generica<Entidade*>::Iterador ite1, ite2;

    for (ite1 = listaE.retornaElemento(1); ite1 != listaE.fim(); ite1++)
        for (i = 2, ite2 = listaE.retornaElemento(2); i < listaE.tamanho(); ite2++, i++)
        {

            aux1 = (*ite1);
            aux2 = (*ite2);
            if ((aux1->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX() > -150 &&
                    aux1->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX() < 1350) &&
                    (aux2->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX() > -150 &&
                     aux2->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX() < 1350))
            {
                if((aux1)->getID() != (aux2)->getID() &&
                        ((aux1)->getID() | (aux2)->getID()) != 10 && ((aux1)->getID() | (aux2)->getID()) != 20 &&
                        (aux1->getFuncionando() && aux1->getPermiteColisaoEnt()) &&
                        (aux2->getFuncionando() && aux2->getPermiteColisaoEnt()) &&
                        verificaColisao(aux1, aux2))
                {
                    if ((aux1)->getID() == 2 && (aux2)->getDano() != 0 && !aux1->getTomandoDano())
                    {
                        ptrFase->criaDano((aux1)->getPos().getEspaco(), aux1->getBorda(), aux2->getDano());
                    }
                    else if ((aux2)->getID() == 2 && (aux1)->getDano() != 0 && !aux2->getTomandoDano())
                    {
                        ptrFase->criaDano(aux2->getPos().getEspaco(), aux2->getBorda(), aux1->getDano());
                    }
                    if ((aux1)->getID() == 4 && (aux1)->getID() != 64 && (aux2)->getID() != 64 && (aux2)->getDano() != 0 && !aux1->getTomandoDano())
                    {
                        ptrFase->criaDano(aux1->getPos().getEspaco(), aux1->getBorda(), aux2->getDano());
                    }
                    else if ((aux2)->getID() == 4 && (aux1)->getID() != 64 && (aux1)->getDano() != 0 && !aux2->getTomandoDano())
                    {
                        ptrFase->criaDano(aux2->getPos().getEspaco(), aux2->getBorda(), aux1->getDano());
                    }

                    switch ((aux1)->getID()|(aux2)->getID())
                    {
                    case (6):
                        if(aux1->getID() == 2)
                        {
                            (aux1)->diminuiVida((aux2)->getDano());
                            (aux1)->setTomandoDano(true);
                        }
                        else
                        {
                            (aux2)->setTomandoDano(true);
                            (aux2)->diminuiVida((aux1)->getDano());
                        }
                        aux1->setPosDano(aux2->getPos().getEspaco());
                        aux2->setPosDano(aux1->getPos().getEspaco());
                        break;
                    case (12):
                        (aux1)->diminuiVida((aux2)->getDano());
                        (aux1)->setTomandoDano(true);
                        (aux2)->diminuiVida((aux1)->getDano());
                        (aux2)->setTomandoDano(true);
                        aux1->setPosDano(aux2->getPos().getEspaco());
                        aux1->setPosVelo(aux2->getPos().getVelocidade());
                        aux2->setPosDano(aux1->getPos().getEspaco());
                        aux2->setPosVelo(aux1->getPos().getVelocidade());
                        break;
                    case(18):
                        (aux1)->diminuiVida((aux2)->getDano());
                        (aux1)->setTomandoDano(true);
                        (aux2)->diminuiVida((aux1)->getDano());
                        (aux2)->setTomandoDano(true);
                        aux1->setPosDano(aux2->getPos().getEspaco());
                        aux2->setPosDano(aux1->getPos().getEspaco());
                        break;
                    case(24):
                        (aux1)->diminuiVida((aux2)->getDano());
                        (aux1)->setTomandoDano(true);
                        (aux2)->diminuiVida((aux1)->getDano());
                        (aux2)->setTomandoDano(true);
                        aux1->setPosDano(aux2->getPos().getEspaco());
                        aux2->setPosDano(aux1->getPos().getEspaco());
                        break;
                    case(66):
                        if(aux1->getID()==2)
                        {
                            aux1->diminuiVida(aux2->getDano());
                            (aux1)->setTomandoDano(true);
                            aux1->setPosDano(aux2->getPos().getEspaco());
                        }

                        else if (aux2->getID()==2)
                        {
                            aux2->diminuiVida(aux1->getDano());
                            (aux2)->setTomandoDano(true);
                            aux2->setPosDano(aux1->getPos().getEspaco());
                        }
                        break;
                    }

                }
            }
        }
}
bool Lista_Entidades::verificaColisao(Entidade * ent1, Entidade * ent2)
{
    float eX1, eBX1, eX2, eBX2;
    float eY1, eBY1, eY2, eBY2;
    eX1 = ent1->getPos().getEspaco().getX();
    eBX1 = ent1->getBorda().getX();
    eX2 = ent2->getPos().getEspaco().getX();
    eBX2 = ent2->getBorda().getX();
    eY1 = ent1->getPos().getEspaco().getY();
    eBY1 = ent1->getBorda().getY();
    eY2 = ent2->getPos().getEspaco().getY();
    eBY2 = ent2->getBorda().getY();
    if (eX1 + eBX1 >= eX2 - eBX2 &&
            eX1 - eBX1 <= eX2 + eBX2 &&
            eY1 + eBY1 >= eY2 - eBY2 &&
            eY1 - eBY1 <= eY2 + eBY2)
        return true;
    return false;
}
void Lista_Entidades::incluiEntidade(Entidade* e)
{
    listaE.inclui_frente(e);
}
void Lista_Entidades::deletaListaEntidade()
{
    while(listaE.tamanho())
    {
        delete listaE.retira_frente();
    }

    ptrMapa = NULL;
    if (ptrFase != NULL)
        delete ptrFase;
    ptrFase = NULL;
}
void Lista_Entidades::atualiza()
{
    Entidade * ent = NULL;
    Inimigo * ptrI = NULL;
    Projetil * ptrP = NULL;
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();

    for(int i = 0 ; i < listaE.tamanho(); ite++, i++)
    {

        ent = (*ite);

        if (ent->getID() == 1 || ent->getID() == 8 || ent->getID() == 64 ||
                ((ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) <= 1350 &&
                 (ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) >= -150))
        {
            ent->atualiza();
        }
        else if ((ent->getID() == 16) &&
                 (((ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) > 1350 &&
                   (ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) < -150 ) ||
                  ent->getPos().getEspaco().getY() > 840))
        {
            ptrP = static_cast<Projetil*> (ent);
            ptrP->resetaEntidade();
            ptrP->setFuncionando(false);
        }
        else if (ent->getID() == 4 &&
                 (((ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) < -1000)||
                  ent->getPos().getEspaco().getY() > 840))
        {
            ptrI = static_cast<Inimigo*> (ent);
            ptrI->resetaEntidade();
            ptrI->setFuncionando(false);
        }
    }

    ptrFase->atualiza();

    if (al_get_timer_started(timerL) && velScore != 0)
    {
        velScore = 0;
    }
    else if (al_get_timer_started(timerL) && al_get_timer_count(timerL) >= 1)
    {
        al_set_timer_count(timerL, 0);
        al_stop_timer(timerL);

    }

    if (scoreMudanca > 0 && !al_get_timer_started(timerL))
    {
        if(scoreMudanca>1000)
            velScore = -10;
        else if(scoreMudanca>100 && scoreMudanca<1000)
            velScore = -4;
        else
            velScore = -1;
    }
    else if(scoreMudanca < 0  && !al_get_timer_started(timerL))
    {
        if(scoreMudanca<-1000)
            velScore = 10;
        else if(scoreMudanca<-100 && scoreMudanca>-1000)
            velScore = 4;
        else
            velScore = 1;
    }
    else if (scoreMudanca == 0)
    {
        velScore = 0;
    }
    scoreMudanca += velScore;
    score -= velScore;
}
void Lista_Entidades::desenha()
{
    Entidade * ent = NULL;
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();


    for(int i = 0; i < listaE.tamanho(); ite++, i++)
    {
        ent = (*ite);
        if (ent->getID() == 1 || ent->getID() == 64 ||
                ((ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) <= 1350 &&
                 (ent->getPos().getEspaco().getX() - ptrMapa->getPos().getEspaco().getX()) >= -150))
        {
            ent->desenha();
        }

    }
    ptrFase->desenha();
    string s1, s2;
    al_draw_textf(Entidade::font16, al_map_rgb(255,255,255), 1000, 10, 0, "Score:%i", score);
    for (int sc = score; sc != 0;)
    {
        sc /= 10;
        s2 += "0";
    }

    s1 = "Score:" + s2;
    if (scoreMudanca > 0)
    {
        al_draw_textf(Entidade::font16, al_map_rgb(255,255,255), 1000 + al_get_text_width(Entidade::font16, s1.c_str()),
                      10 + al_get_font_line_height(Entidade::font16), ALLEGRO_ALIGN_RIGHT, "+%i", scoreMudanca);
    }
    else if (scoreMudanca < 0)
    {
        al_draw_textf(Entidade::font16, al_map_rgb(255,255,255), 1000 + al_get_text_width(Entidade::font16, s1.c_str()),
                      10 + al_get_font_line_height(Entidade::font16), ALLEGRO_ALIGN_RIGHT, "%i", scoreMudanca);
    }


}
Lista_Generica<Entidade*>* Lista_Entidades::getListGen()
{
    return &listaE;
}
void Lista_Entidades::setFase(Fase* f)
{
    ptrFase = f;
}
void Lista_Entidades::setMapa(Mapa* pM)
{
    ptrMapa = pM;
}
void Lista_Entidades::mataJogador(int i)
{
    Inimigo * ptrI = NULL;
    Projetil * ptrP = NULL;
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();
    ite++;
    ptrMapa->setJogador(NULL, i);
    for (int j = 1; j < listaE.tamanho(); j++, ite++)
    {
        if ((*ite)->getID() == 4)
        {
            ptrI = static_cast<Inimigo*> (*ite);
            ptrI->setJogador(NULL, i);
        }
        else if ((*ite)->getID() == 16 || (*ite)->getID() == 8)
        {
            ptrP = static_cast<Projetil*> (*ite);
            if (ptrFase->getJog(i) == ptrP->getPtrJ())
                ptrP->setPtrJ(NULL);
        }
    }
}
void Lista_Entidades::resetaLista()
{
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();
    ite++;
    for (int i = 1; i < listaE.tamanho(); i++, ite++)
    {
        if ((*ite)->getFuncionando())
        {
            (*ite)->getPos().getEspaco().setXY(-200,-200);
            (*ite)->resetaEntidade();
            (*ite)->setFuncionando(false);
        }
    }
}
void Lista_Entidades::resetaJogador(Jogador* pJ, int i)
{
    Inimigo * ptrI = NULL;
    Projetil * ptrP = NULL;
    Lista_Generica<Entidade*>::Iterador ite;
    ite = listaE.inicio();
    ite++;
    ptrMapa->setJogador(pJ, i);
    pJ->resetaEntidade();
    for (int j = 1; j < listaE.tamanho(); j++, ite++)
    {
        if ((*ite)->getID() == 4)
        {
            ptrI = static_cast<Inimigo*> (*ite);
            ptrI->setJogador(pJ, i);
        }
        else if ((*ite)->getID() == 16 || (*ite)->getID() == 8)
        {
            ptrP = static_cast<Projetil*> (*ite);
            if (i == 0)
                ptrP->setPtrJ(pJ);
        }
    }
}
void Lista_Entidades::paraMusica()
{
    ptrFase->paraMusica();
}
void Lista_Entidades::mudaScore(int i)
{
    scoreMudanca += i;
    al_start_timer(timerL);
}
void Lista_Entidades::pausaEntidades()
{
    ptrFase->paraTimer();
    Lista_Generica<Entidade*>::Iterador iteE;
    iteE = listaE.inicio();
    for(int i = 0; i < listaE.tamanho(); i++, iteE++)
    {
        (*iteE)->paraTimer();
    }
}
void Lista_Entidades::despausaEntidades()
{
    Lista_Generica<Entidade*>::Iterador iteE;
    iteE = listaE.inicio();
    for(int i = 0; i < listaE.tamanho(); i++, iteE++)
    {
        (*iteE)->ativaTimer();
    }
    ptrFase->ativaTimer();
}
void Lista_Entidades::salvaScore()
{
    scoreSalvo = score;
}
void Lista_Entidades::resetaScore()
{
    score = scoreSalvo;
}
void Lista_Entidades::setScore(int s)
{
    scoreSalvo = score = s;
}
int Lista_Entidades::getScore()
{
    return score;
}
int Lista_Entidades::getScoreSave()
{
    return scoreSalvo;
}
