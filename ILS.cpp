#pragma once

#include "ILS.h"

using namespace std;

#define INFINITY 999999999;

void ILS :: calcularcost(Solucao& s, Data& d){
    s.cost = 0;
    for(int i = 0; i < s.sequence.size() - 1; i++){
        s.cost += d.matrizAdj[s.sequence[i]-1][s.sequence[i+1]-1];
    }
}

void ILS :: exibirSolucao(Solucao& s, Data& d){
    cout << "Sequence:  ";
    for(int i = 0; i < s.sequence.size() - 1; i++){
        cout << s.sequence[i] << " -> ";
    }
    cout << s.sequence.back() << endl;
    calcularcost(s, d);
    cout << "Cost:  " << s.cost << endl;
    
}

Solucao ILS :: solve(Solucao& s, Data& d, int maxIter, int maxIterIls){
    Solucao bestOfAll;
    bestOfAll.cost = INFINITY;
    for(int i = 0; i < maxIter; i++){
        Solucao s = Construcao(d);
        Solucao best = s;
        int iterIls = 0;
        while(iterIls <= maxIterIls){
            BuscaLocal(s, d);
            if(s.cost < best.cost){
                best = s;
                iterIls = 0;
            }
            // s = Perturbacao(&best);
            iterIls++;
        }
        if (best.cost < bestOfAll.cost){
            bestOfAll = best;
        }
    }
    return bestOfAll;
}
