//
// Created by samuel on 6/15/20.
//

#include "CTimer.h"


CTimer::CTimer() {
    inicio = clock();
    pausado = false;
}

CTimer::~CTimer() {
}

float CTimer::GetTempoDecorrido() {
    if (pausado) return (pausa - inicio) / 1000.0;
    else return (clock() - inicio) / 1000.0;
}

void CTimer::Pausar() {
    if (!pausado) {
        pausado = true;
        pausa = clock();
    }
}

void CTimer::Despausar() {
    if (pausado) {
        long int decorrido = pausa - inicio;
        inicio = clock() - decorrido;
        pausado = false;
    }
}

void CTimer::Reinicia() {
    pausado = false;
    inicio = clock();
}

CTimer *CTimer::Copia() {
    CTimer *outro = new CTimer();
    outro->inicio = inicio;
    outro->pausa = pausa;
    outro->pausado = pausado;
    return outro;
}