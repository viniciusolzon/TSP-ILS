#pragma once

#include "ILS.h"

void calcularcost(Solucao& s, Data& d){
    s.cost = 0;
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
    cout << "Cost:  " << s.cost << endl;
}

Solucao solve(Solucao& s, Data& d, int maxIter, int maxIterIls){
    srand(time(NULL));
    Solucao bestOfAll;
    bestOfAll.cost = INFINITY;
    for(int i = 0; i < maxIter; i++){
        Solucao s = Construcao(d);
        calcularcost(s, d);
        Solucao best = s;
        int iterIls = 0;
        cout << "Iteração:      " << i + 1 << std:: endl; // debug
        while(iterIls <= maxIterIls){
            BuscaLocal(s, d);
            calcularcost(s, d);
            if(s.cost < best.cost){
                best = s;
                iterIls = 0;
            }
            s.sequence = Pertubacao(best);
            calcularcost(s, d);
            iterIls++;
        }
        if (best.cost < bestOfAll.cost){
            bestOfAll = best;
        }
    }
    return bestOfAll;
}
