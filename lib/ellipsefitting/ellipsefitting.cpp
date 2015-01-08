#include "ellipsefitting.h"

using namespace arma;

void fitEllipse(const mat &aPoints, colvec2 &z, float &a, float &b, float &alpha)
    {
    mat X(aPoints.n_rows, 6);
    for (int i = 0; i < (int)aPoints.n_rows; ++i)
        {
        rowvec row;
        row << aPoints(i, 0)*aPoints(i, 0) << aPoints(i, 0)*aPoints(i, 1) << aPoints(i, 1)*aPoints(i, 1) << aPoints(i, 0) << aPoints(i, 1) << 1;
        X.row(i) = row;
        }

    mat U; vec6 s; mat66 V;
    svd(U, s, V, X);

    colvec6 u = V.col(5);

    mat22 A; A << u(0) << u(1)/2 << endr << u(1)/2 << u(2) << endr;
    colvec2 bb; bb << u(3)<< u(4);
    float c = u(5);

    colvec2 D; mat22 Q;
    eig_sym(D, Q, A);

    colvec2 bs = Q.t()*bb;

    colvec2 zs = solve(-(2*diagmat(D)), bs);
    mat h = -bs.t()*zs/2 - c;

    z = Q*zs;

    a = sqrt(h(0)/D(0));
    b = sqrt(h(0)/D(1));
    alpha = atan2(Q(1, 0), Q(0, 0));
    }
