#include "ILS.cpp"
#include "construcao.cpp"
#include "busca_local.cpp"
#include "perturbacao.cpp"
#include "readData.h"

int main(int argc, char* argv[]){

    double ** matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);

    Data d = {matrizAdj, dimension};
    Solucao s = solve(s, d, 50, d.vertices >= 150 ? d.vertices / 2 : d.vertices);
    exibirSolucao(s, d);
}
