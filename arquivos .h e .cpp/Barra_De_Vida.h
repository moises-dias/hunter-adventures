#ifndef BARRA_DE_VIDA_H
#define BARRA_DE_VIDA_H
#include "Efeito.h"


class Barra_De_Vida : public Efeito
{
    public:
        Barra_De_Vida();
        ~Barra_De_Vida();

        void desenha();
        void atualiza();
        void inicializa();
    protected:

    private:
        int nJogador;
        ALLEGRO_BITMAP * icon;
        int vida;
        float vidaI;
};

#endif // BARRA_DE_VIDA_H
