//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CGERENCIADOROBJETOS_H
#define PIG_CGERENCIADOROBJETOS_H

#include "../../cObjeto/CObjeto.h"
#include "../cPoolNumeros/CPoolNumeros.h"
#include <SDL2/SDL_render.h>

class CGerenciadorObjetos {

private:
    SDL_Renderer *renderer;
    int totalObjetos;
    PoolNumeros numObjetos;
    Objeto objetos[MAX_OBJETOS];


public:

    CGerenciadorObjetos(SDL_Renderer *render);

    Objeto pegarObjeto(int id_objeto);

    void encerra();

    int criaObjeto(int posx, int posy, char *nomeArquivoBMP, int retiraFundo = 1, int opacidade = 255);

    void destruirObjeto(int idObjeto);

    void definirValorIntObjeto(int idObjeto, int indice, int valor);

    void definirValorFloatObjeto(int idObjeto, int indice, float valor);

    int pegarValorIntObjeto(int idObjeto, int indice);

    float pegarValorFloatObjeto(int idObjeto, int indice);

    void pegarPosicaoXY(int idObjeto, int *x, int *y);

    void mover(int idObjeto, int x, int y);

    void definirAngulo(int idObjeto, float angulo);

    float pegarAngulo(int idObjeto);

    void definirPivo(int idObjeto, int x, int y);

    void pegarPivo(int idObjeto, int *x, int *y);

    void definirFlip(int idObjeto, PIG_Flip valor);

    PIG_Flip pegarFlip(int idObjeto);

    void definirDimensoes(int idObjeto, int altura, int largura);

    void pegarDimensoes(int idObjeto, int *altura, int *largura);

    void pegarDimensoesOriginais(int idObjeto, int *altura, int *largura);

    void CriaFrame(int idObjeto, int xBitmap, int yBitmap, int altura, int largura);

    void definirColoracao(int idObjeto, PIG_Cor cor);

    void MudarFrameObj(int idObjeto, char *nomeArquivo);

    void definirOpacidade(int idObjeto, int valor);

    void desenha(int id_objeto, OffscreenRenderer offRender = NULL);

    void desenhaSprite(int id_objeto, int X, int Y, int Largura, int Altura, float Angulo);

    int TestaColisao(int id_objeto1, int id_objeto2);

    PIG_Cor **pegarPixels(int id_objeto);

    void AtualizaPixels(int id_objeto, int retiraFundo = 1, int opacidade = 255);


};


#endif //PIG_CGERENCIADOROBJETOS_H
