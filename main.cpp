#include "ILS.cpp"
#include "construcao.cpp"
#include "busca_local.cpp"
#include "pertubacao.cpp"
#include "readData.h"

int main(int argc, char* argv[]){

    double ** matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);

    Data d = {matrizAdj, dimension};

    Solucao s1 = solve(s1, d, 50, 14);
    exibirSolucao(s1, d);
}
