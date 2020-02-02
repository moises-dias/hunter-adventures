#ifndef PROJETIL_H
#define PROJETIL_H
#include "Efeito.h"


class Projetil : public Entidade
{
    public:
        Projetil();
        Projetil(int sX, int sY, char* nomeImg, int numEstados);
        virtual ~Projetil();
        virtual void desenha()=0;
        virtual void atualiza()=0;



        virtual void resetaEntidade()=0;

        void setPtrJ(Jogador* ptr);
        Jogador* getPtrJ();
        void setMapaAtual(Mapa * ptr);

    protected:
        float ang;
        float velInicial;
        Jogador*  ptrJ;
        Mapa* mapaAtual;
};

#endif // PROJETIL_H
