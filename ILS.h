#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

struct Data{
    double** matrizAdj;
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

double epsilon(double a, double b);

bool improve(double value_1, double value_2);

void calcularcost(Solucao& s, Data& d);

bool comparison(const InsertionInfo& a, const InsertionInfo& b);
Solucao Construcao(Data& d);

double calculateSwapCost(Solucao& s, Data& d, int first, int second);
bool bestImprovementSwap(Solucao &s, Data& d);
double calculate2OptCost(Solucao& s, Data& d, int first, int second);
bool bestImprovement2Opt(Solucao &s, Data &d);
double calculateOrOptCost(Solucao& s, Data& d, int first, int second, int amount);        
bool bestImprovementOrOpt(Solucao &s, Data &d, int amount);
void BuscaLocal(Solucao& s, Data& d);

vector<int> Pertubacao(Solucao& s);

Solucao solve(Solucao& s, Data& d, int maxIter, int maxIterIls);
void exibirSolucao(Solucao& s, Data& d);

// struct ILS{
//         void calcularcost(Solucao& s, Data& d);

//         bool comparison(const InsertionInfo& a, const InsertionInfo& b);
//         Solucao Construcao(Data& d);
        
//         double calculateSwapCost(Solucao& s, Data& d, int first, int second);
//         bool bestImprovementSwap(Solucao &s, Data& d);
//         double calculate2OptCost(Solucao& s, Data& d, int first, int second);
//         bool bestImprovement2Opt(Solucao &s, Data &d);
//         double calculateOrOptCost(Solucao& s, Data& d, int first, int second, int amount);        
//         bool bestImprovementOrOpt(Solucao &s, Data &d, int amount);
//         void BuscaLocal(Solucao& s, Data& d);
        
//         vector<int> Pertubacao(Solucao& s);
    
//         Solucao solve(Solucao& s, Data& d, int maxIter, int maxIterIls);
//         void exibirSolucao(Solucao& s, Data& d);
// };
