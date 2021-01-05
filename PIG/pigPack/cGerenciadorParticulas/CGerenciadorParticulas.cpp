//
// Created by samuel on 6/15/20.
//

#include "CGerenciadorParticulas.h"


CGerenciadorParticulas::CGerenciadorParticulas(SDL_Renderer *renderer) {
    render = renderer;
    totalGeradores = 0;
    for (auto &geradore : geradores)
        geradore = nullptr;
    numGeradores = new CPoolNumeros(MAX_GERADORPARTICULAS);
}

CGerenciadorParticulas::~CGerenciadorParticulas() {
    for (int i = 0; i < MAX_GERADORPARTICULAS; i++) {
        if (geradores[i]) delete geradores[i];
    }
    delete numGeradores;
}


int CGerenciadorParticulas::CriarGeradorParticulas(int maxParticulas, char *nomeArquivoBMP,
                                                          int usaGerenciadorTimer) {
    int resp = numGeradores->RetiraLivre();
    geradores[resp] = new CGeradorParticulas(maxParticulas, render, nomeArquivoBMP, usaGerenciadorTimer);
    totalGeradores++;
    return resp;
}

void CGerenciadorParticulas::DestroirGeradorParticulas(int id_gerador) {
    numGeradores->DevolveUsado(id_gerador);
    delete geradores[id_gerador];
    totalGeradores--;
    geradores[id_gerador] = NULL;
}


void CGerenciadorParticulas::Move(int id_gerador, int px, int py) {
    return geradores[id_gerador]->Move(px, py);
}


void CGerenciadorParticulas::MudaDirecaoParticulas(int id_gerador, int dx, int dy) {
    geradores[id_gerador]->MudaDirecao(dx, dy);
}


int CGerenciadorParticulas::CriarParticula(int id_gerador, int fadingOut, int minX, int minY, int maxX, int maxY,
                                                  float maxTempo) {

    return geradores[id_gerador]->CriaParticula(fadingOut, minX, minY, maxX, maxY, maxTempo);
}


void CGerenciadorParticulas::MoveParticulas(int id_gerador) {
    return geradores[id_gerador]->MoveParticulas();
}


int CGerenciadorParticulas::GetQtdAtivas(int id_gerador) {
    return geradores[id_gerador]->GetQtdAtivas();
}


void CGerenciadorParticulas::Desenha(int id_gerador) {
    return geradores[id_gerador]->Desenha();
}


int CGerenciadorParticulas::Colisao(int id_gerador, Objeto obj) {
    return geradores[id_gerador]->Colisao(obj);
}



