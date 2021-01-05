//
// Created by samuel on 6/15/20.
//


#include "CGerenciadorObjetos.h"


CGerenciadorObjetos::CGerenciadorObjetos(SDL_Renderer *render) {
    renderer = render;
    totalObjetos = 0;
    for (auto &objeto : objetos)
        objeto = nullptr;

    numObjetos = new CPoolNumeros(MAX_OBJETOS);

}

Objeto CGerenciadorObjetos::pegarObjeto(int id_objeto) {
    return objetos[id_objeto];
}




void CGerenciadorObjetos::encerra() {
    for (auto &objeto : objetos) {
        delete objeto;
    }
    delete numObjetos;
}

int CGerenciadorObjetos::criaObjeto(int posx, int posy, char *nomeArquivoBMP, int retiraFundo, int opacidade) {
    int resp = numObjetos->RetiraLivre();
    objetos[resp] = new CObjeto(posx, posy, renderer, nomeArquivoBMP, retiraFundo, opacidade);
    totalObjetos++;
    return resp;
}

void CGerenciadorObjetos::destruirObjeto(int idObjeto) {
    numObjetos->DevolveUsado(idObjeto);
    delete objetos[idObjeto];
    totalObjetos--;
    objetos[idObjeto] = NULL;
}

void CGerenciadorObjetos::definirValorIntObjeto(int idObjeto, int indice, int valor) {
    objetos[idObjeto]->valoresInt[indice] = valor;
}

void CGerenciadorObjetos::definirValorFloatObjeto(int idObjeto, int indice, float valor) {
    objetos[idObjeto]->valoresFloat[indice] = valor;
}

int CGerenciadorObjetos::pegarValorIntObjeto(int idObjeto, int indice) {
    return objetos[idObjeto]->valoresInt[indice];
}

float CGerenciadorObjetos::pegarValorFloatObjeto(int idObjeto, int indice) {
    return objetos[idObjeto]->valoresFloat[indice];
}

void CGerenciadorObjetos::pegarPosicaoXY(int idObjeto, int *x, int *y) {
    objetos[idObjeto]->pegarXY(*x, *y);
}

void CGerenciadorObjetos::mover(int idObjeto, int x, int y) {
    objetos[idObjeto]->Move(x, y);
}

void CGerenciadorObjetos::definirAngulo(int idObjeto, float angulo) {
    objetos[idObjeto]->definirAngulo(angulo);
}

float CGerenciadorObjetos::pegarAngulo(int idObjeto) {
    return objetos[idObjeto]->pegarAngulo();
}

void CGerenciadorObjetos::definirPivo(int idObjeto, int x, int y) {
    objetos[idObjeto]->definirPivo(x, y);
}

void CGerenciadorObjetos::pegarPivo(int idObjeto, int *x, int *y) {
    objetos[idObjeto]->pegarPivo(*x, *y);
}

void CGerenciadorObjetos::definirFlip(int idObjeto, PIG_Flip valor) {
    objetos[idObjeto]->definirFlip(valor);
}

PIG_Flip CGerenciadorObjetos::pegarFlip(int idObjeto) {
    return objetos[idObjeto]->pegarFlip();
}

void CGerenciadorObjetos::definirDimensoes(int idObjeto, int altura, int largura) {
    objetos[idObjeto]->definirDimensoes(altura, largura);
}

void CGerenciadorObjetos::pegarDimensoes(int idObjeto, int *altura, int *largura) {
    objetos[idObjeto]->pegarDimensoes(*altura, *largura);
}

void CGerenciadorObjetos::pegarDimensoesOriginais(int idObjeto, int *altura, int *largura) {
    objetos[idObjeto]->pegarDimensoesOriginais(*altura, *largura);
}

void CGerenciadorObjetos::CriaFrame(int idObjeto, int xBitmap, int yBitmap, int altura, int largura) {
    SDL_Rect r;
    r.x = xBitmap;
    r.y = yBitmap;
    r.h = altura;
    r.w = largura;
    objetos[idObjeto]->DefineFrame(&r);
}

void CGerenciadorObjetos::definirColoracao(int idObjeto, PIG_Cor cor) {
    objetos[idObjeto]->definirColoracao(cor);
}

void CGerenciadorObjetos::MudarFrameObj(int idObjeto, char *nomeArquivo) {
    objetos[idObjeto]->MudarFrame(nomeArquivo);
}

void CGerenciadorObjetos::definirOpacidade(int idObjeto, int valor) {
    objetos[idObjeto]->definirOpacidade(valor);
}

void CGerenciadorObjetos::desenha(int id_objeto, OffscreenRenderer offRender) {
    objetos[id_objeto]->Desenha(offRender);
}

void CGerenciadorObjetos::desenhaSprite(int id_objeto, int X, int Y, int Largura, int Altura, float Angulo) {
    objetos[id_objeto]->DesenhaSprite(X, Y, Largura, Altura, Angulo);
}

int CGerenciadorObjetos::TestaColisao(int id_objeto1, int id_objeto2) {
    return objetos[id_objeto1]->Colisao(objetos[id_objeto2]);
}

PIG_Cor **CGerenciadorObjetos::pegarPixels(int id_objeto) {
    return objetos[id_objeto]->pegarPixels();
}

void CGerenciadorObjetos::AtualizaPixels(int id_objeto, int retiraFundo, int opacidade) {
    objetos[id_objeto]->AtualizaPixels(retiraFundo, opacidade);
}
