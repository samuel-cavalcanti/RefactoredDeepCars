//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CGERADORPARTICULAS_H
#define PIG_CGERADORPARTICULAS_H


#include <SDL_render.h>
#include "../cParticula/CParticula.h"
#include "../pigPack/Tipos_PIG.h"
#include "../cObjeto/CObjeto.h"

class CGeradorParticulas {

private:
    Particula parts[MAX_PARTICULAS];
    int px, py, dx, dy;    //posicao2D e direcao atual das particulas
    SDL_Renderer *renderer;
    int hp;
    int maxParticulas;
    char nome[50];
    int ativas;
    int usaGerTimer;

public:

    CGeradorParticulas(int maximoParticulas, SDL_Renderer *renderizador, char *nomeArq, int usaGerenciadorTimer);

    ~CGeradorParticulas();

    void MoveParticulas();

    void Move(int nx, int ny);

    void MudaDirecao(int novaDx, int novaDy);

    void MudaHP(int novoValor);

    void Desenha();

    int CriaParticula(int fadingOut = 0, int minX = -50, int minY = -50, int maxX = LARG_TELA + 50,
                      int maxY = ALT_TELA + 50, float maxTempo = 9999999.9, CGerenciadorTimers *gerenciadorTimer = nullptr);

    int Colisao(Objeto outro);

    int GetQtdAtivas();
};

typedef CGeradorParticulas *GeradorParticulas;


#endif //PIG_CGERADORPARTICULAS_H
