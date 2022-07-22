#pragma once

#include "ILS.h"

bool comparison(const InsertionInfo& a, const InsertionInfo& b){
    return a.cost < b.cost;
}

Solucao Construcao(Data& d){
    Solucao s;
    vector<int> V;
    vector<int> CL;

    for (int i = 1; i <= d.vertices; i++){
        V.push_back(i);
    }

    CL = V;
    random_shuffle(CL.begin(), CL.end());

    for(int i = 0; i < d.vertices/2; i++){
        s.sequence.push_back(CL[i]);
        CL.erase(remove(CL.begin(), CL.end(), CL[i]), CL.end());
    }
    
    int chosen;
    while(!CL.empty()){
        vector<InsertionInfo> insertioncost ((s.sequence.size() - 1) * CL.size());
        int i, l = 0;

        for(int a = 0, b = 1; a < s.sequence.size() - 1; a++, b++){
            int i = s.sequence[a];
            int j = s.sequence[b];

            for (int k = 0; k < CL.size(); k++){
                insertioncost[l].cost = d.matrizAdj[i-1][k] + d.matrizAdj[j-1][k] - d.matrizAdj[i-1][j-1];
                insertioncost[l].inserir = k;
                insertioncost[l].local = a;
                l++;
            }
        }
        sort(insertioncost.begin(), insertioncost.end(), comparison);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * insertioncost.size()));
        chosen = CL[insertioncost[selecionado].inserir];
        s.sequence.insert(s.sequence.begin() + insertioncost[selecionado].local, chosen);
        CL.erase(remove(CL.begin(), CL.end(), chosen), CL.end());
    }
    int last_vertice = s.sequence[0];
    s.sequence.push_back(last_vertice);
    return s;
}
