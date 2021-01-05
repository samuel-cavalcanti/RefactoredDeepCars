//
// Created by samuel on 6/15/20.
//


#include "CPilhaCoordenada.h"


CPilhaCoordenada::CPilhaCoordenada() {
    ini = nullptr;
}

void CPilhaCoordenada::Empilha(int x, int y) {
    Ponto *novo = (Ponto *) malloc(sizeof(Ponto));
    novo->x = x;
    novo->y = y;
    novo->prox = ini;

    ini = novo;
}

int CPilhaCoordenada::Desempilha(int &x, int &y) {
    if (ini == NULL)
        return 0;
    Ponto *antigo = ini;
    ini = ini->prox;
    x = antigo->x;
    y = antigo->y;
    free(antigo);
    return 1;
}

CPilhaCoordenada::~CPilhaCoordenada() {
    Ponto *aux = ini;
    while (ini) {
        ini = ini->prox;
        free(aux);
    }
}