//
// Created by samuel on 1/5/21.
//

#ifndef TESTDEEPCARS_PONTOCARTESIANO_H
#define TESTDEEPCARS_PONTOCARTESIANO_H


class PontoCartesiano {
public:
    PontoCartesiano(double x, double y);
    PontoCartesiano operator-(PontoCartesiano& p);
    PontoCartesiano operator*(PontoCartesiano& p);
    double x;
    double y;

};


#endif //TESTDEEPCARS_PONTOCARTESIANO_H
