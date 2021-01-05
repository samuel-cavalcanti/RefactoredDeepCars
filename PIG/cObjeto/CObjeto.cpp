//
// Created by samuel on 6/15/20.
//

#include "CObjeto.h"
#include "../pigPack/cAssetLoader/CAssetLoader.h"


void CObjeto::DesenhaBB() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, bb[0].x, ALT_TELA - bb[0].y, bb[1].x, ALT_TELA - bb[1].y);
    SDL_RenderDrawLine(renderer, bb[1].x, ALT_TELA - bb[1].y, bb[2].x, ALT_TELA - bb[2].y);
    SDL_RenderDrawLine(renderer, bb[2].x, ALT_TELA - bb[2].y, bb[3].x, ALT_TELA - bb[3].y);
    SDL_RenderDrawLine(renderer, bb[3].x, ALT_TELA - bb[3].y, bb[0].x, ALT_TELA - bb[0].y);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &dest);
}

void CObjeto::AtualizaBB() {
    SDL_Point pivoAbs;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    pivoAbs.x = pivoRelativo.x + x;
    pivoAbs.y = -pivoRelativo.y + y +
                alt;    //inverte o eixo Y, pois o pivoRel considera o eixo Y aumentando para baixo
    //printf("PivoRel: %d %d\n",pivoRelativo.x,pivoRelativo.y);
    //printf("PivoRAbs: %d %d\n",pivoAbs.x,pivoAbs.y);
    float angRad = -angulo * M_PI / 180.0;
    float seno = sin(angRad);
    float cosseno = cos(angRad);
    //printf("Pivo %d %d  %f\n",pivoObj.x,pivoObj.y,angRad);

    //matriz de rotação
    // ( cos(ang) sin(ang))   (Vx)   (Rx)
    // (-sin(ang) cos(ang)) * (Vy) = (Ry)

    //vetor (Rx,Ry) é a resposta do vetor (Vx,Vy) rotacionado em ang

    bb[0].x = (x - pivoAbs.x) * cosseno + (y - pivoAbs.y) * seno + pivoAbs.x;
    bb[0].y = (y - pivoAbs.y) * cosseno - (x - pivoAbs.x) * seno + pivoAbs.y;
    //printf("bb0: %d %d\n",bb[0].x,bb[0].y);

    bb[1].x = (x + larg - pivoAbs.x) * cosseno + (y - pivoAbs.y) * seno + pivoAbs.x;
    bb[1].y = (y - pivoAbs.y) * cosseno - (x + larg - pivoAbs.x) * seno + pivoAbs.y;
    //printf("bb1: %d %d\n",bb[1].x,bb[1].y);

    bb[2].x = (x + larg - pivoAbs.x) * cosseno + (y + alt - pivoAbs.y) * seno + pivoAbs.x;
    bb[2].y = (y + alt - pivoAbs.y) * cosseno - (x + larg - pivoAbs.x) * seno + pivoAbs.y;
    //printf("bb2: %d %d\n",bb[2].x,bb[2].y);

    bb[3].x = (x - pivoAbs.x) * cosseno + (y + alt - pivoAbs.y) * seno + pivoAbs.x;
    bb[3].y = (y + alt - pivoAbs.y) * cosseno - (x - pivoAbs.x) * seno + pivoAbs.y;
    //printf("bb3: %d %d\n",bb[3].x,bb[3].y);

}

void CObjeto::ExtraiPixels() {
    Uint8 *pix8;
    Uint32 *pix32;

    pixels = (PIG_Cor **) malloc(sizeof(PIG_Cor *) * bitmap->h);
    for (int i = 0; i < bitmap->h; i++) {
        pixels[i] = (PIG_Cor *) calloc(sizeof(PIG_Cor), bitmap->w);
    }

    if (bitmap->format->BytesPerPixel == 3) {
        for (int h = 0; h < bitmap->h; h++) {
            pix8 = (Uint8 *) bitmap->pixels + (h * bitmap->pitch);
            for (int w = 0; w < bitmap->w; w++) {
                if (bitmap->format->format == SDL_PIXELFORMAT_RGB24) {
                    pixels[h][w].r = *pix8;
                    pix8++;
                    pixels[h][w].g = *pix8;
                    pix8++;
                    pixels[h][w].b = *pix8;
                    pix8++;
                } else {
                    pixels[h][w].b = *pix8;
                    pix8++;
                    pixels[h][w].g = *pix8;
                    pix8++;
                    pixels[h][w].r = *pix8;
                    pix8++;
                }
                pixels[h][w].a = 255;
            }
        }
    } else if (bitmap->format->BytesPerPixel == 4) {
        pix32 = (Uint32 *) bitmap->pixels;
        for (int h = 0; h < bitmap->h; h++) {
            for (int w = 0; w < bitmap->w; w++) {
                SDL_GetRGBA((Uint32) *pix32, bitmap->format, &(pixels[h][w].r), &(pixels[h][w].g),
                            &(pixels[h][w].b), &(pixels[h][w].a));
                pix32++;
            }
        }
    }
}

void CObjeto::CriaTextura(int retiraFundo, int opacidade) {
    if (retiraFundo) {
        Uint8 red, green, blue, alpha;
        Uint32 *pixel = (Uint32 *) bitmap->pixels;
        SDL_GetRGBA(*pixel, bitmap->format, &red, &green, &blue, &alpha);
        //printf("\ncolorkey: %d %d %d\n",red,green,blue);
        SDL_SetColorKey(bitmap, SDL_TRUE, SDL_MapRGBA(bitmap->format, red, green, blue, alpha));
    }
    if (text) SDL_DestroyTexture(text);
    text = SDL_CreateTextureFromSurface(renderer, bitmap);

    SDL_SetTextureAlphaMod(text, opacidade);
}

CObjeto::CObjeto(int posX, int posY, SDL_Renderer *renderizador, char *nomeArquivo, int retiraFundo, int opacidade) {
    strcpy(nomeImagem, nomeArquivo);
    renderer = renderizador;

    x = posX;
    y = posY;
    angulo = 0;

    estado = 0;
    memset(valoresInt, 0, sizeof(int) * MAX_ATRIBUTOS_OBJETO);
    memset(valoresFloat, 0, sizeof(float) * MAX_ATRIBUTOS_OBJETO);
    flip = SDL_FLIP_NONE;

#ifdef SHARE_BITMAP
    bitmap = CAssetLoader::LoadImage(nomeArquivo);
#else
    bitmap = IMG_Load(nomeArquivo);
#endif
    if (!bitmap)
        printf("erro ao ler arquivo: %s\n", nomeArquivo);
    altOriginal = alt = bitmap->h;
    largOriginal = larg = bitmap->w;

    pivoRelativo.x = 0;
    pivoRelativo.y = alt;
    AtualizaBB();

    dest.x = x;
    dest.y = ALT_TELA - y - alt;
    dest.h = alt;
    dest.w = larg;

    frame.x = frame.y = 0;
    frame.h = alt;
    frame.w = larg;

    text = NULL;
    ExtraiPixels();
    CriaTextura(retiraFundo, opacidade);
}

CObjeto::~CObjeto() {
    for (int i = 0; i < bitmap->h; i++)
        free(pixels[i]);
    free(pixels);
    if (text) SDL_DestroyTexture(text);
#ifdef SHARE_BITMAP
    CAssetLoader::FreeImage(nomeImagem);
#else
    SDL_FreeSurface(bitmap);
#endif
}

void CObjeto::MudarFrame(char *nomeArquivo) {
    strcpy(nomeImagem, nomeArquivo);
    bitmap = IMG_Load(nomeArquivo);
    if (!bitmap)
        printf("erro ao ler arquivo: %s\n", nomeArquivo);
    CriaTextura(1, 255);
}

void CObjeto::DefineFrame(SDL_Rect *r) {
    frame = *r;
}

void CObjeto::pegarXY(int &x, int &y) {
    x = this->x;
    y = this->y;
}

void CObjeto::Desenha(OffscreenRenderer offRender) {
    if (offRender == NULL) {
        SDL_RenderCopyEx(renderer, text, &frame, &dest, -angulo, &pivoRelativo, flip);
        //SDL_SetRenderDrawColor(renderer,255,0,0,255);
        //printf("px: %d py: %d\n",frame.w,frame.h);
        //system("pause");
        //SDL_RenderDrawPoint(renderer,pivoRelativo.x,ALT_TELA-pivoRelativo.y);
        //DesenhaBB();
    } else {
        SDL_Texture *textAux = SDL_CreateTextureFromSurface(offRender->GetRenderer(), bitmap);
        SDL_Rect rectAux = dest;
        rectAux.y = offRender->GetAltura() - alt - y;
        SDL_RenderCopyEx(offRender->GetRenderer(), textAux, &frame, &rectAux, -angulo, &pivoRelativo, flip);
        SDL_DestroyTexture(textAux);
    }
}

void CObjeto::DesenhaSprite(int X, int Y, int Largura, int Altura, float Angulo) {
    //SDL_RenderCopyEx(renderer, text, &frame,&dest,-angulo,&pivoRelativo,flip);
    X = X - Largura / 2;
    Y = Y - Altura / 2;

    dest.x = X;
    dest.y = (ALT_TELA - Altura - Y);
    dest.h = Altura;
    dest.w = Largura;
    SDL_RenderCopyEx(renderer, text, NULL, &dest, -Angulo, NULL, SDL_FLIP_NONE);
}

int CObjeto::Colisao(CObjeto *outro) {
    if (x + larg <= outro->dest.x) {
        return 0;
    }
    if (x >= outro->x + outro->larg) {
        return 0;
    }
    if (y + alt <= outro->y) {
        return 0;
    }
    if (y >= outro->y + outro->alt) {
        return 0;
    }
    return 1;
}

void CObjeto::definirAngulo(float a) {
    angulo = a;
    AtualizaBB();
}

float CObjeto::pegarAngulo() {
    return angulo;
}

void CObjeto::definirFlip(SDL_RendererFlip valor) {
    flip = valor;
}

PIG_Flip CObjeto::pegarFlip() {
    return flip;
}

void CObjeto::definirPivo(int px, int py) {
    pivoRelativo.x = px;
    pivoRelativo.y = py;
    //AtualizaBB();
}

void CObjeto::pegarPivo(int &px, int &py) {
    px = pivoRelativo.x;
    py = pivoRelativo.y;
}

void CObjeto::Move(int nx, int ny) {
    x = nx;
    y = ny;
    dest.x = x;
    dest.y = ALT_TELA - alt - y;
}

void CObjeto::definirDimensoes(int altura, int largura) {
    dest.h = alt = altura;
    dest.w = larg = largura;
    dest.x = x;
    dest.y = ALT_TELA - y - alt;
}

void CObjeto::pegarDimensoes(int &altura, int &largura) {
    altura = alt;
    largura = larg;
}

void CObjeto::pegarDimensoesOriginais(int &altura, int &largura) {
    altura = altOriginal;
    largura = largOriginal;
}

void CObjeto::definirOpacidade(int valor) {
    SDL_SetTextureAlphaMod(text, valor);
}

PIG_Cor **CObjeto::pegarPixels() {
    return pixels;
}

void CObjeto::definirColoracao(SDL_Color cor) {
    SDL_SetTextureColorMod(text, cor.r, cor.g, cor.b);
}

void CObjeto::AtualizaPixels(int retiraFundo, int opacidade) {
    Uint8 *pix8;
    Uint32 *pix32;
    if (bitmap->format->BytesPerPixel == 3) {
        for (int h = 0; h < bitmap->h; h++) {
            pix8 = (Uint8 *) bitmap->pixels + (h * bitmap->pitch);
            for (int w = 0; w < bitmap->w; w++) {
                if (bitmap->format->format == SDL_PIXELFORMAT_RGB24) {
                    *pix8 = pixels[h][w].r;
                    pix8++;
                    *pix8 = pixels[h][w].g;
                    pix8++;
                    *pix8 = pixels[h][w].b;
                    pix8++;
                } else {
                    *pix8 = pixels[h][w].b;
                    pix8++;
                    *pix8 = pixels[h][w].g;
                    pix8++;
                    *pix8 = pixels[h][w].r;
                    pix8++;
                }
            }
        }
    } else if (bitmap->format->BytesPerPixel == 4) {
        pix32 = (Uint32 *) bitmap->pixels;
        for (int h = 0; h < bitmap->h; h++) {
            for (int w = 0; w < bitmap->w; w++) {
                SDL_GetRGBA((Uint32) *pix32, bitmap->format, &(pixels[h][w].r), &(pixels[h][w].g),
                            &(pixels[h][w].b), &(pixels[h][w].a));
                pix32++;
            }
        }
    }

    CriaTextura(retiraFundo, opacidade);
}