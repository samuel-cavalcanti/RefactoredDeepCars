//
// Created by samuel on 6/15/20.
//

#include "CJogo.h"


CJogo::CJogo(const char *nome, const char *arquivoFonteArial, int cursor) {




    rodando = true;
    teclado = SDL_GetKeyboardState(NULL);
    estado = 0;
    contFPS = 0;
    timerFPS = new CTimer();
    offRenderer = NULL;
    cursorPadrao = cursor;




    text_fundo = NULL;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(nome, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARG_TELA, ALT_TELA,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }
    }



    TTF_Init();

    //inicializa o vetor contendo as fontes a serem usadas no jogo
    for (auto & fonte : fontes)
        fonte = NULL;

    //criação da fonte padrão da PIG (fonte com identificador 0)
    fontes[0] = new CMapaCaracteres(arquivoFonteArial, renderer,
                                    36, ESTILO_NORMAL, BRANCO, 0, BRANCO);

    fontesUsadas = 1;

    corFundo = PRETO;


}

CJogo::~CJogo() {
    delete timerFPS;
    if (offRenderer)
        delete offRenderer;

    for (int i = 0; i < MAX_FONTES; i++)
        if (fontes[i])
            delete fontes[i];
    TTF_Quit();

    if (text_fundo)
        SDL_DestroyTexture(text_fundo);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

void CJogo::defineFundo(char *nomeArquivo) {
    SDL_Surface *bitmap = SDL_LoadBMP(nomeArquivo);
    if (text_fundo)
        SDL_DestroyTexture(text_fundo);
    text_fundo = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
}

PIG_Evento CJogo::pegarEvento() {
    SDL_Event event;
    if (SDL_PollEvent((&event))) {
        if (event.type == SDL_QUIT)
            rodando = false;

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                ultimoEvento.tipoEvento = EVENTO_MOUSE;
                ultimoEvento.mouse.acao = MOUSE_PRESSIONADO;
                ultimoEvento.mouse.botao = event.button.button;
                ultimoEvento.mouse.posX = event.button.x;
                ultimoEvento.mouse.posY = ALT_TELA - event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                ultimoEvento.tipoEvento = EVENTO_MOUSE;
                ultimoEvento.mouse.acao = MOUSE_LIBERADO;
                ultimoEvento.mouse.botao = event.button.button;
                ultimoEvento.mouse.posX = event.button.x;
                ultimoEvento.mouse.posY = ALT_TELA - event.button.y;
                break;
            case SDL_MOUSEMOTION:
                ultimoEvento.tipoEvento = EVENTO_MOUSE;
                ultimoEvento.mouse.acao = MOUSE_MOVIDO;
                ultimoEvento.mouse.posX = event.motion.x;
                ultimoEvento.mouse.posY = ALT_TELA - event.motion.y;
                ultimoEvento.mouse.relX = event.motion.xrel;
                ultimoEvento.mouse.relY = -event.motion.yrel;
                if (cursorPadrao == 0)
                    CMouse::move(ultimoEvento.mouse.posX, ALT_TELA - ultimoEvento.mouse.posY);
                break;
            case SDL_MOUSEWHEEL:
                ultimoEvento.tipoEvento = EVENTO_MOUSE;
                ultimoEvento.mouse.acao = MOUSE_RODINHA;
                ultimoEvento.mouse.relX = event.wheel.x;
                ultimoEvento.mouse.relY = event.wheel.y;
                break;
            case SDL_KEYDOWN:
                ultimoEvento.tipoEvento = EVENTO_TECLADO;
                ultimoEvento.teclado.acao = TECLA_PRESSIONADA;
                ultimoEvento.teclado.tecla = event.key.keysym.scancode;
                ultimoEvento.teclado.repeticao = event.key.repeat;
                break;
            case SDL_KEYUP:
                ultimoEvento.tipoEvento = EVENTO_TECLADO;
                ultimoEvento.teclado.acao = TECLA_LIBERADA;
                ultimoEvento.teclado.tecla = event.key.keysym.scancode;
                ultimoEvento.teclado.repeticao = event.key.repeat;
                break;
            case SDL_TEXTEDITING:
                ultimoEvento.tipoEvento = EVENTO_TECLADO;
                ultimoEvento.teclado.acao = TECLA_EDICAO;
                ultimoEvento.teclado.inicio = event.edit.start;
                ultimoEvento.teclado.tamanhoSelecao = event.edit.length;
                strcpy(ultimoEvento.teclado.texto, event.edit.text);
                break;
            case SDL_TEXTINPUT:
                ultimoEvento.tipoEvento = EVENTO_TECLADO;
                ultimoEvento.teclado.acao = TECLA_INPUT;
                strcpy(ultimoEvento.teclado.texto, event.edit.text);
                break;
            case SDL_WINDOWEVENT:
                ultimoEvento.tipoEvento = EVENTO_JANELA;
                ultimoEvento.janela.acao = event.window.event;
                ultimoEvento.janela.dado1 = event.window.data1;
                ultimoEvento.janela.dado2 = event.window.data2;
                break;
        }
    } else
        ultimoEvento.tipoEvento = EVENTO_NULO;
    return ultimoEvento;
}

const Uint8 *CJogo::pegaTeclado() {
    return teclado;
}

void CJogo::iniciaInputTextoTela() {
    SDL_StartTextInput();
}

void CJogo::encerraInputTextoTela() {
    SDL_StopTextInput();
}

void CJogo::iniciaDesenho() {
    if (text_fundo == NULL) {
        SDL_SetRenderDrawColor(renderer, corFundo.r, corFundo.g, corFundo.b, 255);
        SDL_RenderClear(renderer);
    } else
        SDL_RenderCopy(renderer, text_fundo, NULL, NULL);
}

void CJogo::encerraDesenho() {
    SDL_RenderPresent(renderer);

    contFPS++;

    if (timerFPS->GetTempoDecorrido() > 10.0) {
        contFPS = 0;
        timerFPS->Reinicia();
    }
}

int CJogo::getEstado() {
    return estado;
}

bool CJogo::getRodando() const {
    return rodando;
}

void CJogo::setEstado(int valor) {
    estado = valor;
}

void CJogo::desenhaRetangulo(int x, int y, int altura, int largura, SDL_Color cor) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = ALT_TELA - (y + altura);
    rect.h = altura;
    rect.w = largura;

    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
    SDL_RenderFillRect(renderer, &rect);
}

void CJogo::desenhaRetanguloVazado(int x, int y, int altura, int largura, SDL_Color cor) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = ALT_TELA - (y + altura);
    rect.h = altura;
    rect.w = largura;

    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
    SDL_RenderDrawRect(renderer, &rect);
}

void CJogo::desenhaLinhaSimples(int x1, int y1, int x2, int y2, SDL_Color cor) {
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    SDL_RenderDrawLine(renderer, x1, ALT_TELA - y1, x2, ALT_TELA - y2);
}

void CJogo::desenhaLinhas(SDL_Point *points, int n, SDL_Color cor) {
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    SDL_RenderDrawLines(renderer, points, n);
}

void CJogo::desenharPonto(int x, int y, SDL_Color cor) {
    //int i, j;

    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    SDL_RenderDrawPoint(renderer, x, ALT_TELA - y);
    //for(i=0;i<tam;i++)
    //for(j=0;j<tam;j++)
    //SDL_RenderDrawPoint(renderer,x+i,ALT_TELA-y+j);
}

void CJogo::desenharPontos(SDL_Point *points, int count, SDL_Color cor) const {
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    SDL_RenderDrawPoints(renderer, points, count);
}

void CJogo::desenhaLinhasDisjuntas(int *x, int *y, int qtd, SDL_Color cor) {
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    for (int k = 0; k < qtd * 2; k += 2) {
        SDL_RenderDrawLine(renderer, x[k], ALT_TELA - y[k], x[k + 1], ALT_TELA - y[k + 1]);
    }
}

void CJogo::desenhaLinhasSequencia(int *x, int *y, int qtd, SDL_Color cor) {
    SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, 255);
    for (int k = 0; k < qtd - 1; k++) {
        SDL_RenderDrawLine(renderer, x[k], ALT_TELA - y[k], x[k + 1], ALT_TELA - y[k + 1]);
    }
}

int
CJogo::criaFonteFundo(char *nome, int tamanho, int estilo, char *arquivoFundo, int contorno, SDL_Color corContorno) {
    int aux = fontesUsadas;
    fontes[fontesUsadas] = new CMapaCaracteres(nome, renderer, tamanho, estilo, arquivoFundo, contorno,
                                               corContorno);
    fontesUsadas++;
    return aux;
}

int
CJogo::criaFonteNormal(char *nome, int tamanho, int estilo, SDL_Color corLetra, int contorno, SDL_Color corContorno) {
    int aux = fontesUsadas;
    fontes[fontesUsadas] = new CMapaCaracteres(nome, renderer, tamanho, estilo, corLetra, contorno, corContorno);
    fontesUsadas++;
    return aux;
}

int CJogo::getLarguraPixels(char *str, int numFonte) {
    return fontes[numFonte]->GetLarguraPixelsString(str);
}

void CJogo::escreverCentralizada(char *str, int x, int y, int numFonte) {
    fontes[numFonte]->EscreveStringCentralizado(str, x, y);
}

void CJogo::escreverDireita(char *str, int x, int y, int numFonte) {
    fontes[numFonte]->EscreveStringDireita(str, x, y);
}

void CJogo::escreverEsquerda(char *str, int x, int y, int numFonte) {
    fontes[numFonte]->EscreveStringEsquerda(str, x, y);
}

void CJogo::getPixel(int x, int y, int *r, int *g, int *b) {
    if (x < 0 || x >= LARG_TELA || y < 0 || y >= ALT_TELA)
        return;

    SDL_Surface *saveSurface = NULL;
    SDL_Surface *infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(window);
    if (infoSurface != NULL) {
        unsigned char *pixels = new unsigned char[infoSurface->w * infoSurface->h *
                                                  infoSurface->format->BytesPerPixel];
        if (pixels != 0) {
            if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels,
                                     infoSurface->w * infoSurface->format->BytesPerPixel) == 0) {
                Uint8 mr, mg, mb;
                Uint32 *p = (Uint32 *) pixels;
                p += (x + (ALT_TELA - y) * LARG_TELA);
                SDL_GetRGB(*p, (const SDL_PixelFormat *) &infoSurface->format->format, &mr, &mg, &mb);
                *r = mr;
                *g = mg;
                *b = mb;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
    }
}

float CJogo::getFPS() {
    return contFPS / timerFPS->GetTempoDecorrido();
}

void CJogo::preparaOffScreenRenderer(int altura, int largura) {
    if (offRenderer)
        delete offRenderer;
    offRenderer = new COffscreenRenderer(altura, largura);
}

void CJogo::salvaOffScreenBMP(char *nomeArquivo) {
    if (offRenderer)
        offRenderer->SalvarImagem(nomeArquivo);
}

void CJogo::pintaAreaOffScreen(int px, int py, SDL_Color cor) {
    if (offRenderer)
        offRenderer->PintarArea(px, py, cor);
}

void CJogo::pintaFundoOffScreen(SDL_Color cor) {
    if (offRenderer)
        offRenderer->PintarFundo(cor);
}

void CJogo::desenhaRetanguloOffScreen(int x1, int y1, int altura, int largura, SDL_Color cor) {
    if (offRenderer)
        offRenderer->DesenharRetangulo(x1, y1, altura, largura, cor);
}

void CJogo::desenhaRetanguloVazadoOffScreen(int x1, int y1, int altura, int largura, SDL_Color cor) {
    if (offRenderer)
        offRenderer->DesenharRetanguloVazado(x1, y1, altura, largura, cor);
}

void CJogo::desenhaLinhaOffScreen(int x1, int y1, int x2, int y2, SDL_Color cor) {
    if (offRenderer)
        offRenderer->DesenharLinha(x1, y1, x2, y2, cor);
}

void CJogo::saveScreenshotBMP(char *nomeArquivo) {
    SDL_Surface *saveSurface = NULL;
    SDL_Surface *infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(window);
    if (infoSurface != NULL) {
        unsigned char *pixels = new unsigned char[infoSurface->w * infoSurface->h *
                                                  infoSurface->format->BytesPerPixel];
        if (pixels != 0) {
            if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels,
                                     infoSurface->w * infoSurface->format->BytesPerPixel) == 0) {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h,
                                                       infoSurface->format->BitsPerPixel,
                                                       infoSurface->w * infoSurface->format->BytesPerPixel,
                                                       infoSurface->format->Rmask, infoSurface->format->Gmask,
                                                       infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface != NULL) {
                    SDL_SaveBMP(saveSurface, nomeArquivo);
                    SDL_FreeSurface(saveSurface);
                }
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
    }
}