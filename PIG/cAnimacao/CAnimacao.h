//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CANIMACAO_H
#define PIG_CANIMACAO_H


#include <SDL2/SDL.h>
#include "../cObjeto/CObjeto.h"
#include "../cTimer/CTimer.h"
#include "../pigPack/cGerenciadorTimers/CGerenciadorTimers.h"

class CAnimacao : public CObjeto {

    typedef struct {
        int loop;
        int qtdFrames;
        int frames[MAX_FRAMES_MODO];
        float delays[MAX_FRAMES_MODO];
    } Modo;

private:

    int souCopia;
    int qtdTotalFrames;
    Modo *modos[MAX_MODOS];
    int frameDoModo;
    SDL_Rect *frames[MAX_FRAMES];
    Timer tempoFrame;
    int idTimer;
    int offx, offy;
    int modoAtual;
    int frameAtual;
    CGerenciadorTimers *gerenciadorTimer;

public:

    CAnimacao(int px, int py, SDL_Renderer *renderizador, char *nomeArq, CGerenciadorTimers *gerenciadorTimer,
              int retiraFundo = 1, int opacidade = 255);


    CAnimacao(int px, int py, CAnimacao *base, CGerenciadorTimers *gerenciadorTimer);

    ~CAnimacao();

//define o retangulo da imagem que corresponde ao frame
    void CriaFrame(int codFrame, int x, int y, int altura, int largura);

//cria um modo vazio, sem frames associados
    void CriaModo(int idModo, int loop);

//insere um frame em um dos modos
    void InsereFrame(int idModo, int idFrame, float delayFrame) {
        int aux = modos[idModo]->qtdFrames;
        modos[idModo]->frames[aux] = idFrame;
        modos[idModo]->delays[aux] = delayFrame;
        modos[idModo]->qtdFrames++;
    }

//muda o modo atual
    void MudaModo(int idModo, int inicia);

//desenha a animação
    int Desenha();

};


typedef CAnimacao *Animacao;


#endif //PIG_CANIMACAO_H
