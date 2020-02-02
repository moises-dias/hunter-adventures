#include "Sprite.h"

int Sprite::contTempo = 0;

Sprite::Sprite()
{
    estadoAntigo = -1;
    estadoNovo = PARADO;
    frameAtual = 0;
    tempoAnima = 1;
    coef = 1;
    numEstados = 0;
    estFrame = NULL;
    tFrame = NULL;
    imagem = NULL;
    girarRot=0;
}
Sprite::Sprite(int sX, int sY, char* nomeImg, int nEstados)
{
    spriteX = sX;
    spriteY = sY;
    estFrame = NULL;
    tFrame = NULL;
    imagem = al_load_bitmap(nomeImg);
    numEstados = nEstados;
    estadoAntigo = -1;
    estadoNovo = PARADO;
    frameAtual = 0;
    tempoAnima = 1;
    setNumEstados(numEstados);
    girarRot=0;
    coef = 1;

}

Sprite::~Sprite()
{
    for (int i = 0; i < numEstados; i ++)
        if (estFrame[i] != NULL)
            delete [] estFrame[i];
    if (estFrame != NULL)
        delete [] estFrame;
    estFrame = NULL;
    if (tFrame != NULL)
        delete [] tFrame;
    tFrame = NULL;
    if (imagem != NULL)
        al_destroy_bitmap(imagem);
    imagem = NULL;
}
void Sprite::atualizaFrame()
{
    if (estadoNovo != estadoAntigo)
    {
        tempoInicial = contTempo;
        estadoAntigo = estadoNovo;
        frameAtual = estFrame[estadoNovo][0];
    }
    if (frameAtual < estFrame[estadoNovo][0] || frameAtual > estFrame[estadoNovo][1])
    {
        frameAtual = estFrame[estadoNovo][0];
    }
    if (estadoNovo == estadoAntigo)
    {
        if(contTempo - tempoInicial >= (int)(((tFrame[estadoNovo])[frameAtual - estFrame[estadoNovo][0]])*tempoAnima))
        {
            frameAtual++;
            tempoInicial = contTempo;

        }
        if (frameAtual > estFrame[estadoNovo][1] && estFrame[estadoNovo][2])
        {
            frameAtual = estFrame[estadoNovo][0];
            tempoInicial = contTempo;
        }
    }
}
void Sprite::desenha(Vetor_R2 pos, Vetor_R2 referencia, float d, bool rotaciona, ALLEGRO_COLOR cor)
{
    int fx = frameAtual%10;
    int fy = frameAtual/10;
    if (rotaciona)
    {
        al_draw_tinted_scaled_rotated_bitmap_region(imagem,fx*spriteX, fy*spriteY, spriteX, spriteY, cor, spriteX/2,
                                                    spriteY/2,pos.getX()- referencia.getX(), pos.getY()- referencia.getY(),
                                                    coef, coef, d,girarRot);
    }
    else
        al_draw_tinted_scaled_bitmap(imagem, cor, fx*spriteX, fy*spriteY, spriteX, spriteY,
                                     pos.getX() - referencia.getX(), pos.getY() - referencia.getY(),
                                     spriteX*coef, spriteY*coef,
                                     ((int)d)*ALLEGRO_FLIP_HORIZONTAL | girarRot*ALLEGRO_FLIP_VERTICAL);

}

void Sprite::setContTempo(int i)
{
    contTempo = i;
}
int Sprite::getContTempo()
{
    return contTempo;
}
int Sprite::getTempoEstado()
{
    return contTempo - tempoInicial;
}
void Sprite::setEstado(int est)
{
    estadoNovo = est;
}
int Sprite::getEstado()
{
    return estadoNovo;
}
void Sprite::setSpriteX(int s)
{
    spriteX = s;
}
int Sprite::getSpriteX()
{
    return spriteX;
}
void Sprite::setSpriteY(int s)
{
    spriteY = s;
}
int Sprite::getSpriteY()
{
    return spriteY;
}
int Sprite::getFrameAtual()
{
    return frameAtual;
}
void Sprite::setFrameAtual(int f)
{
    frameAtual = f;
    tempoInicial = contTempo;
    estadoAntigo = estadoNovo;
}
void Sprite::setTempoAnima(float t)
{
    tempoAnima = t;
}
int ** Sprite::getEstFrame()
{
    return estFrame;
}
Lista_Generica<int> * Sprite::getTFrame()
{
    return tFrame;
}
void Sprite::setNomeImg(char* n)
{
    imagem=al_load_bitmap(n);
}
void Sprite::setCoef(float c)
{
    coef = c;
}
void Sprite::setNumEstados(int n)
{
    numEstados = n;
    estFrame = new int*[numEstados];
    for (int i = 0; i < numEstados; i++)
        estFrame[i] = new int[3];
    tFrame = new Lista_Generica<int>[numEstados];
}
void Sprite::setImagem(ALLEGRO_BITMAP* img)
{
    imagem = img;
}
ALLEGRO_BITMAP* Sprite::getImagem()
{
    return imagem;
}
void Sprite::setGiraRot(bool r)
{
    girarRot=r;
}



