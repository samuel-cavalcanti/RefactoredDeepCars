//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CGERENCIADORTIMERS_H
#define PIG_CGERENCIADORTIMERS_H

#include "../../cTimer/CTimer.h"
#include "../cPoolNumeros/CPoolNumeros.h"
#include "../Tipos_PIG.h"



class CGerenciadorTimers {

private:
    int totalTimers;
    PoolNumeros numTimers;
    Timer timers[MAX_TIMERS];

public:

    CGerenciadorTimers();

    ~CGerenciadorTimers();

    int criaTimer();

    void destroirTimer(int id_timer);

    void PausaTodos();

    void DespausarTodos();

    float pegarTempoDecorrido(int idTimer);

    void reiniciaTimer(int idTimer);

    void pausaTimer(int idTimer);

    void despausarTimer(int idTimer);


};


#endif //PIG_CGERENCIADORTIMERS_H
