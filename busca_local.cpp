#include "ILS.h"

double ILS :: calculateSwapCost(Solucao& s, Data& d, int first, int second){
    vector<int> s_copy = s.sequence;
    double origin, after, cost = 0;

    for(int i = 0; i < s.sequence.size() - 1; i++){
        origin += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    swap(s_copy[first], s_copy[second]);

    for(int i = 0; i < s.sequence.size() - 1; i++){
        after += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    cost = after - origin;
    return cost;
}

bool ILS :: bestImprovementSwap(Solucao &s, Data& d){
    double bestcost = 0;
    double cost;
    int best_i, best_j;
    for(int i = 1; i < s.sequence.size() - 1; i++){
        for(int j = i + 1; j < s.sequence.size() - 1; j++){
            cost = calculateSwapCost(s, d, i, j);
            if (cost < bestcost){
                bestcost = cost;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(bestcost < 0){
        swap(s.sequence[best_i], s.sequence[best_j]);
        s.cost = s.cost - cost;
        return true;
    }
return false;
}

double ILS :: calculate2OptCost(Solucao& s, Data& d, int first, int second){
    vector<int> s_copy = s.sequence;
    double origin, after, cost = 0;

    for(int i = 0; i < s.sequence.size() - 1; i++){
        origin += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    reverse(s_copy.begin() + first, s_copy.begin() + second + 1);

    for(int i = 0; i < s.sequence.size() - 1; i++){
        after += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    cost = after - origin;
    return cost;
}

bool ILS :: bestImprovement2Opt(Solucao &s, Data &d){
    double bestcost = 0.0;
    int best_i, best_j;             

    for (int i = 1; i < s.sequence.size() - 1; i++){
        for (int j = i + 1; j < s.sequence.size() - 1; j++){
            double cost = calculate2OptCost(s, d, i, j);
            if (cost < bestcost){
                best_i = i;
                best_j = j;
                bestcost = cost;
            }
        }
    }
    if (bestcost < 0){
        reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
        return true;
    }

    return false;
}

double ILS :: calculateOrOptCost(Solucao& s, Data& d, int first, int second, int amount){
    vector<int> s_copy = s.sequence;
    vector<int> collection(s_copy.begin() + first, s_copy.begin() + amount + first);
    double origin, after, cost = 0;

    for(int i = 0; i < s.sequence.size() - 1; i++){
        origin += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    s_copy.erase(s_copy.begin() + first, s_copy.begin() + amount + first);
    s_copy.insert(s_copy.begin() + second, collection.begin(), collection.end());

    for(int i = 0; i < s.sequence.size() - 1; i++){
        after += d.matrizAdj[s_copy[i]-1][s_copy[i+1]-1];
    }

    cost = after - origin;
    return cost;
}

bool ILS :: bestImprovementOrOpt(Solucao &s, Data &d, int amount){
    double bestcost = 0.0;
    int best_i, best_j;             

    for (int i = 1; i < s.sequence.size() - amount; i++){
        for (int j = i + 1; j < s.sequence.size() - amount; j++){
            double cost = calculateOrOptCost(s, d, i, j, amount);
           

            if (cost < bestcost){
                best_i = i;
                best_j = j;
                bestcost = cost;
            }
        }
    }

    if (bestcost < 0){
        vector<int> collection(s.sequence.begin() + best_i, s.sequence.begin() + amount + best_i);
        s.sequence.erase(s.sequence.begin() + best_i, s.sequence.begin() + amount + best_i);
        s.sequence.insert(s.sequence.begin() + best_j, collection.begin(), collection.end());
        return true;
    }

    return false;
}


void ILS :: BuscaLocal(Solucao& s, Data& d){

    std::vector<int> NL = {1, 2, 3, 4, 5};

    bool improved = false;

    while (NL.empty() == false){
        int n = rand() % NL.size();
        switch (NL[n]){
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
