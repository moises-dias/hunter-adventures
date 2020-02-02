#ifndef MENU_PAUSA_H
#define MENU_PAUSA_H
#include "Audio.h"
#include "Vetor_R2.h"
#include "Controle.h"


class Menu_Pausa
{
    public:
        Menu_Pausa();
        Menu_Pausa(Controle * pC);
        ~Menu_Pausa();

        void atualiza();
        void desenha();
        void resetaPausa();
        void inicializa();

        void setControle(Controle* pC);

        bool getMudarEstado();
        int getEstado();

    private:
        ALLEGRO_BITMAP* botao;
        ALLEGRO_BITMAP* glow;
        ALLEGRO_BITMAP* cursor;
        Vetor_R2 posBotao[4];
        Vetor_R2 tamanhoSB;
        Vetor_R2 tamanhoSC;
        Vetor_R2 posCursor;
        Controle* ptrControl;
        int opcao;
        int fade;
        int alpha;
        float escala;
        bool mudaEstado;
        int estado;
        Audio audioPausa;
};

#endif // MENU_PAUSA_H
