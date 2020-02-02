#ifndef EFEITO_H
#define EFEITO_H

#include "Jogador.h"


class Efeito : public Entidade
{
    public:
        Efeito();
        virtual ~Efeito();

        virtual void desenha()=0;
        virtual void atualiza()=0;

        void setPtrJ(Jogador* ptr);
        void setMapaAtual(Mapa * ptr);
    protected:
        Jogador*  ptrJ;
        Mapa* mapaAtual;
};

#endif // EFEITO_H
