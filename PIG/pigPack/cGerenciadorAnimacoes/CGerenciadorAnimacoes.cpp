//
// Created by samuel on 6/15/20.
//

#include "CGerenciadorAnimacoes.h"


CGerenciadorAnimacoes::CGerenciadorAnimacoes(SDL_Renderer *renderer) {
    render = renderer;
    totalAnimacoes = 0;
    for (auto &animacoe : animacoes)
        animacoe = NULL;
    numAnimacoes = new CPoolNumeros(MAX_ANIMACOES);
}

int CGerenciadorAnimacoes::criaAnimacao(char *nomeArquivoBMP, int retiraFundo, int opacidade) {
    int resp = numAnimacoes->RetiraLivre();
    animacoes[resp] = new CAnimacao(0, 0, render, nomeArquivoBMP, nullptr, opacidade, retiraFundo);
    totalAnimacoes++;
    return resp;
}

int CGerenciadorAnimacoes::criaAnimacao(int id_animacaoBase) {
    int resp = numAnimacoes->RetiraLivre();
    animacoes[resp] = new CAnimacao(0, 0, animacoes[id_animacaoBase], nullptr);
    totalAnimacoes++;
    return resp;
}

void CGerenciadorAnimacoes::encerra() {
    for (auto &animacao : animacoes) {
        if (!animacao)
            continue;
        delete animacao;
    }
    delete numAnimacoes;
}

void CGerenciadorAnimacoes::destroiAnimacao(int id_animacao) {
    numAnimacoes->DevolveUsado(id_animacao);
    delete animacoes[id_animacao];
    totalAnimacoes--;
    animacoes[id_animacao] = NULL;
}

void CGerenciadorAnimacoes::mover(int id_animacao, int px, int py) {
    animacoes[id_animacao]->Move(px, py);
}

void CGerenciadorAnimacoes::pegarPosicaoXY(int id_animacao, int *x, int *y) {
    animacoes[id_animacao]->pegarXY(*x, *y);
}

int CGerenciadorAnimacoes::desenha(int id_animacao) {
    return animacoes[id_animacao]->Desenha();
}

void CGerenciadorAnimacoes::criaFrame(int id_animacao, int codFrame, int x, int y, int altura, int largura) {
    animacoes[id_animacao]->CriaFrame(codFrame, x, y, altura, largura);
}

void CGerenciadorAnimacoes::criaModo(int id_animacao, int idModo, int loop) {
    animacoes[id_animacao]->CriaModo(idModo, loop);
}

void CGerenciadorAnimacoes::InsereFrame(int id_animacao, int idModo, int idFrame, float delayFrame) {
    animacoes[id_animacao]->InsereFrame(idModo, idFrame, delayFrame);
}

void CGerenciadorAnimacoes::MudaModo(int id_animacao, int idModo, int inicia) {
    animacoes[id_animacao]->MudaModo(idModo, inicia);
}

void CGerenciadorAnimacoes::definirOpacidade(int id_animacao, int valor) {
    animacoes[id_animacao]->definirOpacidade(valor);
}

void CGerenciadorAnimacoes::definirColoracao(int id_animacao, SDL_Color cor) {
    animacoes[id_animacao]->definirColoracao(cor);
}

void CGerenciadorAnimacoes::definirPivo(int id_animacao, int x, int y) {
    animacoes[id_animacao]->definirPivo(x, y);
}

void CGerenciadorAnimacoes::pegarPivo(int id_animacao, int *x, int *y) {
    animacoes[id_animacao]->pegarPivo(*x, *y);
}

void CGerenciadorAnimacoes::definirAngulo(int id_animacao, float valor) {
    animacoes[id_animacao]->definirAngulo(valor);
}

float CGerenciadorAnimacoes::pegarAngulo(int id_animacao) {
    return animacoes[id_animacao]->pegarAngulo();
}

int CGerenciadorAnimacoes::colisaoAnimacoes(int id_animacao1, int id_animacao2) {
    return animacoes[id_animacao1]->Colisao(animacoes[id_animacao2]);
}

int CGerenciadorAnimacoes::colisaoObjeto(int id_animacao, Objeto obj) {
    return animacoes[id_animacao]->Colisao(obj);
}

void CGerenciadorAnimacoes::definirDimensoes(int id_animacao, int altura, int largura) {
    animacoes[id_animacao]->definirDimensoes(altura, largura);
}

void CGerenciadorAnimacoes::pegarDimensoes(int id_animacao, int *altura, int *largura) {
    animacoes[id_animacao]->pegarDimensoes(*altura, *largura);
}

void CGerenciadorAnimacoes::definirValorInt(int id_animacao, int indice, int valor) {
    animacoes[id_animacao]->valoresInt[indice] = valor;
}

int CGerenciadorAnimacoes::pegarValorInt(int id_animacao, int indice) {
    return animacoes[id_animacao]->valoresInt[indice];
}

void CGerenciadorAnimacoes::definirValorFloat(int id_animacao, int indice, float valor) {
    animacoes[id_animacao]->valoresFloat[indice] = valor;
}

float CGerenciadorAnimacoes::pegarValorFloat(int id_animacao, int indice) {
    return animacoes[id_animacao]->valoresFloat[indice];
}

Animacao CGerenciadorAnimacoes::pegarAnimacao(int id_animacao) {
    return animacoes[id_animacao];
}
