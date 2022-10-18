#include "ILS.h"
#include "readData.h"

int main(int argc, char *argv[]){

    double **matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);
    
    Data d = {matrizAdj, dimension};
    ILS solution(d);

    solution.solve();
    solution.show_solution();

    return 0;
}
