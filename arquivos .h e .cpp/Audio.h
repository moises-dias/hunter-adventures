#ifndef AUDIO_H
#define AUDIO_H
#include "Lista_Generica.h"

enum AUDIO_INFO { ALTURA, MODO, VEL_MUDANCA, ALTURA_F};
enum PLAYMODE{ONCE, LOOP};

class Audio
{
    public:
        Audio();
        ~Audio();

        void inicializaAudio(string * nomes, int n);
        void atualizaAudio();
        bool audioTocando(string nome);
        void tocaAudio(string nome, float altura, ALLEGRO_PLAYMODE modo, float velMudanca);
        void paraAudio(string nome);
        void fadeTo(string nome, float vFade, float alturaFinal);


    private:
        Lista_Generica<string> nomeAudio;
        Lista_Generica<string> nomeInst;
        Lista_Generica<ALLEGRO_SAMPLE*> audio;
        Lista_Generica<ALLEGRO_SAMPLE_INSTANCE*> audioInst;
        Lista_Generica<float> audioInfo[4];/**0 - AlturaAtual, 1 - Modo, 2 - velMudanca, 3 - AlturaFinal**/
};

#endif // AUDIO_H
