//
// Created by samuel on 6/15/20.
//


#include "CPoolNumeros.h"


CPoolNumeros::CPoolNumeros(int qtd) {
    qtdTotal = qtd;
    livres = NULL;
    Elem *aux;
    for (int i = 0; i < qtd; i++) {
        aux = (Elem *) malloc(sizeof(Elem));
        aux->valor = i;
        aux->prox = livres;
        livres = aux;
    }
    usados = (int *) calloc(sizeof(int), qtd);
}

CPoolNumeros::~CPoolNumeros() {
    Elem *aux, *ret;
    aux = livres;
    while (aux) {
        ret = aux->prox;
        free(aux);
        aux = ret;
    }
    free(usados);
}

int CPoolNumeros::RetiraLivre() {
    Elem *aux = livres;
    livres = livres->prox;
    usados[aux->valor] = 1;
    return aux->valor;
}

void CPoolNumeros::DevolveUsado(int valor) {
    if (usados[valor] == 0) {
        //printf("erro de devolucao %d\n",valor);
        return;
    }

    usados[valor] = 0;
    Elem *aux = (Elem *) malloc(sizeof(Elem));
    aux->valor = valor;
    aux->prox = livres;
    livres = aux;
}

void CPoolNumeros::ImprimeLivres() {
    for (int i = 0; i < qtdTotal; i++) {
        if (usados[i] == 0)
            printf("%d\n", i);
    }
}

void CPoolNumeros::ImprimeUsados() {
    for (int i = 0; i < qtdTotal; i++) {
        if (usados[i] == 1)
            printf("%d\n", i);
    }
}