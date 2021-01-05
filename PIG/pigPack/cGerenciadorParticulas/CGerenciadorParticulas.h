//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CGERENCIADORPARTICULAS_H
#define PIG_CGERENCIADORPARTICULAS_H

#include "../../cGeradorParticulas/CGeradorParticulas.h"

class CGerenciadorParticulas {

private:
     SDL_Renderer *render;
     int totalGeradores;
     PoolNumeros numGeradores;
     GeradorParticulas geradores[MAX_GERADORPARTICULAS];

public:

    CGerenciadorParticulas(SDL_Renderer *renderer);

    ~CGerenciadorParticulas();

    int CriarGeradorParticulas(int maxParticulas, char *nomeArquivoBMP, int usaGerenciadorTimer = 1);

    void DestroirGeradorParticulas(int id_gerador);


    void Move(int id_gerador, int px, int py);

    void MudaDirecaoParticulas(int id_gerador, int dx, int dy);


    int CriarParticula(int id_gerador, int fadingOut = 0, int minX = -50, int minY = -50, int maxX = LARG_TELA + 50,
                       int maxY = ALT_TELA + 50, float maxTempo = 9999999.9);


    void MoveParticulas(int id_gerador);


    int GetQtdAtivas(int id_gerador);

    void Desenha(int id_gerador);

    int Colisao(int id_gerador, Objeto obj);

};


#endif //PIG_CGERENCIADORPARTICULAS_H
