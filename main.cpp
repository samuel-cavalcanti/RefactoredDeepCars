#include "PIG/AlgebraLinear/AlgebraLinear.h"
#include "PIG/Pig.h"
#include "redeNeural.c"
#include <cstdlib>
#include <iostream>
#include <string>

#define DESLOCAMENTO_NEURONIOS 85

#define LARGURA_CENARIO 2000
#define ALTURA_CENARIO 2000

#define QTD_CARROS 1000
#define QTD_SPRITES_CARROS 7
#define QTD_OBSTACULO 23
#define QTD_ZONA 8

#define MAX_EXPLOSOES QTD_CARROS

#define CAR_BRAIN_QTD_LAYERS 1
#define CAR_BRAIN_QTD_INPUT 18
#define CAR_BRAIN_QTD_HIDE 6
#define CAR_BRAIN_QTD_OUTPUT 4

typedef struct carro {
  double X, Y;
  double VX, VY;
  double Angulo;
  double Velocidade;
  char Colidiu;
  char Queimado;

  int Sprite;
  double DistanciaSensores[CAR_BRAIN_QTD_INPUT - 1];

  int TamanhoDNA;
  double *DNA;
  double Fitness;

  RedeNeural *Cerebro;

} Carro;

typedef struct animation2 {
  int X;
  int Y;
  int Largura;
  int Altura;
  double Angulo;

  int Ativada;
  int FrameAtual;
  int QuantidadeFrames;

} Animation2;

typedef struct obstaculo {
  double X, Y;
  double VX, VY;
  double angulo;
  double velocidadeRotacao;
  double tamanhoX, tamanhoY;
  double rangeMaxMovimento;
  double rangeAtualMovimento;
  int emMovimento;

} Obstaculo;

typedef struct Zona {
  double valor;
  double angulo;
  double X, Y, larguraX, larguraY;

} Zona;

SDL_Point paredes[LARGURA_CENARIO * ALTURA_CENARIO];
SDL_Point paredesVirtual[LARGURA_CENARIO * ALTURA_CENARIO];
int quantidadeParede;

char cenario[LARGURA_CENARIO][ALTURA_CENARIO];
int distancias[LARGURA_CENARIO][ALTURA_CENARIO];
double matrizBoost[LARGURA_CENARIO][ALTURA_CENARIO];

int spritePista, spriteCarros[10], spriteCarroQueimado, SpritesExplosao[50];
Animation2 ListaExplosoes[MAX_EXPLOSOES];
int TimerGeral;
int carrosColididos;
Carro carros[QTD_CARROS];
int DistanciaRecorde, Geracao;
int valorDesfazerPista, maiorDistancia;
double velocidadeLaser;
int Fonte;
double Periodo = 0.01;
int spriteEstrelasLigada, spriteEstrelasDesligada, estrelasOpacidade = 255;
int spriteEspinho, spriteLama, spriteTurbo;

PIG_Cor cores[6] = {AMARELO, CIANO, VERMELHO, AZUL, VERDE, ROXO};

Obstaculo obstaculos[QTD_OBSTACULO];
Zona zonas[QTD_ZONA];

int SpriteNeuronDesativado, SpriteNeuronAtivado, SpriteLuzAmarelo,
    SpriteLuzAzul, SpriteSeta;
int spriteContornoNeuronio;

inline char *to_c_str(std::string string) { return (char *)string.c_str(); }

int buscarMelhorCarroVivo() {
  int menor = 999999;
  int indice = 0;
  for (int i = 0; i < QTD_CARROS; i++) {
    if (carros[i].Colidiu == 0) {
      int dist = distancias[(int)carros[i].X][(int)carros[i].Y];
      if (dist < menor) {
        menor = dist;
        indice = i;
      }
    }
  }
  return indice;
}

int dist_carro(Carro &c) {

  int pos_x = c.X;
  int pos_y = c.Y;
  int dist = distancias[pos_x][pos_y];
  return dist;
}

int buscarMelhorCarro() {
  // int menor = 999999;
  int indice = 0;
  int menor = dist_carro(carros[indice]);
  for (int i = 1; i < QTD_CARROS; i++) {
    int dist = dist_carro(carros[i]);
    if (dist < menor) {
      menor = dist;
      indice = i;
    }
  }
  return indice;
}

int buscarMelhorFitness() {
  int maior = carros[0].Fitness;
  int indice = 0;
  for (int i = 1; i < QTD_CARROS; i++) {
    double fit = carros[i].Fitness;
    if (fit > maior) {
      maior = fit;
      indice = i;
    }
  }
  return indice;
}

void aplicarSensores(Carro *carro, double *entrada) {
  int i;
  for (i = 0; i < CAR_BRAIN_QTD_INPUT - 1; i++) {
    double X1 = carro->X;
    double Y1 = carro->Y;
    double Angulo = carro->Angulo - 90.0 +
                    ((double)i) * 180.0 / ((double)(CAR_BRAIN_QTD_INPUT - 2));

    double Adjacente = 1 * (cos(Angulo * M_PI / 180.0));
    double Oposto = 1 * (sin(Angulo * M_PI / 180.0));

    while (1) {
      X1 += +Adjacente;
      Y1 += +Oposto;

      if (cenario[(int)X1][(int)Y1] == 0) {
        X1 = X1 - Adjacente;
        Y1 = Y1 - Oposto;

        double dist =
            AlgebraLinear::distanciaEntrePontos(carro->X, carro->Y, X1, Y1);
        if (entrada != NULL) {
          entrada[i] = dist;
        }
        carro->DistanciaSensores[i] = dist;
        break;
      }
    }
  }
  if (entrada != NULL) {
    entrada[i] = carro->Velocidade;
  }
}

PIG_Cor calcularCor(double Intensidade, PIG_Cor CorBase) {
  CorBase.r = CorBase.r * Intensidade;
  CorBase.g = CorBase.g * Intensidade;
  CorBase.b = CorBase.b * Intensidade;

  return CorBase;
}

void InicializarSpritesNeuronios(std::string &dir, PIG *jogo) {
  std::string png = ".png";

  std::string imagemNeuronio7 = dir + "neuronio7" + png;
  std::string imagemContornoNeuronio = dir + "branco" + png;
  std::string imagemLuzAmarelo = dir + "luz" + png;
  std::string imagemLuzAzul = dir + "luzAzul" + png;
  std::string imagemSeta = dir + "seta2" + png;

  SpriteNeuronDesativado = jogo->criarSprite(imagemNeuronio7, 0);
  jogo->definirColoracao(SpriteNeuronDesativado, PRETO);

  SpriteNeuronAtivado = jogo->criarSprite(imagemNeuronio7, 0);
  jogo->definirColoracao(SpriteNeuronAtivado, BRANCO);

  spriteContornoNeuronio = jogo->criarSprite(imagemContornoNeuronio, 0);

  SpriteLuzAmarelo = jogo->criarSprite(imagemLuzAmarelo, 0);

  SpriteLuzAzul = jogo->criarSprite(imagemLuzAzul, 0);

  SpriteSeta = jogo->criarSprite(imagemSeta, 0);
  jogo->definirColoracao(SpriteSeta, PRETO);
}

void DesenharRedeNeural(int X, int Y, int Largura, int Altura, PIG *jogo) {
  double NeuroEntradaX[CAR_BRAIN_QTD_INPUT];
  double NeuroEntradaY[CAR_BRAIN_QTD_INPUT];
  double NeuroEscondidoX[CAR_BRAIN_QTD_LAYERS][CAR_BRAIN_QTD_HIDE];
  double NeuroEscondidoY[CAR_BRAIN_QTD_LAYERS][CAR_BRAIN_QTD_HIDE];
  double NeuroSaidaX[CAR_BRAIN_QTD_OUTPUT];
  double NeuroSaidaY[CAR_BRAIN_QTD_OUTPUT];

  double Entrada[CAR_BRAIN_QTD_INPUT];
  double XOrigem = X + 325;
  double YOrigem = Y + Altura;
  double LarguraPintura = Largura;
  double AlturaPintura = Altura;
  double TamanhoNeuronio = 20;
  char String[1000];
  int Sprite;

  int indice = buscarMelhorCarroVivo();
  Carro *melhorCarro = &carros[indice];

  int qtdEscondidas = melhorCarro->Cerebro->QuantidadeEscondidas;
  int qtdNeuroEntrada = melhorCarro->Cerebro->CamadaEntrada.QuantidadeNeuronios;
  int qtdNeuroEscondidas =
      melhorCarro->Cerebro->CamadaEscondida[0].QuantidadeNeuronios;
  int qtdNeuroSaida = melhorCarro->Cerebro->CamadaSaida.QuantidadeNeuronios;

  for (int i = 0; i < CAR_BRAIN_QTD_INPUT; i++) {
    Entrada[i] = melhorCarro->Cerebro->CamadaEntrada.Neuronios[i].Saida;
  }

  double EscalaAltura =
      ((double)AlturaPintura) / (double)(qtdNeuroEscondidas - 1);
  double EscalaLargura =
      ((double)LarguraPintura - 475) / (double)(qtdEscondidas + 1);

  double temp = YOrigem - (EscalaAltura * (qtdNeuroEscondidas - 2)) / 2.0 +
                (EscalaAltura * (qtdNeuroSaida - 1)) / 2.0;

  sprintf(String, "Acelerar");
  jogo->escreverEsquerda(String, X + Largura - 130,
                         temp - 0 * EscalaAltura - 5 - DESLOCAMENTO_NEURONIOS,
                         Fonte);

  sprintf(String, "Re");
  jogo->escreverEsquerda(String, X + Largura - 130,
                         temp - 1 * EscalaAltura - 5 - DESLOCAMENTO_NEURONIOS,
                         Fonte);

  sprintf(String, "Virar Esquerda");
  jogo->escreverEsquerda(String, X + Largura - 130,
                         temp - 2 * EscalaAltura - 5 - DESLOCAMENTO_NEURONIOS,
                         Fonte);

  sprintf(String, "Virar Direita");
  jogo->escreverEsquerda(String, X + Largura - 130,
                         temp - 3 * EscalaAltura - 5 - DESLOCAMENTO_NEURONIOS,
                         Fonte);

  /// Desenhar Conexoes

  for (int i = 0; i < qtdNeuroEntrada - 1; i++) {
    NeuroEntradaX[i] = XOrigem;
    NeuroEntradaY[i] = YOrigem - i * EscalaAltura;
  }

  for (int i = 0; i < qtdEscondidas; i++) {
    int qtdCamadaAnterior;
    Camada *CamadaAnterior;
    double *XAnterior, *YAnterior;

    if (i == 0) {
      qtdCamadaAnterior = qtdNeuroEntrada;
      CamadaAnterior = &melhorCarro->Cerebro->CamadaEntrada;
      XAnterior = NeuroEntradaX;
      YAnterior = NeuroEntradaY;
    } else {
      qtdCamadaAnterior = qtdNeuroEscondidas;
      CamadaAnterior = &melhorCarro->Cerebro->CamadaEscondida[i - 1];
      XAnterior = NeuroEscondidoX[i - 1];
      YAnterior = NeuroEscondidoY[i - 1];
    }

    for (int j = 0; j < qtdNeuroEscondidas - 1; j++) {
      NeuroEscondidoX[i][j] = XOrigem + (i + 1) * EscalaLargura;
      NeuroEscondidoY[i][j] =
          YOrigem - j * EscalaAltura - DESLOCAMENTO_NEURONIOS;

      //            spriteID = SpriteNeuronDesativado;
      //            double SaidaNeuronio =
      //            MelhorDinossauro->cerebro->CamadaEscondida[i].neuronios[j].saida;
      //            if(SaidaNeuronio > 0)
      //            {
      //                spriteID = SpriteNeuronAtivado;
      //            }

      for (int k = 0; k < qtdCamadaAnterior - 1; k++) {
        double Peso =
            melhorCarro->Cerebro->CamadaEscondida[i].Neuronios[j].Peso[k];
        double Saida = CamadaAnterior->Neuronios[k].Saida;
        if (Peso * Saida > 0) {
          jogo->desenhaLinhaSimples(XAnterior[k], YAnterior[k],
                                    NeuroEscondidoX[i][j],
                                    NeuroEscondidoY[i][j], VERMELHO);
        } else {
          jogo->desenhaLinhaSimples(XAnterior[k], YAnterior[k],
                                    NeuroEscondidoX[i][j],
                                    NeuroEscondidoY[i][j], CINZA);
        }
      }

      //            desenharSprite(spriteID,
      //                           NeuroEscondidoX[i][j],
      //                           NeuroEscondidoY[i][j],
      //                           TamanhoNeuronio,
      //                           TamanhoNeuronio, 0, 0);
    }
  }

  for (int i = 0; i < qtdNeuroSaida; i++) {
    int UltimaCamada = melhorCarro->Cerebro->QuantidadeEscondidas - 1;
    double temp = YOrigem - (EscalaAltura * (qtdNeuroEscondidas - 2)) / 2.0 +
                  (EscalaAltura * (qtdNeuroSaida - 1)) / 2.0;

    NeuroSaidaX[i] = XOrigem + (qtdEscondidas + 1) * EscalaLargura;
    NeuroSaidaY[i] = temp - i * EscalaAltura - DESLOCAMENTO_NEURONIOS;

    //        spriteID = SpriteNeuronDesativado;
    //        double SaidaNeuronio =
    //        MelhorDinossauro->cerebro->CamadaSaida.neuronios[i].saida;
    //        if(SaidaNeuronio > 0.5)
    //        {
    //            spriteID = SpriteNeuronAtivado;
    //        }

    for (int k = 0; k < qtdNeuroEscondidas - 1; k++) {
      double Peso = melhorCarro->Cerebro->CamadaSaida.Neuronios[i].Peso[k];
      double Saida = melhorCarro->Cerebro->CamadaEscondida[UltimaCamada]
                         .Neuronios[k]
                         .Saida;

      if (Peso * Saida > 0) {
        jogo->desenhaLinhaSimples(NeuroEscondidoX[UltimaCamada][k],
                                  NeuroEscondidoY[UltimaCamada][k],
                                  NeuroSaidaX[i], NeuroSaidaY[i], VERMELHO);
      } else {
        jogo->desenhaLinhaSimples(NeuroEscondidoX[UltimaCamada][k],
                                  NeuroEscondidoY[UltimaCamada][k],
                                  NeuroSaidaX[i], NeuroSaidaY[i], CINZA);
      }
    }
    //        desenharSprite(spriteID,
    //                       NeuroSaidaX[i],
    //                       NeuroSaidaY[i],
    //                       TamanhoNeuronio,
    //                       TamanhoNeuronio, 0, 0);
  }

  /// Desenhar neuronios

  for (int i = 0; i < qtdNeuroEntrada - 1; i++) {
    //        NeuroEntradaX[i] = XOrigem;
    //        NeuroEntradaY[i] = YOrigem - i*EscalaAltura;

    PIG_Cor cor;
    double Opacidade;

    if (i == CAR_BRAIN_QTD_INPUT - 1) {
      if (Entrada[i] > 15) {
        Opacidade = 1;
      } else {
        Opacidade = fabs(Entrada[i]) / 15.0;
      }
      cor = calcularCor(Opacidade, BRANCO);
    } else {
      if (Entrada[i] > 200.0) {
        Opacidade = 0;
      } else {
        Opacidade = fabs(200.0 - Entrada[i]) / 200.0;
      }
      cor = calcularCor(Opacidade, BRANCO);
    }

    jogo->definirColoracao(SpriteNeuronAtivado, cor);
    jogo->definirOpacidade(SpriteLuzAmarelo, Opacidade * 255);

    //        desenharSprite(SpriteLuzAmarelo,
    //                       NeuroEntradaX[i],
    //                       NeuroEntradaY[i],
    //                       3.5*TamanhoNeuronio,
    //                       3.5*TamanhoNeuronio, 0, 0);

    jogo->desenharSprite(spriteContornoNeuronio, NeuroEntradaX[i],
                         NeuroEntradaY[i], TamanhoNeuronio * 1.1,
                         TamanhoNeuronio * 1.1, 0, 0);

    jogo->desenharSprite(SpriteNeuronAtivado, NeuroEntradaX[i],
                         NeuroEntradaY[i], TamanhoNeuronio, TamanhoNeuronio, 0,
                         0);

    // definirOpacidade(SpriteLuzAmarelo, 255);
    jogo->definirColoracao(SpriteNeuronAtivado, BRANCO);

    //        desenharSprite(SpriteSeta,
    //                       NeuroEntradaX[i] - 56,
    //                       NeuroEntradaY[i],
    //                       64/(camera->zoom+1),
    //                       12/(camera->zoom+1), 0);
  }

  for (int i = 0; i < qtdEscondidas; i++) {
    //        int qtdCamadaAnterior;
    //        Camada* CamadaAnterior;
    //        double *XAnterior, *YAnterior;
    //
    //        if(i == 0)
    //        {
    //            qtdCamadaAnterior = qtdNeuroEntrada;
    //            CamadaAnterior = &MelhorDinossauro->cerebro->CamadaEntrada;
    //            XAnterior = NeuroEntradaX;
    //            YAnterior = NeuroEntradaY;
    //        }
    //        else
    //        {
    //            qtdCamadaAnterior = qtdNeuroEscondidas;
    //            CamadaAnterior =
    //            &MelhorDinossauro->cerebro->CamadaEscondida[i-1]; XAnterior =
    //            NeuroEscondidoX[i-1]; YAnterior = NeuroEscondidoY[i-1];
    //        }

    for (int j = 0; j < qtdNeuroEscondidas - 1; j++) {
      //            NeuroEscondidoX[i][j] = XOrigem + (i+1)*EscalaLargura;
      //            NeuroEscondidoY[i][j] = YOrigem - j*EscalaAltura;

      Sprite = SpriteNeuronDesativado;
      double SaidaNeuronio =
          melhorCarro->Cerebro->CamadaEscondida[i].Neuronios[j].Saida;
      if (SaidaNeuronio > 0) {
        Sprite = SpriteNeuronAtivado;
        //                desenharSprite( SpriteLuzAmarelo,
        //                                NeuroEscondidoX[i][j],
        //                                NeuroEscondidoY[i][j],
        //                                3.5*TamanhoNeuronio,
        //                                3.5*TamanhoNeuronio, 0, 0);
      }

      //            for(int k=0; k<qtdCamadaAnterior-1; k++)
      //            {
      //                double peso =
      //                MelhorDinossauro->cerebro->CamadaEscondida[i].neuronios[j].peso[k];
      //                double saida = CamadaAnterior->neuronios[k].saida;
      //                if(peso*saida > 0)
      //                {
      //                    DesenharLinhaSimples(XAnterior[k],
      //                                         YAnterior[k],
      //                                         NeuroEscondidoX[i][j],
      //                                         NeuroEscondidoY[i][j],
      //                                         VERMELHO);
      //
      //                }
      //                else
      //                {
      //                    DesenharLinhaSimples(XAnterior[k],
      //                                         YAnterior[k],
      //                                         NeuroEscondidoX[i][j],
      //                                         NeuroEscondidoY[i][j], PRETO);
      //                }
      //            }

      jogo->desenharSprite(spriteContornoNeuronio, NeuroEscondidoX[i][j],
                           NeuroEscondidoY[i][j], TamanhoNeuronio * 1.1,
                           TamanhoNeuronio * 1.1, 0, 0);

      jogo->desenharSprite(Sprite, NeuroEscondidoX[i][j], NeuroEscondidoY[i][j],
                           TamanhoNeuronio, TamanhoNeuronio, 0, 0);
    }
  }

  for (int i = 0; i < qtdNeuroSaida; i++) {
    //        int UltimaCamada =
    //        MelhorDinossauro->cerebro->QuantidadeEscondidas-1; double temp =
    //        YOrigem - (EscalaAltura*(qtdNeuroEscondidas-2))/2.0 +
    //        (EscalaAltura*(qtdNeuroSaida-1))/2.0;
    //
    //        NeuroSaidaX[i] = XOrigem + (qtdEscondidas+1)*EscalaLargura;
    //        NeuroSaidaY[i] = temp - i*EscalaAltura;

    Sprite = SpriteNeuronDesativado;
    double SaidaNeuronio = melhorCarro->Cerebro->CamadaSaida.Neuronios[i].Saida;
    if (SaidaNeuronio > 0.5) {
      Sprite = SpriteNeuronAtivado;
      //            desenharSprite(SpriteLuzAmarelo,
      //                       NeuroSaidaX[i],
      //                       NeuroSaidaY[i],
      //                       3.5*TamanhoNeuronio,
      //                       3.5*TamanhoNeuronio, 0, 0);
    }

    //        for(int k=0; k<qtdNeuroEscondidas-1; k++)
    //        {
    //            double peso =
    //            MelhorDinossauro->cerebro->CamadaSaida.neuronios[i].peso[k];
    //            double saida =
    //            MelhorDinossauro->cerebro->CamadaEscondida[UltimaCamada].neuronios[k].saida;
    //
    //            if(peso*saida > 0)
    //            {
    //                DesenharLinhaSimples(NeuroEscondidoX[UltimaCamada][k],
    //                                     NeuroEscondidoY[UltimaCamada][k],
    //                                     NeuroSaidaX[i],
    //                                     NeuroSaidaY[i], VERMELHO);
    //            }
    //            else
    //            {
    //                DesenharLinhaSimples(NeuroEscondidoX[UltimaCamada][k],
    //                                     NeuroEscondidoY[UltimaCamada][k],
    //                                     NeuroSaidaX[i],
    //                                     NeuroSaidaY[i], PRETO);
    //            }
    //        }
    jogo->desenharSprite(spriteContornoNeuronio, NeuroSaidaX[i], NeuroSaidaY[i],
                         TamanhoNeuronio * 1.1, TamanhoNeuronio * 1.1, 0, 0);

    jogo->desenharSprite(Sprite, NeuroSaidaX[i], NeuroSaidaY[i],
                         TamanhoNeuronio, TamanhoNeuronio, 0, 0);
  }
}

void CriarTexturaExplosoes(std::string &caminhoImagem, PIG *jogo) {
  std::string caminho;

  // PIG_Cor Cor = ((PIG_Cor){150,50,150,255});
  // int Opacidade = 150;

  for (int i = 1; i <= 44; i++) {
    caminho = caminhoImagem + "explosao/exp (" + std::to_string(i) + ").png";
    SpritesExplosao[i] = jogo->criarSprite(caminho, 0);
    // definirColoracao(SpritesExplosao[i],Cor);
    // definirOpacidade(SpritesExplosao[i],Opacidade);
  }
}

void CriarExplosao(int X, int Y, int Largura, int Altura, double Angulo) {
  int i;

  for (i = 0; i < MAX_EXPLOSOES; i++) {
    if (ListaExplosoes[i].Ativada == 0) {
      ListaExplosoes[i].Ativada = 1;

      ListaExplosoes[i].X = X;
      ListaExplosoes[i].Y = Y;
      ListaExplosoes[i].Largura = Largura;
      ListaExplosoes[i].Altura = Altura;
      ListaExplosoes[i].FrameAtual = 0;
      ListaExplosoes[i].QuantidadeFrames = 44;
      ListaExplosoes[i].Angulo = Angulo;
      return;
    }
  }
}

void TrocarFrameExplosoes() {

  for (int i = 0; i < MAX_EXPLOSOES; i++) {
    if (ListaExplosoes[i].Ativada == 1) {
      ListaExplosoes[i].FrameAtual = ListaExplosoes[i].FrameAtual + 1;

      if (ListaExplosoes[i].FrameAtual > ListaExplosoes[i].QuantidadeFrames) {
        ListaExplosoes[i].Ativada = 0;
      }
    }
  }
}

void DesenharFrameExplosoes(PIG *jogo) {
  int i;

  auto camera = jogo->pegarCamera();

  for (i = 0; i < MAX_EXPLOSOES; i++) {
    if (ListaExplosoes[i].Ativada == 1) {
      double X = ListaExplosoes[i].X;
      double Y = ListaExplosoes[i].Y;

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      jogo->desenharSprite(SpritesExplosao[ListaExplosoes[i].FrameAtual], X, Y,
                           ListaExplosoes[i].Largura, ListaExplosoes[i].Altura,
                           ListaExplosoes[i].Angulo);
    }
  }
}

void desenhar(PIG *jogo) {
  double X, Y;
  char String[1000];

  jogo->iniciaDesenho();

  auto teclado = jogo->pegaTeclado();

  auto camera = jogo->pegarCamera();

  jogo->desenharSprite(spriteEstrelasDesligada, LARG_TELA / 2, ALT_TELA / 2,
                       LARG_TELA, ALT_TELA, 0, 0);
  jogo->desenharSprite(spriteEstrelasLigada, LARG_TELA / 2, ALT_TELA / 2,
                       LARG_TELA, ALT_TELA, 0, 0);

  if (teclado[TECLA_g]) {
    int indice = buscarMelhorCarroVivo();
    camera->posX = -carros[indice].X + LARG_TELA / 2;
    camera->posY = -carros[indice].Y + ALT_TELA / 2;
  }

  /// Desenhar mapa de colis�o ou spritePista
  if (teclado[TECLA_z]) {
    for (int i = 0; i < quantidadeParede; i++) {
      X = paredes[i].x;
      Y = paredes[i].y;

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      paredesVirtual[i].x = X;
      paredesVirtual[i].y = ALT_TELA - Y;
    }

    jogo->desenharPontos(paredesVirtual, quantidadeParede, BRANCO);
  } else {
    X = LARGURA_CENARIO / 2;
    Y = ALTURA_CENARIO / 2;
    //
    X = ((X + camera->posX) +
         ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
    Y = ((Y + camera->posY) +
         ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

    jogo->desenharSprite(spritePista, X, Y, LARGURA_CENARIO, ALTURA_CENARIO, 0);

    /// Desenhando obstaculos

    for (int i = 0; i < QTD_OBSTACULO; i++) {
      X = obstaculos[i].X;
      Y = obstaculos[i].Y;

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      jogo->desenharSprite(spriteEspinho, X, Y, obstaculos[i].tamanhoX,
                           obstaculos[i].tamanhoY, obstaculos[i].angulo, 1);
    }

    /// Desenhando zonas

    for (int i = 0; i < QTD_ZONA; i++) {
      X = zonas[i].X;
      Y = zonas[i].Y;

      double X1 = X - zonas[i].larguraX / 2;
      double Y1 = Y - zonas[i].larguraY / 2;

      X1 = ((X1 + camera->posX) +
            ((X1 + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y1 = ((Y1 + camera->posY) +
            ((Y1 + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      int sprite;
      if (zonas[i].valor > 0) {
        sprite = spriteTurbo;
      } else {
        sprite = spriteLama;
      }

      //            DesenharRetangulo(X1,
      //                              Y1,
      //                              zonas[i].larguraY*(camera->zoom+1),
      //                              zonas[i].larguraX*(camera->zoom+1),VERDE);

      jogo->desenharSprite(sprite, X, Y, zonas[i].larguraX, zonas[i].larguraY,
                           zonas[i].angulo, 1);
    }
  }

  /// Desenhar carros colididos

  for (int i = 0; i < QTD_CARROS; i++) {
    if (carros[i].Colidiu == 1) {
      X = carros[i].X;
      Y = carros[i].Y;

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      int Altura = 20;
      if (carros[i].Queimado == 1) {
        jogo->desenharSprite(spriteCarroQueimado, X, Y, Altura * 2.1, Altura,
                             carros[i].Angulo);
      } else {
        jogo->desenharSprite(carros[i].Sprite, X, Y, Altura * 2.1, Altura,
                             carros[i].Angulo);
      }
    }
  }
  //
  /// Desenhar carros vivos

  for (int i = 0; i < QTD_CARROS; i++) {
    if (carros[i].Colidiu == 0) {
      X = carros[i].X;
      Y = carros[i].Y;

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      int Altura = 20;
      if (carros[i].Queimado == 1) {
        jogo->desenharSprite(spriteCarroQueimado, X, Y, Altura * 2.1, Altura,
                             carros[i].Angulo);
      } else {
        jogo->desenharSprite(carros[i].Sprite, X, Y, Altura * 2.1, Altura,
                             carros[i].Angulo);
      }
    }
  }

  /// Desenhar sensores do melhorCarro
  if (teclado[TECLA_c]) {
    int indice = buscarMelhorCarroVivo();

    aplicarSensores(&carros[indice], NULL);

    double X1 = carros[indice].X;
    double Y1 = carros[indice].Y;

    X1 = ((X1 + camera->posX) +
          ((X1 + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
    Y1 = ((Y1 + camera->posY) +
          ((Y1 + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

    for (int i = 0; i < CAR_BRAIN_QTD_INPUT - 1; i++) {
      X = carros[indice].X +
          carros[indice].DistanciaSensores[i] *
              cos(DEGTORAD * (carros[indice].Angulo - 90.0 +
                              ((180.0 / (CAR_BRAIN_QTD_INPUT - 2)) * i)));
      Y = carros[indice].Y +
          carros[indice].DistanciaSensores[i] *
              sin(DEGTORAD * (carros[indice].Angulo - 90.0 +
                              ((180.0 / (CAR_BRAIN_QTD_INPUT - 2)) * i)));

      int XNumero = X;
      int YNumero = Y - 10;

      XNumero = ((XNumero + camera->posX) +
                 ((XNumero + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      YNumero = ((YNumero + camera->posY) +
                 ((YNumero + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      X = ((X + camera->posX) +
           ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
      Y = ((Y + camera->posY) +
           ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

      if (i < CAR_BRAIN_QTD_INPUT / 4 || i > 3 * CAR_BRAIN_QTD_INPUT / 4) {
        jogo->desenhaLinhaSimples(X1, Y1, X, Y, cores[0]);
        jogo->definirOpacidade(SpriteLuzAmarelo, 255);
        jogo->desenharSprite(SpriteLuzAmarelo, X, Y, 15, 15, 0, 1);
      } else {
        jogo->desenhaLinhaSimples(X1, Y1, X, Y, cores[1]);
        jogo->desenharSprite(SpriteLuzAzul, X, Y, 15, 15, 0, 1);
      }

      // sprintf(String,"%.0f", carros[indice].DistanciaSensores[i]);
      // escreverCentralizada(String, XNumero, YNumero, Fonte);
    }
  }

  auto event = jogo->pegarEvento();

  /// Desenhar matrizDistancias
  if (teclado[TECLA_x]) {
    int MouseX = AlgebraLinear::XVirtualParaReal(
        event.mouse.posX, event.mouse.posY, camera->cameraRotationZ,
        camera->rotationXY, camera->posX, camera->posY);
    int MouseY = AlgebraLinear::YVirtualParaReal(
        event.mouse.posX, event.mouse.posY, camera->cameraRotationZ,
        camera->rotationXY, camera->posY, camera->zoom);

    for (int i = -25; i < 25; i++) {
      for (int j = -25; j < 25; j++) {
        X = i + MouseX;
        Y = j + MouseY;

        sprintf(String, "%d", distancias[(int)X][(int)Y]);

        X = ((X + camera->posX) +
             ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
        Y = ((Y + camera->posY) +
             ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

        jogo->escreverCentralizada(String, X, Y, Fonte);
      }
    }
  }

  /// Desenhar Laser
  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      if (cenario[i][j] == 1) {
        if (distancias[i][j] == valorDesfazerPista) {
          X = i;
          Y = j;

          X = ((X + camera->posX) +
               ((X + camera->posX) - (LARG_TELA / 2.0)) * camera->zoom);
          Y = ((Y + camera->posY) +
               ((Y + camera->posY) - (ALT_TELA / 2.0)) * camera->zoom);

          if (3 * (camera->zoom + 1) < 1) {
            jogo->desenhaRetangulo(X, Y, 1, 1, VERDE);
          } else {
            jogo->desenhaRetangulo(X, Y, 3 * (camera->zoom + 1),
                                   3 * (camera->zoom + 1), VERDE);
          }
        }
      }
    }
  }

  DesenharFrameExplosoes(jogo);
  DesenharRedeNeural(-230, 660, 600, 75, jogo);

  jogo->desenharMouse();
  jogo->encerraDesenho();
  //    EncerrarDesenho();
}

void movimentarCamera(PIG_Teclado teclado, Camera *camera) {

  double A = 10.0 / (camera->zoom + 1);
  if (teclado[TECLA_w] == 1) {
    camera->posY = camera->posY - A;
  }
  if (teclado[TECLA_s] == 1) {
    camera->posY = camera->posY + A;
  }
  if (teclado[TECLA_a] == 1) {
    camera->posX = camera->posX + A;
  }
  if (teclado[TECLA_d] == 1) {
    camera->posX = camera->posX - A;
  }

  double B = 0.2;
  double C = (B / (3.0 / (camera->zoom + 1)));
  if (teclado[TECLA_q] == 1) {
    if (camera->zoom >= 0) {
      camera->zoom = camera->zoom + B;
    } else {
      camera->zoom = camera->zoom + C;
    }

    if (camera->zoom >= 50) {
      camera->zoom = 50;
    }
  }
  if (teclado[TECLA_e] == 1) {
    if (camera->zoom >= 0) {
      camera->zoom = camera->zoom - B;
    } else {
      camera->zoom = camera->zoom - C;
    }

    if (camera->zoom <= -0.9999) {
      camera->zoom = -0.9999;
    }
  }
}

void inicializarSprites(const char *diretorioImagem, PIG *jogo) {

  std::string dir = diretorioImagem;
  std::string carro = "carro";

  std::string png = ".png";
  std::string bmp = ".bmp";

  std::string imagemPistaObstaculo = dir + "pistaObstaculo" + png;
  std::string imagemEstrelasLigada = dir + "star" + bmp;
  std::string imagemEstrelasDesligada = dir + "star2" + bmp;
  std::string imagemEspinho = dir + "spike" + png;
  std::string imagemLama = dir + "lama" + png;
  std::string imagemTurbo = dir + "turbo" + png;

  spritePista = jogo->criarSprite(imagemPistaObstaculo, 0);

  for (int i = 0; i < 7; i++) {
    std::string imagemSpriteCarro = dir + carro + std::to_string(i) + png;
    spriteCarros[i] = jogo->criarSprite(imagemSpriteCarro, 0);
  }
  std::string imagemCarroQueimado = dir + "carroQueimado" + png;
  spriteCarroQueimado = jogo->criarSprite(imagemCarroQueimado, 0);
  std::string ImagemEstrelasLigada = dir + "star" + bmp;

  spriteEstrelasLigada = jogo->criarSprite(imagemEstrelasLigada, 1);
  spriteEstrelasDesligada = jogo->criarSprite(imagemEstrelasDesligada, 0);
  spriteEspinho = jogo->criarSprite(imagemEspinho, 0);
  spriteLama = jogo->criarSprite(imagemLama, 0);
  spriteTurbo = jogo->criarSprite(imagemTurbo, 0);

  CriarTexturaExplosoes(dir, jogo);

  InicializarSpritesNeuronios(dir, jogo);

  printf("Sprites carregados \n");
}

void preencherMatrizColisao(PIG *jogo) {
  PIG_Cor cor;
  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      cor = jogo->getPixelColor(spritePista, i, j);
      if (cor.r == 110 && cor.g == 110 && cor.b == 110 && cor.a == 255) {
        cenario[i][j] = 1;
      } else {
        cenario[i][j] = 0;
      }
    }
  }
}

void criarPontoDesenhoColisao() {
  quantidadeParede = 0;

  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      if (cenario[i][j] == 1) {
        paredes[quantidadeParede].x = i;
        paredes[quantidadeParede].y = j;
        quantidadeParede++;
      }
    }
  }
}

double getRandomValue() {
  return (rand() % 20001 / 10.0) - 1000.0;
  // return (rand()%201/10.0) - 10.0;
  // return (rand()%2001/1000.0) - 1.0;
  // return (rand()%2001/10000.0) - 0.1;

  // return rand()%3 - 1;
}

void inicializarObstaculo(Obstaculo *obs, double X, double Y, int emMovimento,
                          double VX, double VY, double rangeMaxMovimento,
                          double angulo, double velocidadeRotacao,
                          double tamanhoX, double tamanhoY) {
  obs->angulo = angulo;
  obs->emMovimento = emMovimento;
  obs->rangeMaxMovimento = rangeMaxMovimento;
  obs->rangeAtualMovimento = 0;
  obs->tamanhoX = tamanhoX;
  obs->tamanhoY = tamanhoY;
  obs->velocidadeRotacao = velocidadeRotacao;
  obs->VX = VX;
  obs->VY = VY;
  obs->X = X;
  obs->Y = Y;

  int k = X - tamanhoX / 2;
  int l = Y - tamanhoY / 2;
  for (int i = k; i < k + tamanhoX; i++) {
    for (int j = l; j < l + tamanhoY; j++) {
      if (AlgebraLinear::distanciaEntrePontos(X, Y, i, j) < tamanhoX / 2) {
        cenario[i][j] = 0;
      }
    }
  }
}

void inicializarObstaculos() {
  int tamanho = 75;
  double velocidadeGiro = 5;

  inicializarObstaculo(&obstaculos[0], 310, 1868, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[1], 705, 1868, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[2], 500, 1930, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[3], 900, 1930, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);

  inicializarObstaculo(&obstaculos[4], 215, 467, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[5], 240, 410, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[6], 280, 369, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[7], 337, 350, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);

  inicializarObstaculo(&obstaculos[8], 335, 244, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[9], 392, 245, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[10], 434, 200, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);
  inicializarObstaculo(&obstaculos[11], 427, 150, 0, 0, 0, 0, 0, velocidadeGiro,
                       tamanho, tamanho);

  inicializarObstaculo(&obstaculos[12], 1385, 360, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[13], 1315, 470, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[14], 1385, 580, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[15], 1315, 690, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[16], 1385, 800, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[17], 1315, 910, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[18], 1385, 1020, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[19], 1315, 1130, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);

  inicializarObstaculo(&obstaculos[20], 1865, 825, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[21], 1560, 530, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
  inicializarObstaculo(&obstaculos[22], 1930, 235, 0, 0, 0, 0, 0,
                       velocidadeGiro, tamanho, tamanho);
}

void inicializarCarro(int i, double X, double Y) {
  carros[i].Angulo = 90;
  carros[i].X = X;
  carros[i].Y = Y;
  carros[i].VX = 0;
  carros[i].VY = 0;
  carros[i].Velocidade = 0;
  carros[i].Colidiu = 0;
  carros[i].Queimado = 0;

  RNA_CopiarVetorParaCamadas(carros[i].Cerebro, carros[i].DNA);
}

void alocarCarro(int i) {
  int Tamanho;

  carros[i].Sprite = spriteCarros[rand() % QTD_SPRITES_CARROS];

  carros[i].Cerebro =
      RNA_CriarRedeNeural(CAR_BRAIN_QTD_LAYERS, CAR_BRAIN_QTD_INPUT,
                          CAR_BRAIN_QTD_HIDE, CAR_BRAIN_QTD_OUTPUT);

  Tamanho = RNA_QuantidadePesos(carros[i].Cerebro);

  carros[i].TamanhoDNA = Tamanho;
  carros[i].DNA = (double *)malloc(Tamanho * sizeof(double));

  for (int j = 0; j < carros[i].TamanhoDNA; j++) {
    carros[i].DNA[j] = getRandomValue();
  }
}

void alocarCarros() {
  for (int i = 0; i < QTD_CARROS; i++) {
    alocarCarro(i);
  }
}

void inicializarNovaPartida() {
  carrosColididos = 0;
  valorDesfazerPista = maiorDistancia;
  velocidadeLaser = 1;

  for (int i = 0; i < QTD_CARROS; i++) {
    inicializarCarro(i, 1852 + rand() % 95, 1200);

    /// 1225 at� 1320
  }
}

int valorMenorVizinho(int i, int j) {
  int vizinhos[8];

  vizinhos[0] = distancias[i - 1][j + 1];
  vizinhos[1] = distancias[i][j + 1];
  vizinhos[2] = distancias[i + 1][j + 1];
  vizinhos[3] = distancias[i - 1][j];

  vizinhos[4] = distancias[i + 1][j];
  vizinhos[5] = distancias[i - 1][j - 1];
  vizinhos[6] = distancias[i][j - 1];
  vizinhos[7] = distancias[i + 1][j - 1];

  int Menor = 9999999;
  int indice = -1;
  for (int i = 0; i < 8; i++) {
    if (vizinhos[i] < Menor && vizinhos[i] >= 0) {
      Menor = vizinhos[i];
      indice = i;
    }
  }
  return Menor;
}

void salvarDistanciasArquivo() {
  FILE *f = fopen("matrizDistancias", "wb");
  for (auto &distancia : distancias) {
    for (int &valor : distancia) {
      fwrite(&valor, 1, 4, f);
    }
  }
  fclose(f);
}

void carregarDistanciasArquivo(FILE *f) {
  for (auto &distancia : distancias) {
    for (int &valor : distancia) {
      fread(&valor, 1, 4, f);
    }
  }
}

int buscarMaiorDistancia() {
  int maior = 0;
  int indicek = 0, indicem = 0;
  for (int k = 0; k < LARGURA_CENARIO; k++) {
    for (int m = 0; m < ALTURA_CENARIO; m++) {
      if (cenario[k][m] == 1) {
        if (distancias[k][m] > maior && distancias[k][m] != 9999999) {
          maior = distancias[k][m];
          indicek = k;
          indicem = m;
        }
      }
    }
  }
  // printf("(%d,%d)\n",indicek,indicem);
  return maior;
}

void zerarLinhaChegada() { distancias[1346][266] = 0; }

void criarMatrizDistancias() {
  int novo;
  int mudancas;

  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      distancias[i][j] = 9999999;
    }
  }

  zerarLinhaChegada();

  while (1) {
    mudancas = 0;

    for (int k = 0; k < LARGURA_CENARIO; k++) {
      for (int m = 0; m < ALTURA_CENARIO; m++) {
        if (cenario[k][m] == 1) {
          novo = 1 + valorMenorVizinho(k, m);
          if (novo < distancias[k][m]) {
            distancias[k][m] = novo;
            mudancas++;
          }
        }
      }
    }

    // printf("Mudancas: %d\n",mudancas);

    if (mudancas == 0) {
      break;
    }
  }

  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      if (cenario[i][j] == 0) {
        distancias[i][j] = -1;
      }
    }
  }

  zerarLinhaChegada();
}

void preencherMatrizDistancias(std::string &arquivoMatrizDistancias) {

  FILE *f = fopen(arquivoMatrizDistancias.c_str(), "rb");
  if (f == NULL) {
    printf("criando Matriz de distâncias \n");
    criarMatrizDistancias();
    salvarDistanciasArquivo();
  } else {
    carregarDistanciasArquivo(f);
    fclose(f);
  }

  maiorDistancia = buscarMaiorDistancia();
}

void inicializarZona(Zona *zona, double X, double Y, double larguraX,
                     double larguraY, double valor, double angulo) {
  zona->larguraX = larguraX;
  zona->larguraY = larguraY;
  zona->X = X;
  zona->Y = Y;
  zona->valor = valor;
  zona->angulo = angulo;
}

void inicializarZonas() {
  double valorIncremento = 0.75;
  double valorDecremento = -1;

  inicializarZona(&zonas[0], 1520, 1610, 75, 75, valorDecremento, 0);

  inicializarZona(&zonas[1], 130, 1230, 75, 75, valorIncremento, 270);
  inicializarZona(&zonas[2], 790, 350, 75, 75, valorDecremento, 0);
  inicializarZona(&zonas[3], 790, 900, 75, 75, valorDecremento, 0);
  inicializarZona(&zonas[4], 669, 630, 75, 75, valorIncremento, 90);

  inicializarZona(&zonas[5], 1042, 965, 75, 75, valorIncremento, 270);
  inicializarZona(&zonas[6], 1164, 600, 75, 75, valorDecremento, 90);
  inicializarZona(&zonas[7], 1040, 265, 75, 75, valorDecremento, 90);

  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      matrizBoost[i][j] = 0;

      for (int k = 0; k < QTD_ZONA; k++) {
        double inicioX = zonas[k].X - zonas[k].larguraX / 2.0;
        double finalX = zonas[k].X + zonas[k].larguraX / 2.0;
        double inicioY = zonas[k].Y - zonas[k].larguraY / 2.0;
        double finalY = zonas[k].Y + zonas[k].larguraY / 2.0;

        if (inicioX <= i && i <= finalX && inicioY <= j && j <= finalY) {
          matrizBoost[i][j] = zonas[k].valor;
        }
      }
    }
  }
}

PIG *configuracoesIniciais(const char *arquivoFonte,
                           const char *diretorioImagem,
                           const char *arquivoMatrizDistancias) {

  //    CriarJanela("Deep Cars", arquivoFonte, 0);

  PIG *jogo = new PIG("Deep Cars", arquivoFonte, 0);

  std::string matrizDistancias = arquivoMatrizDistancias;

  inicializarSprites(diretorioImagem, jogo);

  TimerGeral = jogo->criarTimer();

  InicializarGeradorAleatorio();
  preencherMatrizColisao(jogo);
  printf("Matriz de colisao preenchida\n");
  preencherMatrizDistancias(matrizDistancias);
  printf("Matriz de distancias preenchida\n");

  inicializarObstaculos();
  inicializarZonas();

  criarPontoDesenhoColisao();

  alocarCarros();

  Fonte = jogo->criaFonteNormal((char *)arquivoFonte, 15, ESTILO_NORMAL, VERDE,
                                0, PRETO);

  DistanciaRecorde = 0;
  Geracao = 0;

  inicializarNovaPartida();

  return jogo;
}

void movimentarCarros() {
  for (int i = 0; i < QTD_CARROS; i++) {
    if (carros[i].Colidiu == 0) {
      double TempVelocidade;

      if (matrizBoost[(int)carros[i].X][(int)carros[i].Y] != 0) {
        if (matrizBoost[(int)carros[i].X][(int)carros[i].Y] < 0) {
          TempVelocidade = carros[i].Velocidade / 5.0;
        } else {
          carros[i].Velocidade =
              carros[i].Velocidade +
              matrizBoost[(int)carros[i].X][(int)carros[i].Y];
          TempVelocidade = carros[i].Velocidade;
        }
      } else {
        TempVelocidade = carros[i].Velocidade;
      }

      carros[i].VX = TempVelocidade * cos(DEGTORAD * carros[i].Angulo);
      carros[i].VY = TempVelocidade * sin(DEGTORAD * carros[i].Angulo);

      if (cenario[(int)(carros[i].X + carros[i].VX)]
                 [(int)(carros[i].Y + carros[i].VY)] == 0 ||

          cenario[(int)(carros[i].X + 18 * cos(DEGTORAD * carros[i].Angulo) +
                        carros[i].VX)]
                 [(int)(carros[i].Y + 18 * sin(DEGTORAD * carros[i].Angulo) +
                        carros[i].VY)] == 0 ||

          cenario[(int)(carros[i].X - 18 * cos(DEGTORAD * carros[i].Angulo) +
                        carros[i].VX)]
                 [(int)(carros[i].Y - 18 * sin(DEGTORAD * carros[i].Angulo) +
                        carros[i].VY)] == 0) {
        carros[i].Colidiu = 1;
        carrosColididos++;
      } else {
        carros[i].X = carros[i].X + carros[i].VX;
        carros[i].Y = carros[i].Y + carros[i].VY;
      }

      if (carros[i].Velocidade > 0) {
        carros[i].Velocidade = carros[i].Velocidade - 0.1;
        if (carros[i].Velocidade < 0) {
          carros[i].Velocidade = 0;
        }
      }
    }
  }
}

void verificarInteracaoJogador(PIG_Teclado teclado) {

  if (teclado[TECLA_CIMA]) {
    carros[0].Velocidade = carros[0].Velocidade + 0.2;
  }
  if (teclado[TECLA_BAIXO]) {
    carros[0].Velocidade = carros[0].Velocidade - 0.2;
    if (carros[0].Velocidade < -8) {
      carros[0].Velocidade = -8;
    }
  }
  if (teclado[TECLA_ESQUERDA]) {
    carros[0].Angulo = carros[0].Angulo + 5.0;
  }
  if (teclado[TECLA_DIREITA]) {
    carros[0].Angulo = carros[0].Angulo - 5.0;
  }

  if (teclado[TECLA_KP_8]) {
    carros[1].Velocidade = carros[1].Velocidade + 0.2;
  }
  if (teclado[TECLA_KP_5]) {
    carros[1].Velocidade = carros[1].Velocidade - 0.2;
  }
  if (teclado[TECLA_KP_4]) {
    carros[1].Angulo = carros[1].Angulo + 5.0;
  }
  if (teclado[TECLA_KP_6]) {
    carros[1].Angulo = carros[1].Angulo - 5.0;
  }

  if (teclado[TECLA_t]) {
    carros[2].Velocidade = carros[2].Velocidade + 0.2;
  }
  if (teclado[TECLA_g]) {
    carros[2].Velocidade = carros[2].Velocidade - 0.2;
  }
  if (teclado[TECLA_f]) {
    carros[2].Angulo = carros[2].Angulo + 5.0;
  }
  if (teclado[TECLA_h]) {
    carros[2].Angulo = carros[2].Angulo - 5.0;
  }
}

void carregarRedeArquivo() {
  FILE *f = fopen("rede", "rb");

  if (f == NULL) {
    return;
  }

  fread(&carros[0].TamanhoDNA, 1, sizeof(int), f);
  fread(carros[0].DNA, carros[0].TamanhoDNA, sizeof(double), f);
  RNA_CopiarVetorParaCamadas(carros[0].Cerebro, carros[0].DNA);

  fclose(f);

  // RNA_CopiarVetorParaCamadas(Dinossauros[0].cerebro, Dinossauros[0].DNA);
}

void salvarRedeArquivo() {
  char String[1000];
  int indice = buscarMelhorFitness();

  double fit = carros[indice].Fitness;
  double dist =
      (double)distancias[(int)carros[indice].X][(int)carros[indice].Y];

  printf("Salvando em arquivo, melhor da geracao: %f pixels\n",
         maiorDistancia - dist);

  sprintf(String, "redes\\%.2f - [%d,%d,%d,%d]", fit, CAR_BRAIN_QTD_LAYERS,
          CAR_BRAIN_QTD_INPUT, CAR_BRAIN_QTD_HIDE, CAR_BRAIN_QTD_OUTPUT);

  FILE *f = fopen(String, "wb");

  fwrite(&carros[indice].TamanhoDNA, 1, sizeof(int), f);
  fwrite(carros[indice].DNA, carros[indice].TamanhoDNA, sizeof(double), f);

  fclose(f);
}

void aplicarSaida(Carro *carro, double *saida) {
  if (saida[0] > 0) {
    carro->Velocidade = carro->Velocidade + 0.2;
  }
  if (saida[1] > 0) {
    carro->Velocidade = carro->Velocidade - 0.2;
    if (carro->Velocidade < -4) {
      carro->Velocidade = -4;
    }
  }
  if (saida[2] > 0) {
    carro->Angulo = carro->Angulo + 5.0;
  }
  if (saida[3] > 0) {
    carro->Angulo = carro->Angulo - 5.0;
  }
}

void controlarCarros() {
  double Saida[CAR_BRAIN_QTD_OUTPUT];
  double Entrada[CAR_BRAIN_QTD_INPUT];

  for (int i = 0; i < QTD_CARROS; i++) {
    if (carros[i].Colidiu == 0) {
      aplicarSensores(&carros[i], Entrada);

      RNA_CopiarParaEntrada(carros[i].Cerebro, Entrada);
      RNA_CalcularSaida(carros[i].Cerebro);
      RNA_CopiarDaSaida(carros[i].Cerebro, Saida);

      if (carros[i].Colidiu == 0) {
        aplicarSaida(&carros[i], Saida);
      }
    }
  }
}

void encerrarPartida() {
  double dist;
  for (int i = 0; i < QTD_CARROS; i++) {
    carros[i].Colidiu = 1;

    dist = (double)maiorDistancia -
           (double)distancias[(int)carros[i].X][(int)carros[i].Y];

    carros[i].Fitness = dist;
  }

  salvarRedeArquivo();
}

void randomMutations() {
  static double RangeRandom = carros[0].TamanhoDNA;

  carro *Vetor[QTD_CARROS];
  carro *Temp;

  for (int i = 0; i < QTD_CARROS; i++) {
    Vetor[i] = &carros[i];
  }

  for (int i = 0; i < QTD_CARROS; i++) {
    for (int j = 0; j < QTD_CARROS - 1; j++) {
      if (Vetor[j]->Fitness < Vetor[j + 1]->Fitness) {
        Temp = Vetor[j];
        Vetor[j] = Vetor[j + 1];
        Vetor[j + 1] = Temp;
      }
    }
  }

  printf("Melhor geracao: \t\t%f\n", Vetor[0]->Fitness);

  int Step = 5;
  for (int i = 0; i < Step; i++) /// Clonando individuos
  {
    for (int j = Step + i; j < QTD_CARROS; j = j + Step) {
      for (int k = 0; k < Vetor[j]->TamanhoDNA; k++) {
        Vetor[j]->DNA[k] =
            Vetor[i]->DNA[k]; /// individuo 'j' recebe dna do individuo 'i'
      }
    }
  }

  for (int j = Step; j < QTD_CARROS; j++) {
    int tipo;
    int mutations = (rand() % (int)RangeRandom) + 1;

    for (int k = 0; k < mutations; k++) {
      tipo = rand() % 3;

      int indice = rand() % Vetor[j]->TamanhoDNA;
      switch (tipo) {
      case 0: {
        // int mutations = 20;
        Vetor[j]->DNA[indice] = getRandomValue(); /// Valor Aleatorio
      } break;
      case 1: {
        double number = (rand() % 10001) / 10000.0 + 0.5;
        Vetor[j]->DNA[indice] =
            Vetor[j]->DNA[indice] * number; /// Multiplica��o aleatoria
      } break;
      case 2: {
        double number = getRandomValue() / 100.0;
        Vetor[j]->DNA[indice] =
            Vetor[j]->DNA[indice] + number; /// Soma aleatoria
      } break;
      }
    }
  }

  printf("Range Random: \t%.1f de %d\n", RangeRandom, carros[0].TamanhoDNA);
  RangeRandom = RangeRandom * 0.999;
  if (RangeRandom < 15) {
    RangeRandom = 15;
  }

  // printf("5");
  Geracao++;
}

void verificarFimDePartida() {
  if (carrosColididos == QTD_CARROS) {
    encerrarPartida();
    randomMutations();
    inicializarNovaPartida();
  }
}

void desfazerPista(PIG(*jogo)) {
  for (int i = 0; i < LARGURA_CENARIO; i++) {
    for (int j = 0; j < ALTURA_CENARIO; j++) {
      if (distancias[i][j] <= valorDesfazerPista) {
        jogo->setPixelColor(spritePista, i, j, BRANCO);
      }
    }
  }
}

void laserDestruidor() {
  for (int i = 0; i < QTD_CARROS; i++) {
    if (distancias[(int)carros[i].X][(int)carros[i].Y] >= valorDesfazerPista) {
      if (carros[i].Colidiu == 0) {
        carros[i].Colidiu = 1;
        carrosColididos++;
      }
      if (carros[i].Queimado == 0) {
        CriarExplosao(carros[i].X, carros[i].Y, 250, 250, rand() % 360);
        carros[i].Queimado = 1;
      }
    }
  }
  if (valorDesfazerPista > 0) {
    valorDesfazerPista = valorDesfazerPista - velocidadeLaser;
    velocidadeLaser = velocidadeLaser + 0.004;
  }
}

void atualizarOpacidadeBackground(PIG *jogo) {
  static int sinal = 1;
  if (estrelasOpacidade >= 255 || estrelasOpacidade <= 0) {
    sinal = -sinal;
  }
  estrelasOpacidade = estrelasOpacidade + 5 * sinal;
  jogo->definirOpacidade(spriteEstrelasLigada, estrelasOpacidade);
}

void verificarTeclasUsuario(int tecla) {
  if (tecla == TECLA_n) {
    Periodo = Periodo / 2;
    printf("Periodo: %f\n", Periodo);
  }
  if (tecla == TECLA_m) {
    Periodo = Periodo * 2;
    printf("Periodo: %f\n", Periodo);
  }
  if (tecla == TECLA_F2) {
    carregarRedeArquivo();
  }
}

void girarObstaculos() {
  for (auto &obstaculo : obstaculos) {
    obstaculo.angulo = obstaculo.angulo + obstaculo.velocidadeRotacao;
  }
}

int main(int argc, char *args[]) {

  if (argc < 4) {
    std::cout << "precisa passar 3 parâmetros:" << std::endl;
    std::cout << "arquivo fonte, Ex: arial.tff" << std::endl;
    std::cout << "diretório imagem: Ex imagens/" << std::endl;
    std::cout << "arquivo de matriz de distancias: ./matrizDistancias" << std::endl;
    std::exit(0);
  }
  std::string arquivoFonte = args[1];
  std::string diretorioImagem = args[2];
  std::string arquivoMatrizDistancias = args[3];

  std::cout << arquivoFonte << " " << diretorioImagem << " "
            << arquivoMatrizDistancias << std::endl;

  PIG *jogo =
      configuracoesIniciais(arquivoFonte.c_str(), diretorioImagem.c_str(),
                            arquivoMatrizDistancias.c_str());

  while (jogo->getRodando()) {
    //        AtualizarJanela();
    verificarTeclasUsuario(jogo->pegarTecla());

    if (jogo->tempoDecorrido(TimerGeral) >= Periodo) {
      movimentarCamera(jogo->pegaTeclado(), jogo->pegarCamera());

      controlarCarros();
      movimentarCarros();

      girarObstaculos();

      verificarInteracaoJogador(jogo->pegaTeclado());

      laserDestruidor();

      TrocarFrameExplosoes();
      atualizarOpacidadeBackground(jogo);

      verificarFimDePartida();

      desenhar(jogo);

      jogo->reiniciarTimer(TimerGeral);
    }
  }

  delete jogo;

  return 0;
}
