#ifndef CARREGAR_SALVAR_H
#define CARREGAR_SALVAR_H
#include "Gerenciador_Fases.h"

class Jogo;

class Carregar_Salvar
{
    public:
        Carregar_Salvar();
        Carregar_Salvar(Jogo* pJo);
        ~Carregar_Salvar();

        void salvaLista(Lista_Generica<Entidade*> * listE, bool persistir);
        void carregaLista(Lista_Generica<Entidade*> * listE, bool salvo);

        bool saveCarregado(int s);
        void deletaSave(int s, bool reseta);
        void setSave(int s);
        int getSave();
        int getFaseAtual(int s);
        int getCheckpointAtual();
        bool getDoisJogadores();

        void getRanking();
        void salvaRanking();
        void carregaRanking();
        void setScoreSave(int s);
        int getScoreSave();

        Lista_Generica<string>* getNomeRanking();
        Lista_Generica<int>* getScoreRanking();

        void setNomeSave(string nome);


        void reseta();
        void inicializaSave();

        int charToIntRanged(char* c, int i, int f);
        int charToInt(char c);

        void convertToListEnt(Lista_Generica<Entidade*> * listE);
        void convertToListVec(Lista_Generica<Entidade*> * listE);
        void limpaLista();


    private:
        int saveAtual;
        int scoreSave;
        string nomeS;
        ifstream saveI[3];
        ofstream saveO;
        ifstream rankingI;
        ofstream rankingO;
        char * linha;
        Jogo * ptrJogo;
        Lista_Generica<Vetor_R2> posIni;
        Lista_Generica<int> indIni;

        Lista_Generica<int> score;
        Lista_Generica<string> nomeScore;




};

#endif // CARREGAR_SALVAR_H
