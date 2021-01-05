//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CPILHACOORDENADA_H
#define PIG_CPILHACOORDENADA_H

#include <cstdlib>

class CPilhaCoordenada {
private:

    typedef struct Ponto {
        int x, y;
        struct Ponto *prox;
    } Ponto;

    Ponto *ini;

public:

    CPilhaCoordenada();

    void Empilha(int x, int y);

    int Desempilha(int &x, int &y);

    ~CPilhaCoordenada();
};


#endif //PIG_CPILHACOORDENADA_H
