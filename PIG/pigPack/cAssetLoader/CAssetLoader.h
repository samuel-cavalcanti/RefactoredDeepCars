//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CASSETLOADER_H
#define PIG_CASSETLOADER_H

#include "../cPoolNumeros/CPoolNumeros.h"
#include <SDL2/SDL_image.h>
#include "../Tipos_PIG.h"

typedef struct hnode {
    char str[100];
    int cont;
    SDL_Surface *bitmap;
} HashNode;


namespace CAssetLoader {
    namespace {
        static int totalBitmaps;
        static PoolNumeros numBitmaps;
        static HashNode *slots[MAX_SLOTS_AL];
    }


    void iniciar();

    void encerra();

    SDL_Surface *LoadImage(char *nomeArq);

    void FreeImage(char *nomeArq);


}


#endif //PIG_CASSETLOADER_H
