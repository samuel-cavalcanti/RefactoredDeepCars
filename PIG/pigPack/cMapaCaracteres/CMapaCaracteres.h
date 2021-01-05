//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CMAPACARACTERES_H
#define PIG_CMAPACARACTERES_H

#include "../../cOffscreenRenderer/COffscreenRenderer.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class CMapaCaracteres {
private:

    int *alturaLetra;
    int *larguraLetra;

    SDL_Renderer *render;
    SDL_Texture **glyphsT;

public:

    char nome[100];
    int tamFonte;

    CMapaCaracteres(const char *nomeFonte, SDL_Renderer *renderer, int tamanhoFonte, int estilo, PIG_Cor corFonte,
                    int outline, PIG_Cor corOutline);

    CMapaCaracteres(char *nomeFonte, SDL_Renderer *renderer, int tamanhoFonte, int estilo, char *fundoFonte,
                    int outline, PIG_Cor corOutline);

    ~CMapaCaracteres();

    int GetLarguraPixelsString(char *str);

    void EscreveStringCentralizado(char *str, int x, int y);

    void EscreveStringEsquerda(char *str, int x, int y);

    void EscreveStringDireita(char *str, int x, int y);
};

typedef CMapaCaracteres *MapaCaracteres;

#endif //PIG_CMAPACARACTERES_H
