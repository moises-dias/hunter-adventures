#ifndef MENU_H
#define MENU_H
#include "Controle.h"
#include "Botao.h"
#include "Audio.h"
#include "Carregar_Salvar.h"

const int NUM_MENU = 9;

class Menu
{
    public:
        Menu();
        ~Menu();
        void inicializa();
        bool definirJogo(Controle& control, int& faseInicio, bool& doisJogadores, bool& done);
        void desenhaMenu();

        void checarSaves();
        void checarFases(int s);
        void resetarMenu();
        void setTeclaPressionada(bool t);
        void setTecla(int t);

        bool funcaoTransicao(bool& done);

        void setPtrCarSav(Carregar_Salvar * pcs);

    private:
        ALLEGRO_BITMAP * telaMenu;
        ALLEGRO_BITMAP * cursor;


        Audio audioMenu;
        Carregar_Salvar * ptrCarSav;

        bool permiteOpcao[3];
        bool iniciando;
        bool finalizando;
        bool teclaPressionada;
        int tecla;
        string nome;

        ALLEGRO_FONT* fontMenu;
        ALLEGRO_FONT* fontRank;
        int menuAtual;
        int opcaoSelecionada;
        float cor;

        float escalaCursor;

        Lista_Generica<string>::Iterador iteMenu;
        Lista_Generica<string> opcoesMenu[NUM_MENU];
        int posXMenu, posYMenu;

        Lista_Generica<string> * nomeRanking;
        Lista_Generica<int> * scoreRanking;






};

#endif // MENU_H
