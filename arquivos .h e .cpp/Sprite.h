#ifndef SPRITE_H
#define SPRITE_H
#include "Lista_Generica.h"
#include "Vetor_R2.h"
#include "CEI.h"

class Sprite
{
    public:
        Sprite();
        Sprite(int sX, int sY, char* nomeImg, int nEstados);
        ~Sprite();
        void atualizaFrame();
        void desenha(Vetor_R2 pos , Vetor_R2 referencia, float d, bool rotaciona, ALLEGRO_COLOR cor);

        static void setContTempo(int i);
        static int getContTempo();
        void setEstado(int est);
        int getEstado();
        int getTempoEstado(); // retorna os ticks passados por um certo frame no estado que se está
        void setSpriteX(int s);
        int getSpriteX();
        void setSpriteY(int s);
        int getSpriteY();
        int getFrameAtual();
        void setFrameAtual(int f);
        void setTempoAnima(float t);
        int ** getEstFrame();
        Lista_Generica<int> * getTFrame();
        void setNomeImg(char* n);
        void setCoef(float c);
        void setNumEstados(int n);
        void setImagem(ALLEGRO_BITMAP* img);
        void setGiraRot(bool r);
        ALLEGRO_BITMAP* getImagem();
    private:
        static int contTempo;
        int frameAtual;
        int tempoInicial;
        int estadoAntigo;
        int estadoNovo;
        int spriteX, spriteY;
        ALLEGRO_BITMAP* imagem;
        int ** estFrame;
        Lista_Generica<int> *tFrame;
        float tempoAnima;
        float coef;
        int numEstados;
        bool girarRot;
};

#endif // SPRITE_H
