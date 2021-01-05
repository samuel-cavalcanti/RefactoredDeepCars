//
// Created by samuel on 1/5/21.
//
#include <cmath>
#include "PIG/AlgebraLinear/AlgebraLinear.h"
#include "PIG/AlgebraLinear/PontoCartesiano.h"
#include "gtest/gtest.h"

TEST (AlgebraLinar, distanciaEntrePontos) {
    auto ponto = PontoCartesiano(2, 3);
    auto outroPonto = PontoCartesiano(2, 3);
    auto distancia = AlgebraLinear::distanciaEntrePontos(ponto, outroPonto);

    auto origem = PontoCartesiano(0, 0);
    auto distancia_ate_origem = AlgebraLinear::distanciaEntrePontos(ponto, origem);
    auto distancia_ate_origem_aredondado = floor(distancia_ate_origem * 10) / 10;
    ASSERT_EQ(0.0, distancia);
    ASSERT_EQ(3.6, distancia_ate_origem_aredondado);
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();


}