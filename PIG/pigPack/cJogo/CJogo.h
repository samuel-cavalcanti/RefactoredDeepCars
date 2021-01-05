//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CJOGO_H
#define PIG_CJOGO_H

#include<SDL2/SDL_render.h>

#include <SDL2/SDL.h>
#include "../../cTimer/CTimer.h"
#include "../cMouse/CMouse.h"

#include "../cMapaCaracteres/CMapaCaracteres.h"

class CJogo {

private:
    SDL_Texture *text_fundo;
    PIG_Cor corFundo{};
    PIG_Evento ultimoEvento{};
    PIG_Teclado teclado;
    int contFPS;
    Timer timerFPS;
    int fontesUsadas;
    MapaCaracteres fontes[MAX_FONTES]{};
    SDL_Window *window;
    int estado;
    bool rodando;

public:
    SDL_Renderer *renderer;
    OffscreenRenderer offRenderer;
    int cursorPadrao;

    CJogo(const char *nome, const char *arquivoFonteArial, int cursor = 0);

    ~CJogo();

    void defineFundo(char *nomeArquivo);

    PIG_Evento pegarEvento();

    PIG_Teclado pegaTeclado();

    void iniciaInputTextoTela();

    void encerraInputTextoTela();

    void iniciaDesenho();

    void encerraDesenho();

    int getEstado();

    bool getRodando() const;

    void setEstado(int valor);

    void desenhaRetangulo(int x, int y, int altura, int largura, PIG_Cor cor);

    void desenhaRetanguloVazado(int x, int y, int altura, int largura, PIG_Cor cor);

    void desenhaLinhaSimples(int x1, int y1, int x2, int y2, PIG_Cor cor);

    void desenhaLinhas(SDL_Point *points, int n, PIG_Cor cor);

    void desenharPonto(int x, int y, PIG_Cor cor);

    void desenharPontos(SDL_Point *points, int count, SDL_Color cor) const;

    void desenhaLinhasDisjuntas(int *x, int *y, int qtd, PIG_Cor cor);

    void desenhaLinhasSequencia(int *x, int *y, int qtd, PIG_Cor cor);

    int criaFonteFundo(char *nome, int tamanho, int estilo, char *arquivoFundo, int contorno, PIG_Cor corContorno);

    int criaFonteNormal(char *nome, int tamanho, int estilo, PIG_Cor corLetra, int contorno, PIG_Cor corContorno);

    int getLarguraPixels(char *str, int numFonte = 0);

    void escreverCentralizada(char *str, int x, int y, int numFonte = 0);

    void escreverDireita(char *str, int x, int y, int numFonte = 0);

    void escreverEsquerda(char *str, int x, int y, int numFonte = 0);

    void getPixel(int x, int y, int *r, int *g, int *b);

    float getFPS();

    void preparaOffScreenRenderer(int altura, int largura);

    void salvaOffScreenBMP(char *nomeArquivo);

    void pintaAreaOffScreen(int px, int py, PIG_Cor cor);

    void pintaFundoOffScreen(PIG_Cor cor);

    void desenhaRetanguloOffScreen(int x1, int y1, int altura, int largura, PIG_Cor cor);

    void desenhaRetanguloVazadoOffScreen(int x1, int y1, int altura, int largura, PIG_Cor cor);

    void desenhaLinhaOffScreen(int x1, int y1, int x2, int y2, PIG_Cor cor);

    void saveScreenshotBMP(char *nomeArquivo);


};

typedef CJogo *Jogo;


#endif //PIG_CJOGO_H
