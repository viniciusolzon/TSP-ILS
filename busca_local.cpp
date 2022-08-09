#include "ILS.h"

double calculateSwapCost(Solucao& s, Data& d, int first, int second){
    double origin = 0.0, after = 0.0, cost = 0.0;
    if(second == first + 1){
        origin = d.matrizAdj[s.sequence[first - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[second]][s.sequence[first]] + d.matrizAdj[s.sequence[second]][s.sequence[second + 1]];
        after = d.matrizAdj[s.sequence[first - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[second]][s.sequence[first]] + d.matrizAdj[s.sequence[first]][s.sequence[second + 1]];
        cost = after - origin;
    }
    else{
        origin = d.matrizAdj[s.sequence[first - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[first + 1]][s.sequence[first]] + d.matrizAdj[s.sequence[second - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[second + 1]][s.sequence[second]];
        after = d.matrizAdj[s.sequence[first - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[first + 1]][s.sequence[second]] + d.matrizAdj[s.sequence[second - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[second + 1]][s.sequence[first]];
        cost = after - origin;
    }

    return cost;
}

bool bestImprovementSwap(Solucao &s, Data& d){
    double bestcost = 0.0;
    double cost = 0.0;
    int best_i, best_j;
    for(int i = 1; i < s.sequence.size() - 2; i++){
        for(int j = i + 1; j < s.sequence.size() - 1; j++){
            cost = calculateSwapCost(s, d, i, j);
            if(improve(bestcost, cost)){
                bestcost = cost;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(bestcost < 0){
        int aux = s.sequence[best_i];
        s.sequence[best_i] = s.sequence[best_j];
        s.sequence[best_j] = aux;
        s.cost += bestcost;
        return true;
    }
return false;
}

double calculate2OptCost(Solucao& s, Data& d, int first, int second){
    double origin = 0.0, after = 0.0, cost = 0.0;
    origin = d.matrizAdj[s.sequence[first - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[second]][s.sequence[second + 1]];
    after = d.matrizAdj[s.sequence[first - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[first]][s.sequence[second + 1]];
    cost = after - origin;

    return cost;
}

bool bestImprovement2Opt(Solucao &s, Data &d){
    double bestcost = 0.0, cost = 0.0;
    int best_i, best_j;             

    for(int i = 1; i < s.sequence.size() - 2; i++){
        for(int j = i + 1; j < s.sequence.size() - 1; j++){
            cost = calculate2OptCost(s, d, i, j);
            if (cost < bestcost){
                best_i = i;
                best_j = j;
                bestcost = cost;
            }
        }
    }
    if(bestcost < 0){
        reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);

        s.cost+=bestcost;
        return true;
    }

    return false;
}

double calculateOrOptCost(Solucao& s, Data& d, int first, int second, int amount){
    double origin = 0.0, after = 0.0, cost = 0.0;
    if(first > second){
        origin = d.matrizAdj[s.sequence[second - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[first - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[first + amount - 1]][s.sequence[first + amount]];
        after = d.matrizAdj[s.sequence[second - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[first + amount - 1]][s.sequence[second]] + d.matrizAdj[s.sequence[first - 1]][s.sequence[first + amount]];
    }
    else{
        origin = d.matrizAdj[s.sequence[first - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[first + amount - 1]][s.sequence[first + amount]] + d.matrizAdj[s.sequence[second + amount - 1]][s.sequence[second + amount]];
        after = d.matrizAdj[s.sequence[first - 1]][s.sequence[first + amount]] + d.matrizAdj[s.sequence[second + amount - 1]][s.sequence[first]] + d.matrizAdj[s.sequence[first + amount - 1]][s.sequence[second + amount]];
    }
    cost = after - origin;

    return cost;
}

bool bestImprovementOrOpt(Solucao &s, Data &d, int amount){
    double bestcost = 0.0, cost = 0.0;
    int best_i, best_j;
    for(int i = 1; i < s.sequence.size() - amount; i++){
        for(int j = 1; j < s.sequence.size() - amount; j++){
            if(i == j){
                cost = 0.0;
            }
            else{
                cost = calculateOrOptCost(s, d, i, j, amount);
            }
            if(improve(bestcost, cost)){
                best_i = i;
                best_j = j;
                bestcost = cost;
            }
        }
    }

    if(bestcost < 0){
        vector<int> bloco(s.sequence.begin() + best_i, s.sequence.begin() + best_i  + amount);
        s.sequence.erase(s.sequence.begin() + best_i, s.sequence.begin() + best_i + amount);
        s.sequence.insert(s.sequence.begin() + best_j, bloco.begin(), bloco.end());

        s.cost+=bestcost;
        return true;
    }

    return false;
}

void BuscaLocal(Solucao& s, Data& d){
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    while(NL.empty() == false){
        int n = rand() % NL.size();
        switch(NL[n]){
            case 1:
                improved = bestImprovementSwap(s, d);
                break;
            case 2:
                improved = bestImprovement2Opt(s, d);
                break;
            case 3:
                improved = bestImprovementOrOpt(s, d, 1); // Reinsertion
                break;
            case 4:
                improved = bestImprovementOrOpt(s, d, 2); // Reinsertion: 2 vertices
                break;
            case 5:
                improved = bestImprovementOrOpt(s, d, 3); // Reinsertion: 3 vertices
                break;
        }
        if(improved)
            NL = {1, 2, 3, 4, 5};
        else
            NL.erase(NL.begin() + n);
    }
}
