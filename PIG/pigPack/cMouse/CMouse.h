//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CMOUSE_H
#define PIG_CMOUSE_H

#include <SDL2/SDL.h>
#include "../cAssetLoader/CAssetLoader.h"

typedef struct iC {
    SDL_Surface *bitmap;
    SDL_Texture *text;
    char nomeArquivo[60];
} InfoCursor;


namespace CMouse {

    namespace {
        int cursorAtual;
        static InfoCursor *cursores[MAX_CURSORES];
        static SDL_Rect rect;
        static SDL_Renderer *renderer;
    }

    void iniciar(SDL_Renderer *render);

    void encerra();

    void pegaXY(int &x, int &y);

    void mudaCursor(int indice);

    void desenha();

    void move(int x, int y);

    void carregaCursor(int indice, char *nomeArquivo);


}


#endif //PIG_CMOUSE_H
