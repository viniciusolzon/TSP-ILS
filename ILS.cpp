#pragma once

#include "ILS.h"

double epsilon(double a, double b){
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool improve(double value_1, double value_2){
    return (value_1 - value_2) > epsilon(value_1, value_2);
}

void calcularcost(Solucao& s, Data& d){
    s.cost = 0.0;
    for(int i = 0; i < s.sequence.size() - 1; i++){
        s.cost += d.matrizAdj[s.sequence[i]][s.sequence[i+1]];
    }
}

void exibirSolucao(Solucao& s, Data& d){
    cout << "Sequence:  ";
    for(int i = 0; i < s.sequence.size() - 1; i++){
        cout << s.sequence[i] << " -> ";
    }
    cout << s.sequence.back() << endl;
    cout << "Cost:  " << setprecision(2) << fixed << s.cost << "\n";
}

Solucao solve(Solucao& s, Data& d, int maxIter, int maxIterIls){
    auto start = std::chrono::high_resolution_clock::now();
    srand(time(NULL));
    Solucao bestOfAll;
    for(int i = 0; i < maxIter; i++){
        Solucao s = Construcao(d);
        calcularcost(s, d);
        Solucao best = s;
        if(i == 0){
            bestOfAll = s;
        }
        int iterIls = 0;
        while(iterIls <= maxIterIls){
            BuscaLocal(s, d);
            if(improve(best.cost, s.cost)){
                best = s;
                iterIls = 0;
            }
            s = Pertubacao(best, d);
            iterIls++;
        }
        if(improve(bestOfAll.cost, best.cost)){
            bestOfAll.sequence = best.sequence;
            bestOfAll.cost = best.cost;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> float_ms = end - start;
    cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";
    return bestOfAll;
}
