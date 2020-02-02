#ifndef POSICAO_H
#define POSICAO_H
#include "Vetor_R2.h"

class Posicao
{
    public:
        Posicao();
        ~Posicao();

        void mover();

        void setEspaco(Vetor_R2 esp);
        Vetor_R2& getEspaco();
        void setVelocidade(Vetor_R2 vel);
        Vetor_R2& getVelocidade();
        void setAceleracao(Vetor_R2 ace);
        Vetor_R2& getAceleracao();
        void setPlatAtual(int plat);
        int getPlatAtual();

    private:
        Vetor_R2 espaco;
        Vetor_R2 velocidade;
        Vetor_R2 aceleracao;
        int platAtual;

};

#endif // POSICAO_H
