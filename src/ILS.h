#ifndef ILS_H
#define ILS_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::setprecision;
using std::fixed;
using std::numeric_limits;

struct Data{
    double **matrizAdj;
    int vertices;
};

struct Solucao{
    vector<int> sequence;
    double cost;
};

struct InsertionInfo{
    int inserir;
    int remover;
    double cost = 0.0;
};

class ILS{
    int maxIter;
    int maxIterILS;
    Data dist;
    Solucao best_solution;

public:
    ILS(Data d); // CONSTRUTOR
    ~ILS(); // DESCONSTRUTOR

    double epsilon(double a, double b);
    bool improve(double value_1, double value_2);
    void calcularcost(Solucao *s, Data *d);

    Solucao Construcao(Data *d);
    vector<InsertionInfo> calcularCustoInsercao(Solucao *s, Data *d, vector<int> CL);

    void BuscaLocal(Solucao *s, Data *d);
    double calculateSwapCost(Solucao *s, Data *d, int first, int second);
    bool bestImprovementSwap(Solucao *s, Data *d);
    double calculate2OptCost(Solucao *s, Data *d, int first, int second);
    bool bestImprovement2Opt(Solucao *s, Data *d);
    double calculateOrOptCost(Solucao *s, Data *d, int first, int second, int amount);        
    bool bestImprovementOrOpt(Solucao *s, Data *d, int amount);

    Solucao Pertubacao(Solucao *s, Data *d);

    void solve();
    void show_solution();
};

bool compare(const InsertionInfo &a, const InsertionInfo &b);

#endif
