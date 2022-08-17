#pragma once

#include "ILS.h"

bool comparison(const InsertionInfo& a, const InsertionInfo& b){
    return a.cost < b.cost;
}

Solucao Construcao(Data& d){
    Solucao s;
    vector<int> CL;

    for (int i = 2; i <= d.vertices; i++){
        CL.push_back(i);
    }

    s.sequence = {1};

    random_shuffle(CL.begin(), CL.end());
    for(int i = 0; i < 3; i++){
        s.sequence.push_back(CL[i]);
        CL.erase(CL.begin() + i);
    }

    s.sequence.push_back(1);

    int chosen;
    while(!CL.empty()){
        vector<InsertionInfo> insertioncost ((s.sequence.size() - 1) * CL.size());
        
        for(int a = 0, b = 1, l = 0; a < s.sequence.size() - 1; a++, b++){
            int i = s.sequence[a];
            int j = s.sequence[b];

            for (int k = 0; k < CL.size(); k++){
                insertioncost[l].cost = d.matrizAdj[i][CL[k]] + d.matrizAdj[j][CL[k]] - d.matrizAdj[i][j];
                insertioncost[l].inserir = k;
                insertioncost[l].remover = a;
                l++;
            }
        }
        sort(insertioncost.begin(), insertioncost.end(), comparison);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * insertioncost.size()));
        selecionado = selecionado != 0 ? rand() % (selecionado) : 0;
        chosen = CL[insertioncost[selecionado].inserir];
        s.sequence.insert(s.sequence.begin() + insertioncost[selecionado].remover + 1, chosen);
        CL.erase(CL.begin() + insertioncost[selecionado].inserir);
    }
    return s;
}
