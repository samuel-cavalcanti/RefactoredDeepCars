//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CPOOLNUMEROS_H
#define PIG_CPOOLNUMEROS_H

#include <clocale>
#include <cstdlib>
#include <cstdio>


class CPoolNumeros {

    typedef struct xxx { // medo
        int valor;
        struct xxx *prox;
    } Elem;

private:

    Elem *livres;
    int *usados{};

public:

    int qtdTotal;

    CPoolNumeros(int qtd);

    ~CPoolNumeros();

    int RetiraLivre();

    void DevolveUsado(int valor);

    void ImprimeLivres();

    void ImprimeUsados();
};

typedef CPoolNumeros *PoolNumeros;

#endif //PIG_CPOOLNUMEROS_H
