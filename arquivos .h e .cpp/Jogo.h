#include "Carregar_Salvar.h"
#include "Menu.h"
#include "Menu_Pausa.h"




class Jogo
{
public:
	Jogo();
	~Jogo();
	void mudaEstado(int &oldState, int newState);
	void principalLoopJogo();
	void atualiza();
	void desenhaJogo();
	void menu();
	void desenhaMenu();
	void menuPausa();
	void desenhaMenuPausa();
	void gameOver();
	void desenhaGameOver();

	void setFase(int f);
	int getFase();
	void setCheckpoint(int c);
	bool getDoisJog();
	int getCheckpoint();
	ALLEGRO_TIMER * getTimer();

	void salvaPosicao(Lista_Generica<Entidade*> * listE);
	void carregaPosicao(Lista_Generica<Entidade*> * listE);
	void resetaSave(int f, int c);


private:

	ALLEGRO_DISPLAY * display = NULL;
	ALLEGRO_DISPLAY_MODE dataDisp;
	ALLEGRO_EVENT_QUEUE * eventQ = NULL;
	ALLEGRO_EVENT ev;
	ALLEGRO_TIMER * timer = NULL;
	ALLEGRO_FONT * font26 = NULL;
	ALLEGRO_JOYSTICK * joyS = NULL;


	int state;
	bool done;
	bool draw ;
	bool doisJogadores;
	bool saveCarregado;
	Menu menuPrincipal;
	Controle control;
	Menu_Pausa pausa;
	Gerenciador_Fases gFase;
	Carregar_Salvar persistencia;
	int joyX, joyY;


	int checkpoint;
	int altura, largura;
	int tempoPausa;
	int fase;
	int faseSave;
	int checkpointSave;
	Lista_Entidades todasEnt[3];


};

