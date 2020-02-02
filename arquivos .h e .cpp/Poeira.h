#ifndef POEIRA_H
#define POEIRA_H
#include "Efeito.h"

class Poeira : public Efeito
{
    public:
        Poeira();
        Poeira(Vetor_R2 p, Vetor_R2 b, int t, bool rot, int d, bool desaparece, float vDes);
        ~Poeira();

        void desenha();
        void atualiza();

        void setFuncionando(bool f);
        void setDir(int d);
        void setPos(Vetor_R2 p);

        void resetaEntidade();
    private:

        bool rotaciona;
        bool des;
        float vD;
        float alpha;
        int tipo;
};

#endif // POEIRA_H
