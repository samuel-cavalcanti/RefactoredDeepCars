//
// created by samuel on 6/15/20.
//

#ifndef PIG_CGERENCIADORANIMACOES_H
#define PIG_CGERENCIADORANIMACOES_H

#include "../../cAnimacao/CAnimacao.h"

class CGerenciadorAnimacoes {


private:
    SDL_Renderer *render;
    int totalAnimacoes;
    PoolNumeros numAnimacoes;
    Animacao animacoes[MAX_ANIMACOES];

public:


    CGerenciadorAnimacoes(SDL_Renderer *renderer);


    Animacao pegarAnimacao(int id_animacao);

    void encerra();

    int criaAnimacao(char *nomeArquivoBMP, int retiraFundo = 1, int opacidade = 255);

    int criaAnimacao(int id_animacaoBase);

    void destroiAnimacao(int id_animacao);

    void mover(int id_animacao, int px, int py);

    void pegarPosicaoXY(int id_animacao, int *x, int *y);

    int desenha(int id_animacao);

    void criaFrame(int id_animacao, int codFrame, int x, int y, int altura, int largura);

    void criaModo(int id_animacao, int idModo, int loop);

    void InsereFrame(int id_animacao, int idModo, int idFrame, float delayFrame);

    void MudaModo(int id_animacao, int idModo, int inicia);

    void definirOpacidade(int id_animacao, int valor);

    void definirColoracao(int id_animacao, PIG_Cor cor);

    void definirPivo(int id_animacao, int x, int y);

    void pegarPivo(int id_animacao, int *x, int *y);

    void definirAngulo(int id_animacao, float valor);

    float pegarAngulo(int id_animacao);

    int colisaoAnimacoes(int id_animacao1, int id_animacao2);

    int colisaoObjeto(int id_animacao, Objeto obj);

    void definirDimensoes(int id_animacao, int altura, int largura);

    void pegarDimensoes(int id_animacao, int *altura, int *largura);

    void definirValorInt(int id_animacao, int indice, int valor);

    int pegarValorInt(int id_animacao, int indice);

    void definirValorFloat(int id_animacao, int indice, float valor);

    float pegarValorFloat(int id_animacao, int indice);

};


#endif //PIG_CGERENCIADORANIMACOES_H
