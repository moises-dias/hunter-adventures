#ifndef ATAQUE_NORMAL_JOGADOR_H
#define ATAQUE_NORMAL_JOGADOR_H
#include "Projetil.h"

class Lista_Entidades;
class Rastro;

class Ataque_Normal_Jogador :public Projetil
{
    public:
        Ataque_Normal_Jogador();
        Ataque_Normal_Jogador(Vetor_R2 posI, int di, bool ve);
        ~Ataque_Normal_Jogador();

        void desenha();
        void atualiza();
        void criaFrames();

        void setFuncionando(bool f);
        void setPos(Vetor_R2 p);
        void setDi(int d);
        void setVer(bool v);

        void resetaEntidade();

        void paraTimer();

    private:
        float coefVel;
        bool ver;
        float alpha;


};

#endif // ATAQUE_NORMAL_JOGADOR_H
