//
// Created by samuel on 6/15/20.
//

#include "CGerenciadorTimers.h"


CGerenciadorTimers::CGerenciadorTimers() {
    totalTimers = 0;

    for (auto &timer : timers)
        timer = nullptr;

    numTimers = new CPoolNumeros(MAX_TIMERS);
}

CGerenciadorTimers::~CGerenciadorTimers() {
    for (auto &timer : timers) {
        delete timer;
    }

    delete numTimers;
}

int CGerenciadorTimers::criaTimer() {
    int resp = numTimers->RetiraLivre();
    timers[resp] = new CTimer();
    totalTimers++;
    return resp;
}

void CGerenciadorTimers::destroirTimer(int id_timer) {
    numTimers->DevolveUsado(id_timer);
    totalTimers--;
    delete timers[id_timer];
    timers[id_timer] = NULL;
}

void CGerenciadorTimers::PausaTodos() {
    for (auto &timer : timers) {
        if (timer)
            timer->Pausar();
    }
}

void CGerenciadorTimers::DespausarTodos() {
    for (auto &timer : timers) {
        if (timer) {
            timer->Despausar();
        }
    }
}

float CGerenciadorTimers::pegarTempoDecorrido(int idTimer) {
    return timers[idTimer]->GetTempoDecorrido();
}

void CGerenciadorTimers::reiniciaTimer(int idTimer) {
    timers[idTimer]->Reinicia();
}

void CGerenciadorTimers::pausaTimer(int idTimer) {
    timers[idTimer]->Pausar();
}

void CGerenciadorTimers::despausarTimer(int idTimer) {
    timers[idTimer]->Despausar();
}


