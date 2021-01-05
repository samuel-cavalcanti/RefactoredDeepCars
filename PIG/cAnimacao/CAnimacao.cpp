//
// Created by samuel on 6/15/20.
//

#include "CAnimacao.h"

CAnimacao::CAnimacao(int px, int py, SDL_Renderer *renderizador, char *nomeArq, CGerenciadorTimers *gerenciadorTimer,
                     int retiraFundo, int opacidade) : CObjeto(px, py, renderizador, nomeArq, retiraFundo,
                                                               opacidade) {
    souCopia = 0;
    offx = offy = 0;
    modoAtual = 0;
    frameDoModo = -1;

    for (auto &frame : frames)
        frame = (SDL_Rect *) malloc(sizeof(SDL_Rect));

    qtdTotalFrames = 0;

    for (auto &modo : modos) {
        modo = NULL;
    }

    if (gerenciadorTimer) {
        tempoFrame = NULL;
        idTimer = gerenciadorTimer->criaTimer();
        this->gerenciadorTimer = gerenciadorTimer;
    } else {
        tempoFrame = new CTimer();
        idTimer = -1;
    }
}

CAnimacao::CAnimacao(int px, int py, CAnimacao *base, CGerenciadorTimers *gerenciadorTimer) : CObjeto(px, py,
                                                                                                      base->renderer,
                                                                                                      base->nomeImagem) {
    souCopia = 1;
    frameDoModo = -1;

    qtdTotalFrames = base->qtdTotalFrames;
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i] = base->frames[i];
    }

    for (int i = 0; i < MAX_MODOS; i++) {
        modos[i] = base->modos[i];
    }
    definirDimensoes(base->alt, base->larg);

    offx = base->offx;
    offy = base->offy;
    modoAtual = base->modoAtual;

    if (gerenciadorTimer) {
        tempoFrame = NULL;
        idTimer = gerenciadorTimer->criaTimer();
        this->gerenciadorTimer = gerenciadorTimer;
    } else {
        tempoFrame = new CTimer();
        idTimer = -1;
    }
}

CAnimacao::~CAnimacao() {
    if (!souCopia) {
        for (auto &modo : modos) {
            if (modo)
                free(modo);
        }
        for (auto &frame : frames)
            free(frame);
    }

    if (!tempoFrame)
        gerenciadorTimer->destroirTimer(idTimer);

    delete tempoFrame;

//    if (tempoFrame) {
//        delete tempoFrame;
//    } else {
//        CGerenciadorTimers::destroirTimer(idTimer);
//    }
}

void CAnimacao::CriaFrame(int codFrame, int x, int y, int altura, int largura) {
    frames[codFrame]->x = x;
    frames[codFrame]->y = y;
    frames[codFrame]->h = altura;
    frames[codFrame]->w = largura;
}

void CAnimacao::CriaModo(int idModo, int loop) {
    if (this->modos[idModo])
        free(modos[idModo]);

    modos[idModo] = (Modo *) malloc(sizeof(Modo));
    Modo *atual = modos[idModo];
    atual->loop = loop;
    atual->qtdFrames = 0;
}

void CAnimacao::MudaModo(int idModo, int inicia) {
    modoAtual = idModo;

    if (inicia) frameDoModo = 0;
    else frameDoModo = modos[idModo]->qtdFrames - 1;
    frameAtual = modos[idModo]->frames[frameDoModo];

    if (tempoFrame)
        tempoFrame->Reinicia();
    else
        gerenciadorTimer->reiniciaTimer(idTimer);

    CObjeto::DefineFrame(frames[frameAtual]);
}

int CAnimacao::Desenha() {
    int resp = 0; //0 indica que que o modo de anima��o atual n�o terminou
    Modo *modo = modos[modoAtual];
    float tempo = modo->delays[frameDoModo];
    float tempoDecorrido;

    if (tempoFrame)
        tempoDecorrido = tempoFrame->GetTempoDecorrido();
    else
        tempoDecorrido = gerenciadorTimer->pegarTempoDecorrido(idTimer);

    if (tempoDecorrido > tempo) {
        frameDoModo++;
        if (frameDoModo >= modo->qtdFrames) {//testa se chegou ao ultimo frame do modo atual
            if (modo->loop) {
                frameDoModo = 0;
            } else {
                frameDoModo--; //permanece no ultimo frame e informa o encerramento do modo com resp=1
                resp = 1;
            }
        }
        if (tempoFrame)
            tempoFrame->Reinicia();
        else
            gerenciadorTimer->reiniciaTimer(idTimer);

        frameAtual = modo->frames[frameDoModo];
        CObjeto::DefineFrame(frames[frameAtual]);
    }

    int px = x + offx;
    int py = y + offy;

    CObjeto::Move(px, py);
    CObjeto::Desenha(NULL);

    px -= offx;
    py -= offy;

    CObjeto::Move(px, py);

    return resp;
}
