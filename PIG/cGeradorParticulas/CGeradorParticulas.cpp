//
// Created by samuel on 6/15/20.
//

#include "CGeradorParticulas.h"

CGeradorParticulas::CGeradorParticulas(int maximoParticulas, SDL_Renderer *renderizador, char *nomeArq,
                                       int usaGerenciadorTimer) {
    maxParticulas = maximoParticulas;
    strcpy(nome, nomeArq);
    usaGerTimer = usaGerenciadorTimer;

    for (int i = 0; i < maxParticulas; i++)
        parts[i] = NULL;

    renderer = renderizador;
    dy = 0;
    dx = 0;
    hp = 1;
    ativas = 0;
}

CGeradorParticulas::~CGeradorParticulas() {
    for (int i = 0; i < maxParticulas; i++)
        if (parts[i]) delete parts[i];
}

void CGeradorParticulas::MoveParticulas() {
    for (int i = 0; i < maxParticulas; i++)
        if (parts[i]) {
            parts[i]->Move();
            if (!parts[i]->viva) {
                delete parts[i];
                parts[i] = NULL;
                ativas--;
            }
        }
}

void CGeradorParticulas::Move(int nx, int ny) {
    px = nx;
    py = ny;
}

void CGeradorParticulas::MudaDirecao(int novaDx, int novaDy) {
    dx = novaDx;
    dy = novaDy;
}

void CGeradorParticulas::MudaHP(int novoValor) {
    hp = novoValor;
}

void CGeradorParticulas::Desenha() {
    for (int i = 0; i < maxParticulas; i++)
        if ((parts[i]) && (parts[i]->viva)) {
            parts[i]->Desenha(NULL);
        }
}

int CGeradorParticulas::CriaParticula(int fadingOut, int minX, int minY, int maxX, int maxY, float maxTempo,
                                      CGerenciadorTimers *gerenciadorTimer) {
    int i = 0;
    if (ativas >= maxParticulas) return -1;
    while (parts[i])
        i++;
    parts[i] = new CParticula(px, py, dx, dy, hp, fadingOut, renderer, nome, gerenciadorTimer);
    parts[i]->DefineLimites(minX, maxX, minY, maxY, maxTempo);
    ativas++;
    return i;
}

int CGeradorParticulas::Colisao(Objeto outro) {
    int resp = 0;
    int i = 0, cont = 0;
    while (!resp && i < maxParticulas && cont < ativas) {
        if (parts[i]) {
            cont++;
            resp = parts[i]->Colisao(outro);
        }
        i++;
    }
    return resp;
}

int CGeradorParticulas::GetQtdAtivas() {
    return ativas;
}
