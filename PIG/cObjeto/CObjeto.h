//
// Created by samuel on 6/15/20.
//

#ifndef PIG_COBJETO_H
#define PIG_COBJETO_H


#include <SDL2/SDL_render.h>
#include "../pigPack/Tipos_PIG.h"
#include "../cOffscreenRenderer/COffscreenRenderer.h"


class CObjeto {

protected:
    int alt, larg, altOriginal, largOriginal;
    float angulo;
    SDL_Rect dest, frame;
    SDL_Texture *text;
    SDL_Point pivoRelativo;
    SDL_Renderer *renderer;
    SDL_RendererFlip flip;
    SDL_Surface *bitmap;
    SDL_Point bb[4];

    char nomeImagem[100];
    int estado;

    void DesenhaBB();

    void AtualizaBB();

    void ExtraiPixels();

    void CriaTextura(int retiraFundo, int opacidade);

public:

    PIG_Cor **pixels;

    int x, y;
    int valoresInt[MAX_ATRIBUTOS_OBJETO];
    float valoresFloat[MAX_ATRIBUTOS_OBJETO];

    CObjeto(int posX, int posY, SDL_Renderer *renderizador, char *nomeArquivo, int retiraFundo = 1,
            int opacidade = 255);

    ~CObjeto();

    void MudarFrame(char *nomeArquivo);

    void DefineFrame(SDL_Rect *r);

    void pegarXY(int &x, int &y);

    void definirColoracao(PIG_Cor cor);

    void Desenha(OffscreenRenderer offRender = NULL);

    void DesenhaSprite(int X, int Y, int Largura, int Altura, float Angulo);

    int Colisao(CObjeto *outro);

    void definirAngulo(float a);

    float pegarAngulo();

    void definirFlip(PIG_Flip valor);

    PIG_Flip pegarFlip();

    void definirPivo(int px, int py);

    void pegarPivo(int &px, int &py);

    void Move(int nx, int ny);

    void definirDimensoes(int altura, int largura);

    void pegarDimensoes(int &altura, int &largura);

    void pegarDimensoesOriginais(int &altura, int &largura);

    void definirOpacidade(int valor);

    PIG_Cor **pegarPixels();

    void AtualizaPixels(int retiraFundo = 1, int opacidade = 255);
};

typedef CObjeto *Objeto;

#endif //PIG_COBJETO_H
