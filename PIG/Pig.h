//
// Created by samuel on 6/15/20.
//

#ifndef PIG_PIG_H
#define PIG_PIG_H

#include <string>
#include "pigPack/PIG_Pack.h"
#include "AlgebraLinear/AlgebraLinear.h"
#include "vector"

typedef struct Camera {
    double posX;
    double posY;
    double zoom;
    double rotationXY;
    double cameraRotationZ;
} Camera;


class PIG : public CJogo {

private:
    std::vector<int> *existenciaObjeto;
    std::vector<int> *existenciaTimer;
    CGerenciadorTimers* gerenciadorTimers;
    CGerenciadorObjetos* gerenciadorObjetos;
    CGerenciadorParticulas* gerenciadorParticulas;
    CGerenciadorAnimacoes* gerenciadorAnimacoes;
    Camera* camera;

public:


    PIG(const char *nome, const char *arquivoFonteArial, int cursor);

    ~PIG();

    int pegarTecla();

    Camera* pegarCamera();

    int criarTimer();

    int criarSprite(const std::string &nomeArquivo, int retiraFundo);

    int pegarLargura(int id_objeto);

    void pegarDimensoes(int id_objeto, int *altura, int *largura);

    int pegarAltura(int id_objeto);

    void definirColoracao(int id_objeto, SDL_Color cor);

    void definirOpacidade(int id_objeto, int valor);

    void desenharSprite(int id_sprite, int X, int Y, int Largura, int Altura, float Angulo, int referencialVirtual = 1);

    SDL_Color getPixelColor(int objeto, int coluna, int linha);

    void setPixelColor(int objeto, int coluna, int linha, SDL_Color cor);

    void desenharMouse();

    float tempoDecorrido(int id_timer);

    void pausarTimer(int id_timer);

    void pausarTudo();

    void despausarTimer(int id_timer);

    void despausarTudo();

    void reiniciarTimer(int id_timer);

    void destruirTimer(int id_timer);

};


#endif //PIG_PIG_H
