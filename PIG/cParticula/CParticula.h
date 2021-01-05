//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CPARTICULA_H
#define PIG_CPARTICULA_H

#include "../cObjeto/CObjeto.h"
#include "../cTimer/CTimer.h"
#include "../pigPack/cGerenciadorTimers/CGerenciadorTimers.h"


class CParticula : public CObjeto {

private:
    Timer timer;
    int idTimer;
    int xini, yini;
    int dx, dy;
    int menor_x, maior_x, menor_y, maior_y;
    int hp;
    int fading;
    float lim_tempo;
    CGerenciadorTimers* gerenciadorTimer;


public:
    int viva;

    CParticula(int x, int y, int desloc_x, int desloc_y, int vida, int fadeOut, SDL_Renderer *renderizador,
               char *nomeArq, CGerenciadorTimers* gerenciadorTimer);

    void DefineLimites(int xMenor, int xMaior, int yMenor, int yMaior, float tempoMax);

    ~CParticula();

    void Move();

    void MudaDirecao(int novaDx, int novaDy);

    int Colisao(Objeto outro);
};


typedef CParticula *Particula;

#endif //PIG_CPARTICULA_H
