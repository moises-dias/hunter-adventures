#ifndef RASTRO_H
#define RASTRO_H
#include "Efeito.h"


class Rastro: public Efeito
{
    public:
        Rastro();
        Rastro(Vetor_R2 p, ALLEGRO_BITMAP* img, int sX, int sY, float coef, float velDes,
               float alphaIni, float ang,float cR, float cG, float cB);
        ~Rastro();

        void desenha();
        void atualiza();

        void setPos(Vetor_R2 p);
        void setFrameAtual(int f);
        void setVelDes(float v);
        void setAng(int a);

        void resetaEntidade();

        void paraTimer();
    private:
        float cRed;
        float cGreen;
        float cBlue;
        float alpha;
        float vDes;
        float angulo;
        bool rot;



};

#endif // RASTRO_H
