//
// Created by samuel on 6/15/20.
//

#include "CParticula.h"

CParticula::CParticula(int x, int y, int desloc_x, int desloc_y, int vida, int fadeOut, SDL_Renderer *renderizador,
                       char *nomeArq, CGerenciadorTimers *gerenciadorTimer) : CObjeto(x, y, renderizador, nomeArq) {
    xini = x;
    yini = y;
    dx = desloc_x;
    dy = desloc_y;
    hp = vida;
    fading = fadeOut;

    if (gerenciadorTimer) {
        timer = NULL;
        idTimer = gerenciadorTimer->criaTimer();
    } else {
        timer = new CTimer();
        idTimer = -1;
    }

    viva = 1;
}

void CParticula::DefineLimites(int xMenor, int xMaior, int yMenor, int yMaior, float tempoMax) {
    menor_x = xMenor;
    maior_x = xMaior;
    menor_y = yMenor;
    maior_y = yMaior;
    lim_tempo = tempoMax;
}

CParticula::~CParticula() {
    if (timer) {
        delete timer;
    } else {
        gerenciadorTimer->destroirTimer(idTimer);
    }

}

void CParticula::Move() {
    if (!viva) return;
    float decorrido;
    if (timer) {
        decorrido = timer->GetTempoDecorrido();
    } else {
        decorrido = gerenciadorTimer->pegarTempoDecorrido(idTimer);
    }
    if (fading) {
        CObjeto::definirOpacidade(255 * (1 - (decorrido / lim_tempo)));
    }

    x = xini + dx * decorrido;
    y = yini + dy * decorrido;
    CObjeto::Move(x, y);
    viva = (x > menor_x) && (x < maior_x) && (y > menor_y) && (y < maior_y) && (decorrido < lim_tempo);
}

void CParticula::MudaDirecao(int novaDx, int novaDy) {
    dx = novaDx;
    dy = novaDy;
}

int CParticula::Colisao(Objeto outro) {
    int resp = CObjeto::Colisao(outro);
    if (resp) {
        hp--;
        viva = hp > 0;
    }
    return resp;
}