#include "Mapa.h"
#include "Jogador.h"

int Mapa::mouseX = 0;
int Mapa::mouseY = 0;

Mapa::Mapa()
{
    imagem = NULL;
}
Mapa::Mapa( int sX, int sY, char* nomeImg, int numPlat, int fase)
{
    jogadores[0] = NULL;
    jogadores[1] = NULL;
    xP = 0;
    yP = 0;

    plats = new Plataforma [numPlat];
    numPlataformas = numPlat;

    int numImgs = sX/1000;
    mapas = new ALLEGRO_BITMAP* [numImgs];

    string s1, s2 = "Dados/Fase/Fase", s3 = "_000", s4 = ".png";
    char *num = "0123456789";
    for (int i = 0; i < 10; i++)
    {
        s1 = s2 + num[fase] + s3 + num[i] + s4;
        mapas[i] = al_load_bitmap(s1.c_str());
    }
    faseAtual = fase;

    if (fase == 1)
        for (int i = 0; i < numImgs; i++)
            al_convert_mask_to_alpha(mapas[i], al_map_rgb(255,255,255));

    plats[numPlataformas-4].getPos(0).setXY(19896, -150);
    plats[numPlataformas-4].getPos(1).setXY(20000, 1000);
    plats[numPlataformas-4].setTipo(0);
    plats[numPlataformas-3].getPos(0).setXY(-100, -150);
    plats[numPlataformas-3].getPos(1).setXY(20, 1000);
    plats[numPlataformas-3].setTipo(0);
    plats[numPlataformas-2].getPos(0).setXY(-30, -1000);
    plats[numPlataformas-2].getPos(1).setXY(20, 2000);
    plats[numPlataformas-2].setTipo(2);
    plats[numPlataformas-1].getPos(0).setXY(3576, -1000);
    plats[numPlataformas-1].getPos(1).setXY(3700, 2000);
    plats[numPlataformas-1].setTipo(2);

    font = al_load_font("OpenSans.ttf", 16, 0);

    ID=1;
    permiteColisaoEnt=true;
    proj = 0;
    pos.getEspaco().setXY(0, 120);
    spriteX = sX;
    spriteY = sY;
    carregouImg = false;
    iMapTr = 0;
    imagem = NULL;
    if (nomeImg != NULL)
    {
        imagem = al_load_bitmap(nomeImg);
        al_convert_mask_to_alpha(imagem, al_map_rgb(255,255,255));
        carregouImg = true;
    }


}


Mapa::~Mapa()
{
    if (carregouImg && imagem != NULL)
        al_destroy_bitmap(imagem);
    imagem = NULL;
    if (font != NULL)
        al_destroy_font(font);
    if (plats != NULL)
        delete [] plats;
    for (int i = 0; i < 10; i++)
        if (mapas[i] != NULL)
            al_destroy_bitmap(mapas[i]);
    if (mapas != NULL)
        delete [] mapas;
}
void Mapa::desenha()
{

    posX = (int)pos.getEspaco().getX() - iMapTr*2*(spriteX/10);
    posY = (int)pos.getEspaco().getY();
    float escala = 2;

    posX /= escala;
    posY /= escala;

    if (carregouImg)
    {
        al_draw_scaled_bitmap(imagem, 500+ (pos.getEspaco().getX()*0.01)/2.0,720*1.5, 600, 360, 0,0,1200, 360*2, 0);
        al_draw_scaled_bitmap(imagem, (pos.getEspaco().getX()*0.28)/2.0,720, 600, 360, 0,0,1200, 360*2, 0);
        if (1800 - (pos.getEspaco().getX()*0.3)/2.0 <= 600)
            al_draw_scaled_bitmap(imagem, 0,720, 1800,360, (1800 - (pos.getEspaco().getX()*0.28)/2)*2, 0, 3200,720, 0);
        al_draw_scaled_bitmap(imagem, (pos.getEspaco().getX()*0.45)/2.0,0, 600, 360, 0,0,1200, 360*2, 0);
        if (1800 - (pos.getEspaco().getX()*0.45)/2.0 <= 600)
        {
            al_draw_scaled_bitmap(imagem, 0, 0, 1800, 360, (1800 - (pos.getEspaco().getX()*0.45)/2.0)*2, 0, 3200,720, 0);
            if (3200 + (1800 - (pos.getEspaco().getX()*0.45)/2.0)*2 <= 1200)
                al_draw_scaled_bitmap(imagem, 0, 0, 1800, 360, 3200 + (1800 - (pos.getEspaco().getX()*0.45)/2.0)*2, 0, 3200,720, 0);
        }
    }


    al_draw_scaled_bitmap(mapas[iMapTr], posX, posY, spriteX/10 - posX, spriteY,
                          0, 0,(spriteX/10 - posX)*escala, (spriteY)*escala, 0);
    if (iMapTr < 9)
        al_draw_scaled_bitmap(mapas[iMapTr + 1], 0, posY, (posX  + 600 - spriteX/10), spriteY,
                              (spriteX/10 - posX)*escala, 0,(posX + 600 - spriteX/10)*escala,(spriteY)*escala, 0);
}
void Mapa::atualiza()
{
    if (jogadores[0] == NULL && jogadores[1] == NULL)
    {
        xP = 600;
        yP = 520;
    }
    else if (jogadores[0] == NULL)
    {
        xP = jogadores[1]->getPos().getEspaco().getX();
        yP = jogadores[1]->getPos().getEspaco().getY();
    }
    else if(jogadores[1] == NULL)
    {
        xP = jogadores[0]->getPos().getEspaco().getX();
        yP = jogadores[0]->getPos().getEspaco().getY();
    }
    else
    {
        xP = (jogadores[0]->getPos().getEspaco().getX() + jogadores[1]->getPos().getEspaco().getX())/2;
        yP = (jogadores[0]->getPos().getEspaco().getY() + jogadores[1]->getPos().getEspaco().getY())/2;
    }
    if (xP < 600)
        xP = 600;
    if (xP > spriteX*2 - 600)
        xP = spriteX*2 - 600;
    if (yP < 360)
        yP = 360;
    if (yP > spriteY*2 - 360)
        yP = spriteY*2 - 360;

    if ((xP-600 - pos.getEspaco().getX() < -2) || (xP-600 - pos.getEspaco().getX() > 2))
        pos.getVelocidade().setX((xP-600 - pos.getEspaco().getX())*0.05);
    else
        pos.getEspaco().setX(xP - 600);
    if ((yP-360 - pos.getEspaco().getY() < -2) || (yP-360 - pos.getEspaco().getY() > 2))
        pos.getVelocidade().setY((yP-360 - pos.getEspaco().getY())*0.05);
    else
        pos.getEspaco().setY(yP-360);

    posX = pos.getEspaco().getX();

    iMapTr = ((posX+600)/2000) - (((posX+600)/1000)+1)%2;
    if (iMapTr < 0)
        iMapTr = 0;

    pos.mover();

    plats[numPlataformas - 2].getPos(0).setXY(pos.getEspaco().getX()-20, pos.getEspaco().getY()-100);
    plats[numPlataformas - 2].getPos(1).setXY(pos.getEspaco().getX()+30, pos.getEspaco().getY()+1000);
    plats[numPlataformas - 1].getPos(0).setXY(pos.getEspaco().getX()+1170, pos.getEspaco().getY()-100);
    plats[numPlataformas - 1].getPos(1).setXY(pos.getEspaco().getX()+1220, pos.getEspaco().getY()+1000);

}


void Mapa::setJogador(Jogador *j, int i)
{
    jogadores[i] = j;
}
int Mapa::getNumPlat()
{
    return numPlataformas;
}
Plataforma& Mapa::getPlat(int i)
{
    return plats[i];
}
int Mapa::getFase()
{
    return faseAtual;
}
