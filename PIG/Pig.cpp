//
// Created by samuel on 6/15/20.
//

#include "Pig.h"

#include <iostream>

PIG::PIG(const char *nome, const char *arquivoFonteArial, int cursor) : CJogo(nome, arquivoFonteArial, cursor) {


    CAssetLoader::iniciar();

    if (cursor != 0)
        CMouse::iniciar(this->renderer);

    this->gerenciadorTimers = new CGerenciadorTimers();

    this->gerenciadorObjetos = new CGerenciadorObjetos(this->renderer);

    this->gerenciadorParticulas = new CGerenciadorParticulas(this->renderer);

    this->gerenciadorAnimacoes = new CGerenciadorAnimacoes(this->renderer);


    srand(time(nullptr));

    this->setEstado(true);


    existenciaTimer = new std::vector<int>(MAX_TIMERS, 0);
    existenciaObjeto = new std::vector<int>(MAX_OBJETOS, 0);

    this->camera = new Camera;

    this->camera->posX = 0;
    this->camera->posY = 0;
    this->camera->zoom = 0;
    this->camera->rotationXY = 0;
    this->camera->cameraRotationZ = 0;

    std::cout << "PIG inicializado " << this->renderer << std::endl;


}

int PIG::pegarTecla() {
    int i;

    auto evento = pegarEvento();

    return evento.teclado.tecla;

}

int PIG::criarTimer() {
    int timer;


    timer = this->gerenciadorTimers->criaTimer();
    this->existenciaTimer->at(timer) = 1;
    return timer;
}

int PIG::criarSprite(const std::string &nomeArquivo, int retiraFundo) {

    int objeto = gerenciadorObjetos->criaObjeto(0, 0, (char *) nomeArquivo.c_str(), retiraFundo, 255);
    this->existenciaObjeto->at(objeto) = 1;

    return objeto;
}

void PIG::pegarDimensoes(int id_objeto, int *altura, int *largura) {
    if (this->existenciaObjeto->at(id_objeto)) {
        gerenciadorObjetos->pegarDimensoes(id_objeto, altura, largura);
    }
}

int PIG::pegarLargura(int id_objeto) {
    int a, l;

    pegarDimensoes(id_objeto, &a, &l);
    return l;

}

int PIG::pegarAltura(int id_objeto) {
    int a, l;

    pegarDimensoes(id_objeto, &a, &l);
    return a;


}

void PIG::definirColoracao(int id_objeto, PIG_Cor cor) {
    if (this->existenciaObjeto->at(id_objeto)) {
        gerenciadorObjetos->definirColoracao(id_objeto, cor);
    }
}


void PIG::definirOpacidade(int id_objeto, int valor) {
    if (this->existenciaObjeto->at(id_objeto)) {
        gerenciadorObjetos->definirOpacidade(id_objeto, valor);
    }
}

void PIG::desenharSprite(int id_sprite, int X, int Y, int Largura, int Altura, float Angulo, int referencialVirtual) {
    if (!this->existenciaObjeto->at(id_sprite)) {
        return;
    }

    if (referencialVirtual == 1) {
        gerenciadorObjetos->desenhaSprite(id_sprite, X, Y, Largura * (this->camera->zoom + 1),
                                          Altura * (this->camera->zoom + 1),
                                          Angulo);
    } else {
        gerenciadorObjetos->desenhaSprite(id_sprite, X, Y, Largura, Altura, Angulo);
    }
}

PIG_Cor PIG::getPixelColor(int objeto, int coluna, int linha) {

    int largura = pegarLargura(objeto);
    int altura = pegarAltura(objeto);


    PIG_Cor cor = gerenciadorObjetos->pegarObjeto(objeto)->pixels[altura - 1 - linha][coluna];

    cor.a = 255;


    return cor;
}


void PIG::setPixelColor(int objeto, int coluna, int linha, PIG_Cor cor) {

    int altura = pegarAltura(objeto);

    auto corAtual = this->gerenciadorObjetos->pegarObjeto(objeto)->pixels[altura - 1 - linha][coluna];

    corAtual.r = cor.r;
    corAtual.g = cor.g;
    corAtual.b = cor.b;
    corAtual.a = 255;


}

void PIG::desenharMouse() {
    if (cursorPadrao != 0)
        CMouse::desenha();
}

Camera *PIG::pegarCamera() {
    return this->camera;
}

float PIG::tempoDecorrido(int id_timer) {

    if (this->existenciaTimer->at(id_timer) != 1) {
        printf("Id timer invalido id_timer %d", id_timer);
        exit(2);
    }


    return this->gerenciadorTimers->pegarTempoDecorrido(id_timer);
}

/********************************
A função pausaTimer() é responsável por realizar uma pausa na contagem do tempo de um timer. Após a despausa,
o timer continuará a contar o tempo normalmente.
Parâmetros:
id_timer (entrada, passagem por valor): identificador do timer.
********************************/
void PIG::pausarTimer(int id_timer) {
    if (this->existenciaTimer->at(id_timer) == 1) {
        this->gerenciadorTimers->pausaTimer(id_timer);
    }
}

/********************************
A função PausaTudo() é responsável por realizar uma pausa na contagem do tempo de todos os timers
criados pela função criaTimer();
********************************/
void PIG::pausarTudo() {
    this->gerenciadorTimers->PausaTodos();
}

/********************************
A função DespausaTimer() é responsável por retomar a contagem do tempo de um timer após uma pausa.
Parâmetros:
id_timer (entrada, passagem por valor): identificador do timer.
********************************/
void PIG::despausarTimer(int id_timer) {
    if (this->existenciaTimer->at(id_timer) == 1) {
        this->gerenciadorTimers->despausarTimer(id_timer);
    }
}

/********************************
A função DespausaTudo() é responsável por retomar a contagem do tempo de todos os timers
criados pela função criaTimer();
********************************/
void PIG::despausarTudo() {
    this->gerenciadorTimers->DespausarTodos();
}

/********************************
A função reiniciaTimer() é responsável por zerar a contagem do tempo de um timer, idependente do tempo já decorrido.
Na prática, é equivalente a encerrer um temporizador e recriá-lo em seguida.
Parâmetros:
id_timer (entrada, passagem por valor): identificador do timer.
********************************/
void PIG::reiniciarTimer(int id_timer) {
    if (this->existenciaTimer->at(id_timer) == 1) {
        this->gerenciadorTimers->reiniciaTimer(id_timer);
    }
}

/********************************
A função destroirTimer() é responsável por encerrar a utilização de timer.
Seu identificador não poderá ser utilizado após a função ser executada.
Parâmetros:
id_timer (entrada, passagem por valor): identificador do timer.
********************************/
void PIG::destruirTimer(int id_timer) {
    if (this->existenciaTimer->at(id_timer) == 1) {
        this->existenciaTimer->at(id_timer) = 0;
        this->gerenciadorTimers->destroirTimer(id_timer);
    }
}


/********************************
A função FinalizaJogo() é responsável por encerrar com a PIG. Após tudo o jogo ser executado
a função deve ser chamada e ela irá realizar a liberação de memória dos elementos criados pela PIG.
********************************/
PIG::~PIG() {

    delete this->gerenciadorTimers;

    delete this->gerenciadorObjetos;

    delete this->gerenciadorParticulas;

    delete this->gerenciadorAnimacoes;


    if (cursorPadrao == 0)
        CMouse::encerra();
    CAssetLoader::encerra();

    delete this->existenciaTimer;
    delete this->existenciaObjeto;

    delete camera;

}
