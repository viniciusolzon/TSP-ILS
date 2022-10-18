#include "ILS.h"

// Solucao ILS :: Construcao(Data *d){
//     Solucao s;
//     vector<int> CL;

//     for (int i = 2; i <= d->vertices; i++){
//         CL.push_back(i);
//     }

//     s.sequence = {1};

//     random_shuffle(CL.begin(), CL.end());
//     for(int i = 0; i < 3; i++){
//         s.sequence.push_back(CL[i]);
//         CL.erase(CL.begin() + i);
//     }

//     s.sequence.push_back(1);

//     int chosen;
//     while(!CL.empty()){
//         vector<InsertionInfo> insertioncost ((s.sequence.size() - 1) * CL.size());
        
//         for(int a = 0, b = 1, l = 0; a < s.sequence.size() - 1; a++, b++){
//             int i = s.sequence[a];
//             int j = s.sequence[b];

//             for (int k = 0; k < CL.size(); k++){
//                 insertioncost[l].cost = d->matrizAdj[i][CL[k]] + d->matrizAdj[j][CL[k]] - d->matrizAdj[i][j];
//                 insertioncost[l].inserir = k;
//                 insertioncost[l].remover = a;
//                 l++;
//             }
//         }
//         sort(insertioncost.begin(), insertioncost.end(), compare);
//         double alpha = (double) rand() / RAND_MAX;
//         int selecionado = rand() % ((int) ceil(alpha * insertioncost.size()));
//         selecionado = selecionado != 0 ? rand() % (selecionado) : 0;
//         chosen = CL[insertioncost[selecionado].inserir];
//         s.sequence.insert(s.sequence.begin() + insertioncost[selecionado].remover + 1, chosen);
//         CL.erase(CL.begin() + insertioncost[selecionado].inserir);
//     }

//     return s;
// }

Solucao ILS :: Construcao(Data *d){
    Solucao current;
    vector<int> CL;

    for (int i = 2; i <= d->vertices; i++){
        CL.push_back(i);
    }

    current.sequence = {1};

    random_shuffle(CL.begin(), CL.end());
    for(int i = 0; i < 2; i++){
        current.sequence.push_back(CL[i]);
        CL.erase(remove(CL.begin(), CL.end(), CL[i]), CL.end());
    }

    current.sequence.push_back(1);
     
    while(!CL.empty()){
        vector<InsertionInfo> insertioncost;
        insertioncost = calcularCustoInsercao(&current, d, CL);

        sort(insertioncost.begin(), insertioncost.end(), compare);

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * insertioncost.size()));

        current.sequence.insert(current.sequence.begin() + insertioncost[selecionado].remover + 1, insertioncost[selecionado].inserir);

        CL.erase(remove(CL.begin(), CL.end(), insertioncost[selecionado].inserir), CL.end());
    }
    // calcularcost(&current, d);

    return current;
}

vector<InsertionInfo> ILS :: calcularCustoInsercao(Solucao *s, Data *d, vector<int> CL){
    InsertionInfo info;
    vector<InsertionInfo> custoInsercao;

    int l = 0;
    for(int a = 0, b = 1; l < s->sequence.size() - 1; a++, b++){
        int i = s->sequence[a];
        int j = s->sequence[b];
        
        for(auto k : CL){
            info.inserir = k;
            info.remover = a;
            info.cost = d->matrizAdj[i][k] + d->matrizAdj[j][k] - d->matrizAdj[i][j];

            custoInsercao.push_back(info);
        }
        l++;
    }

    return custoInsercao;
}
