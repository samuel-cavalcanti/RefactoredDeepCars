//
// Created by samuel on 6/15/20.
//

#include "CMouse.h"


namespace CMouse {



    void iniciar(SDL_Renderer *render) {
        renderer = render;
        rect.w = rect.h = 32;
        cursorAtual = 0;
        SDL_ShowCursor(0);
        //printf("Mouse iniciado\n");
        for (auto &cursore : cursores)
            cursore = NULL;
    }

    void encerra() {
        //printf("Encerrando mouse...");
        for (auto &cursore : cursores) {
            if (cursore) {
                CAssetLoader::FreeImage(cursore->nomeArquivo);
                SDL_DestroyTexture(cursore->text);
                free(cursore);
            }
        }
        //printf("encerrado\n");
    }

    void pegaXY(int &x, int &y) {
        x = rect.x;
        y = rect.y + 32;
    }

    void mudaCursor(int indice) {
        cursorAtual = indice;
    }

    void desenha() {
        if (cursores[cursorAtual])
            SDL_RenderCopy(renderer, cursores[cursorAtual]->text, NULL, &rect);
        else printf("Cursor atual nao definido...\n");
    }

    void move(int x, int y) {
        rect.x = x;
        rect.y = y;
    }

    void carregaCursor(int indice, char *nomeArquivo) {
        if (cursores[indice]) {
            CAssetLoader::FreeImage(cursores[indice]->nomeArquivo);
            SDL_DestroyTexture(cursores[indice]->text);
            free(cursores[indice]);
        }
        cursores[indice] = (InfoCursor *) malloc(sizeof(InfoCursor));
        strcpy(cursores[indice]->nomeArquivo, nomeArquivo);
        SDL_Surface *bitmap = CAssetLoader::LoadImage(nomeArquivo);
        cursores[indice]->bitmap = bitmap;
        if (bitmap == NULL) {
            printf("erro ao ler arquivo: %s\n", nomeArquivo);
            return;
        }

        Uint8 red, green, blue, alpha;
        Uint32 *pixel = (Uint32 *) bitmap->pixels;
        SDL_GetRGBA(*pixel, bitmap->format, &red, &green, &blue, &alpha);

        SDL_SetColorKey(bitmap, SDL_TRUE, SDL_MapRGBA(bitmap->format, red, green, blue, alpha));
        cursores[indice]->text = SDL_CreateTextureFromSurface(renderer, bitmap);
    }
}