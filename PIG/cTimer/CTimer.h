//
// Created by samuel on 6/15/20.
//

#ifndef PIG_CTIMER_H
#define PIG_CTIMER_H

#include <ctime>

class CTimer {
private:
    long int inicio;
    long int pausa;
    bool pausado;
public:
    CTimer();

    ~CTimer();

    float GetTempoDecorrido();

    void Pausar();

    void Despausar();

    void Reinicia();

    CTimer *Copia();
};

typedef CTimer *Timer;

#endif //PIG_CTIMER_H
