//
// Created by samuel on 6/15/20.
//

#include "CAssetLoader.h"


namespace CAssetLoader {

    namespace {
        int BuscaNome(char *str) {
            for (int i = 0; i < MAX_SLOTS_AL; i++) {
                if (slots[i]) {
                    if (strcmp(slots[i]->str, str) == 0)
                        return i;
                }
            }
            return -1;
        }

    }

    void iniciar() {
        numBitmaps = new CPoolNumeros(MAX_SLOTS_AL);
        totalBitmaps = 0;
        for (int i = 0; i < MAX_SLOTS_AL; i++)
            slots[i] = NULL;
    }

    void encerra() {
        for (int i = 0; i < MAX_SLOTS_AL; i++) {
            if (slots[i]) {
                SDL_FreeSurface(slots[i]->bitmap);
                free(slots[i]);
            }
        }
        delete numBitmaps;
    }

    SDL_Surface *LoadImage(char *nomeArq) {
        int indice = BuscaNome(nomeArq);
        if (indice == MAX_SLOTS_AL) {
            printf("Asset loader sobrecarregado\n");
        } else if (indice == -1) {
            indice = numBitmaps->RetiraLivre();
            slots[indice] = (HashNode *) malloc(sizeof(HashNode));
            slots[indice]->bitmap = IMG_Load(nomeArq);
            slots[indice]->cont = 1;
            strcpy(slots[indice]->str, nomeArq);
            // printf("%s carregado no indice %d\n",slots[indice]->str,indice);
            totalBitmaps++;
        } else {
            slots[indice]->cont++;
        }
        return slots[indice]->bitmap;
    }

    void FreeImage(char *nomeArq) {
        int indice = BuscaNome(nomeArq);
        if (indice == -1) {
            printf("Nao existe asset carregado: %s\n", nomeArq);
        } else {
            slots[indice]->cont--;
            if (slots[indice]->cont == 0) {
                SDL_FreeSurface(slots[indice]->bitmap);
                free(slots[indice]);
                slots[indice] = NULL;
                numBitmaps->DevolveUsado(indice);
            }
        }
    }

}
