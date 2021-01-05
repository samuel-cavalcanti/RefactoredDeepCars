
/********************************
A função AnguloEquivalente recebe um angulo qualquer e te retorna o equivalente positivo (de 0 a 360)
Exemplo: AnguloEquivalente(720) retorna 0 pois 720 graus é equivalente a 0 graus no circulo trigonometrico;
********************************/
#ifndef ALGEBRA_LINEAR_VICTOR_H
#define ALGEBRA_LINEAR_VICTOR_H


#define DEGTORAD (0.01745329251994329576)
#define RADTODEG (57.29577951307854999853275233034)


#define M_Radi DEGTORAD

#include "PontoCartesiano.h"

namespace AlgebraLinear {


    double AnguloEquivalente(double angulo);

    double AnguloEntrePontos(double X1, double Y1, double X2, double Y2);

    double distanciaEntrePontos(double X1, double Y1, double X2, double Y2);

    double distanciaEntrePontos(PontoCartesiano &p1, PontoCartesiano &p2);

    int NumeroAleatorio(int Minimo, int Maximo);

    void RodarXY(double *X, double *Y, double cameraRotationXY);

    double XVirtualParaReal(double X, double Y, double cameraRotationZ, double cameraRotationXY,
                            double cameraPosX, double cameraZoom);

    double YVirtualParaReal(double X, double Y, double cameraRotationZ, double cameraRotationXY,
                            double cameraPosY, double cameraZoom);

    double XRealParaVirtual(double X, double Y, double cameraPosX, double cameraPosY, double cameraZoom,
                            double cameraRotationXY);

    double YRealParaVirtual(double X, double Y, double cameraPosX, double cameraPosY, double cameraZoom,
                            double cameraRotationXY, double cameraRotationZ, double Z = 0);

    void XYRealParaVirtual(int *NovoX, int *NovoY, double X, double Y, double cameraPosX, double cameraPosY,
                           double cameraZoom, double cameraRotationXY, double cameraRotationZ, double Z = 0);

    void XeYRealParaVirtual(double *X, double *Y, double cameraPosX, double cameraPosY, double cameraZoom,
                            double cameraRotationXY, double cameraRotationZ);

};


#endif // ALGEBRA_LINEAR_VICTOR_H