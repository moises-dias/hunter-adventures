#include "Plataforma.h"
#include "Entidade.h"

#ifndef MAPA_H
#define MAPA_H

class Jogador;

class Mapa : public Entidade
{
private:
    Jogador* jogadores[2];
    float delayX, delayY;
    float xP;
    float yP;
    ALLEGRO_BITMAP* imagem;
    ALLEGRO_BITMAP** mapas;
    int spriteX;
    int spriteY;
    int posX;
    int posY;
    int faseAtual;

     int iMapTr;
     int iMapFr;

    int numPlataformas;
    Plataforma *plats;

    bool carregouImg;

    ALLEGRO_FONT * font;


public:
    Mapa();
    Mapa(int sX, int sY, char* nomeImg, int numPlat, int fase);
    ~Mapa();
    void desenha();
    void atualiza();


    void setJogador(Jogador*j, int i);
    int getFase();

    void setNumPlat(int n);
    int getNumPlat();
    Plataforma& getPlat(int i);


    static int mouseX;
    static int mouseY;


};

#endif // MAPA_H
