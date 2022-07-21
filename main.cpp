#include "ILS.cpp"
#include "construcao.cpp"
#include "busca_local.cpp"
#include "pertubacao.cpp"

int main(){

    Data d = {{
        {0, 2, 1, 4, 9, 1},
        {2, 0, 5, 9, 7, 2},
        {1, 5, 0, 3, 8, 6},
        {4, 9, 3, 0, 2, 5},
        {9, 7, 8, 2, 0, 2},
        {1, 2 ,6, 5, 2, 0}
    }, 6};

    Solucao s1 = solve(s1, d, 50, 6);
    exibirSolucao(s1, d);
}
