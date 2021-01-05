//
// Created by samuel on 6/15/20.
//

#include <cmath>
#include "AlgebraLinear.h"
#include "../pigPack/Tipos_PIG.h"


double AlgebraLinear::AnguloEquivalente(double angulo) {
    int a = angulo / 360;

    angulo = angulo - (360 * a);
    if (angulo < 0) {
        angulo = angulo + 360.0;
    }
    return angulo;
}

/********************************
A função AnguloEntrePontos retorna o angulo entre dois pontos.
Exemplo: Se o segmento de reta que une os 2 pontos for paralelo ao eixo X
o valor retornado será 0 ou 180, dependendo da ordem dos pontos na passagem dos parametros.
********************************/

double AlgebraLinear::AnguloEntrePontos(double X1, double Y1, double X2, double Y2) {
    double Angulo;

    if (X1 == X2 && Y1 == Y2) {
        return 0.0;
    }
    Angulo = (((180.0 * atan(((double) (Y2 - Y1) / (double) (X2 - X1)))) / M_PI));

    if (X2 < X1) {
        Angulo = Angulo + 180.0;
    }
    return AnguloEquivalente(Angulo);
}

/********************************
A função distanciaEntrePontos retorna a distancia entre 2 pontos
********************************/

double AlgebraLinear::distanciaEntrePontos(double X1, double Y1, double X2, double Y2) {
    return sqrt((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2));
}

double AlgebraLinear::distanciaEntrePontos(PontoCartesiano &p1, PontoCartesiano &p2) {
    auto diferenca = p1 - p2;
    auto diferencaAoQuadrado = diferenca * diferenca;
    return sqrt(diferencaAoQuadrado.x + diferencaAoQuadrado.y);
}

/********************************
A função NumeroAleatorio retorna um numero aleatorio com os intervalos incluidos,
ou seja, os valores "Minimo" e "Maximo" também são possíveis de se obter.
********************************/

int AlgebraLinear::NumeroAleatorio(int Minimo, int Maximo) {
    Maximo = Maximo + 1;
    return (rand() % (Maximo - Minimo)) + Minimo;
}


void AlgebraLinear::RodarXY(double *X, double *Y, double cameraRotationXY) {
    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;


    double D = distanciaEntrePontos(*X, *Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, *X, *Y);

    *X = CamX + D * cos(M_Radi * (A + cameraRotationXY));
    *Y = CamY + D * sin(M_Radi * (A + cameraRotationXY));
}

double AlgebraLinear::XVirtualParaReal(double X, double Y, double cameraRotationZ, double cameraRotationXY,
                                       double cameraPosX, double cameraZoom) {
    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double C = (CamY) * sin(M_Radi * cameraRotationZ);
    double B = sin(M_Radi * cameraRotationZ) - 1;

    if (B == 0) {
        B = 0.0001;
    }

    Y = (-Y + C) / B;

    double D = distanciaEntrePontos(X, Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, X, Y);

    X = CamX + D * cos(M_Radi * (A - cameraRotationXY));

    return (X - cameraPosX + (cameraZoom * ((LARG_TELA / 2.0) - cameraPosX))) / (cameraZoom + 1);
}

double AlgebraLinear::YVirtualParaReal(double X, double Y, double cameraRotationZ, double cameraRotationXY,
                                       double cameraPosY, double cameraZoom) {
    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double C = (CamY) * sin(M_Radi * cameraRotationZ);
    double B = sin(M_Radi * cameraRotationZ) - 1;

    if (B == 0) {
        B = 0.0001;
    }

    Y = (-Y + C) / B;

    double D = distanciaEntrePontos(X, Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, X, Y);

    Y = CamY + D * sin(M_Radi * (A - cameraRotationXY));

    return (Y - cameraPosY + (cameraZoom * ((ALT_TELA / 2.0) - cameraPosY))) / (cameraZoom + 1);
}

double AlgebraLinear::XRealParaVirtual(double X, double Y, double cameraPosX, double cameraPosY, double cameraZoom,
                                       double cameraRotationXY) {
    X = ((X + cameraPosX) + ((X + cameraPosX) - (LARG_TELA / 2.0)) * cameraZoom);
    Y = ((Y + cameraPosY) + ((Y + cameraPosY) - (ALT_TELA / 2.0)) * cameraZoom);

    if (cameraRotationXY == 0.0) {
        return X;
    }

    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double D = distanciaEntrePontos(X, Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, X, Y);

    X = CamX + D * cos(M_Radi * (A + cameraRotationXY));

    return X;

    //return ((X+cameraPosX)+((X+cameraPosX)-(LARG_TELA/2.0))*cameraZoom);
}

double
AlgebraLinear::YRealParaVirtual(double X, double Y, double cameraPosX, double cameraPosY, double cameraZoom,
                                double cameraRotationXY, double cameraRotationZ, double Z) {
    X = ((X + cameraPosX) + ((X + cameraPosX) - (LARG_TELA / 2.0)) * cameraZoom);
    Y = ((Y + cameraPosY) + ((Y + cameraPosY) - (ALT_TELA / 2.0)) * cameraZoom);

    if (cameraRotationXY == 0.0 && cameraRotationZ == 0.0) {
        return Y;
    }

    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double D = distanciaEntrePontos(X, Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, X, Y);

    Y = CamY + D * sin(M_Radi * (A + cameraRotationXY));

    Y = Y - (Y - CamY - Z) * sin(M_Radi * cameraRotationZ);

    return Y;

    //return ((Y+cameraPosY)+((Y+cameraPosY)-(ALT_TELA/2.0))*cameraZoom);
}

void AlgebraLinear::XYRealParaVirtual(int *NovoX, int *NovoY, double X, double Y, double cameraPosX, double cameraPosY,
                                      double cameraZoom, double cameraRotationXY, double cameraRotationZ, double Z) {

    X = ((X + cameraPosX) + ((X + cameraPosX) - (LARG_TELA / 2.0)) * cameraZoom);
    Y = ((Y + cameraPosY) + ((Y + cameraPosY) - (ALT_TELA / 2.0)) * cameraZoom);

    if (cameraRotationXY == 0.0 && cameraRotationZ == 0.0) {
        *NovoX = X;
        *NovoY = Y;
        return;
    }

    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double D = distanciaEntrePontos(X, Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, X, Y);

    X = CamX + D * cos(M_Radi * (A + cameraRotationXY));
    Y = CamY + D * sin(M_Radi * (A + cameraRotationXY));
    Y = Y - (Y - CamY - Z) * sin(M_Radi * cameraRotationZ);

    *NovoX = X;
    *NovoY = Y;
}

void AlgebraLinear::XeYRealParaVirtual(double *X, double *Y, double cameraPosX, double cameraPosY, double cameraZoom,
                                       double cameraRotationXY, double cameraRotationZ) {
    *X = XRealParaVirtual(*X, *Y, cameraPosX, cameraPosY, cameraZoom, cameraRotationXY);
    *Y = YRealParaVirtual(*X, *Y, cameraPosX, cameraPosY, cameraZoom, cameraRotationXY, cameraRotationZ);

    double CamX = LARG_TELA / 2.0;
    double CamY = ALT_TELA / 2.0;

    double D = distanciaEntrePontos(*X, *Y, CamX, CamY);
    double A = AnguloEntrePontos(CamX, CamY, *X, *Y);

    *X = CamX + D * cos(M_Radi * (A + cameraRotationXY));
    *Y = CamY + D * sin(M_Radi * (A + cameraRotationXY));
}

