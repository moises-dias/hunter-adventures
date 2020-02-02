#ifndef ATAQUE_CIMA_JOGADOR_H
#define ATAQUE_CIMA_JOGADOR_H
#include "Projetil.h"


class Ataque_Cima_Jogador : public Projetil
{
    public:
        Ataque_Cima_Jogador();
        Ataque_Cima_Jogador(Vetor_R2 posI);
        ~Ataque_Cima_Jogador();

        void desenha();
        void atualiza();
        void criaFrames();

        void setFuncionando(bool f);
        void setPos(Vetor_R2 p);

        void resetaEntidade();

        void paraTimer();

    private:
        float alpha;
};

#endif // ATAQUE_CIMA_JOGADOR_H
