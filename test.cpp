#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <algorithm>
#include <cmath>
// #include <limits>    Precisa mesmo?
#include <chrono>

using namespace std;

struct Data{
    vector<vector<double>> matrizAdj;
    int vertices;
};

struct Solucao{
    vector<int> sequence = {};
    double cost = 0.0;
};

struct InsertionInfo{
    int inserir = 0;
    int remover = 0;
    double cost = 0.0;
};

bool comparison(const InsertionInfo& a, const InsertionInfo& b){
    return a.cost < b.cost;
}

double epsilon(double a, double b) {
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool improve(double value_1, double value_2) {
    return (value_1 - value_2) > epsilon(value_1, value_2);
}

void calcularcost(Solucao& s, Data& d){
    s.cost = 0.0;
    for(int i = 0; i < s.sequence.size() - 1; i++){
        s.cost += d.matrizAdj[s.sequence[i]-1][s.sequence[i+1]-1];
    }
}

void exibirSolucao(Solucao& s, Data& d){
    cout << "Sequence:  ";
    for(int i = 0; i < s.sequence.size() - 1; i++){
        cout << s.sequence[i] << " -> ";
    }
    cout << s.sequence.back() << "\n";
    cout << "Cost:  " << setprecision(2) << fixed << s.cost << "\n";
}

Solucao Construcao(Data& d){
    Solucao s;
    vector<int> CL;

    for (int i = 1; i <= d.vertices; i++){
        CL.push_back(i);
    }

    random_shuffle(CL.begin(), CL.end());
    for(int i = 0; i < 3; i++){
        s.sequence.push_back(CL[i]);
        CL.erase(CL.begin() + i);
    }
    s.sequence.push_back(s.sequence[0]);
    
    int chosen;
    while(!CL.empty()){
        vector<InsertionInfo> insertioncost ((s.sequence.size() - 1) * CL.size());
        
        for(int a = 0, b = 1, l = 0; a < s.sequence.size() - 1; a++, b++){
            int i = s.sequence[a];
            int j = s.sequence[b];

            for (int k = 0; k < CL.size(); k++){
                insertioncost[l].cost = d.matrizAdj[i -1][CL[k] - 1] + d.matrizAdj[j - 1][CL[k] - 1] - d.matrizAdj[i - 1][j - 1];
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

double calculateSwapCost(Solucao& s, Data& d, int first, int second){
    double origin = 0.0, after = 0.0, cost = 0.0;
    if(second == first + 1){
        origin = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[second] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[second] - 1][s.sequence[second + 1] - 1];
        after = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[second] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first] - 1][s.sequence[second + 1] - 1];
        cost = after - origin;
    }
    else{
        origin = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first + 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[second - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[second + 1] - 1][s.sequence[second] - 1];
        after = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[first + 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[second - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[second + 1] - 1][s.sequence[first] - 1];
        cost = after - origin;
    }
    return cost;
}

bool bestImprovementSwap(Solucao &s, Data& d){
    double bestcost = 0.0;
    double cost = 0.0;
    int best_i = 0, best_j = 0;
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
    origin = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[second] - 1][s.sequence[second + 1] - 1];
    after = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[first] - 1][s.sequence[second + 1] - 1];
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
        origin = d.matrizAdj[s.sequence[second - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first + amount - 1] - 1][s.sequence[first + amount] - 1];
        after = d.matrizAdj[s.sequence[second - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first + amount - 1] - 1][s.sequence[second] - 1] + d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first + amount] - 1];
    }
    else{
        origin = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first + amount - 1] - 1][s.sequence[first + amount] - 1] + d.matrizAdj[s.sequence[second + amount - 1] - 1][s.sequence[second + amount] - 1];
        after = d.matrizAdj[s.sequence[first - 1] - 1][s.sequence[first + amount] - 1] + d.matrizAdj[s.sequence[second + amount - 1] - 1][s.sequence[first] - 1] + d.matrizAdj[s.sequence[first + amount - 1] - 1][s.sequence[second + amount] - 1];
    }
    cost = after - origin;

    return cost;
}

bool bestImprovementOrOpt(Solucao &s, Data &d, int amount){
    int count = 0;
    double bestcost = 0.0, cost = 0.0;
    int best_i = 0, best_j = 0;
    for (int i = 1; i < s.sequence.size() - amount; i++){
        for (int j = 1; j < s.sequence.size() - amount; j++){
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
    while (!NL.empty()){
        int n = rand() % NL.size();
        switch(NL[n]){
            case 1:
                improved = bestImprovementSwap(s, d);
                break;
            case 2:
                improved = bestImprovement2Opt(s, d);
                break;
            case 3:
                improved = bestImprovementOrOpt(s, d, 1);
                break;
            case 4:
                improved = bestImprovementOrOpt(s, d, 2);
                break;
            case 5:
                improved = bestImprovementOrOpt(s, d, 3);
                break;
        }
        if(improved)
            NL = {1, 2, 3, 4, 5};
        else
            NL.erase(NL.begin() + n);
    }
}

Solucao Pertubacao(Solucao& s, Data& d){
    Solucao s_copy;
    s_copy.sequence = s.sequence;
    s_copy.cost = s.cost;

    int lim = ceil(s_copy.sequence.size() / 10.0);

    double origin = 0.0, after = 0.0, cost = 0.0;
    origin = d.matrizAdj[s_copy.sequence[s_copy.sequence.size() - 2]][s_copy.sequence[s_copy.sequence.size() - 1]];

    s_copy.sequence.pop_back();

    int size1, start1, end1, size2, start2, end2;
    size1 = max(2, rand() % lim);
    start1 = rand() % (s_copy.sequence.size() - size1 - 1);
    end1 = start1 + size1 - 1;
    size2 = max(2, rand() % lim);

    if(s_copy.sequence.size() - 1 - end1 > size2 && start1 > size2){
        int coin = rand() % 2;
        if(coin == 0){
            int min = end1 + 2;
            int max = s_copy.sequence.size() - size2;
            start2 = rand()%(max - min + 1) + min;
            end2 = start2 + size2 - 1;
        }
        else{
            int max = start1 - size2 - 1;
            start2 = rand()% (max + 1);
            end2 = start2 + size2 - 1;
        }
    }
    else if(s_copy.sequence.size() - 1 - end1 > size2){
            int min = end1 + 2;
            int max = s_copy.sequence.size() - size2;
            start2 = rand()%(max - min + 1) + min;
            end2 = start2 + size2 - 1;
            
    }   else{
            int max = start1 - size2 - 1;
            start2 = rand()%(max + 1);
            end2 = start2 + size2 - 1;
    }

    vector<int> segment1(s_copy.sequence.begin() + start1, s_copy.sequence.begin() + start1 + size1);
    vector<int> segment2(s_copy.sequence.begin() + start2, s_copy.sequence.begin() + start2 + size2);

    if(end1 == s_copy.sequence.size() - 1){
        origin += d.matrizAdj[s_copy.sequence[start1 - 1]][s_copy.sequence[start1]];
        if(start2 == 0){
            origin += d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end2 + 1]];
            after = d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end2 + 1]] + d.matrizAdj[s_copy.sequence[start2]][s_copy.sequence[start1 - 1]];
        }
        else{
            origin += d.matrizAdj[s_copy.sequence[start2 - 1]][s_copy.sequence[start2]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end2 + 1]];
            after = d.matrizAdj[s_copy.sequence[start2]][s_copy.sequence[start1 - 1]] + d.matrizAdj[s_copy.sequence[start1]][s_copy.sequence[start2 - 1]] + d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end2 + 1]];
        }
    }
    else if(start1 == 0){
            origin += d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end1 + 1]];
            if(end2 == s_copy.sequence.size() - 1){
                origin += d.matrizAdj[s_copy.sequence[start2 - 1]][s_copy.sequence[start2]];
                after = d.matrizAdj[s_copy.sequence[start1]][s_copy.sequence[start2 - 1]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end1 + 1]];
            }
            else{
                origin += d.matrizAdj[s_copy.sequence[start2 - 1]][s_copy.sequence[start2]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end2 + 1]];
                after = d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end1 + 1]] + d.matrizAdj[s_copy.sequence[start1]][s_copy.sequence[start2 - 1]] + d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end2 + 1]];
            }
    }   else{
        origin += d.matrizAdj[s_copy.sequence[start1 - 1]][s_copy.sequence[start1]] + d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end1 + 1]];
        if(end2 == s_copy.sequence.size() - 1){
            origin += d.matrizAdj[s_copy.sequence[start2 - 1]][s_copy.sequence[start2]];
            after = d.matrizAdj[s_copy.sequence[start1]][s_copy.sequence[start2 - 1]] + d.matrizAdj[s_copy.sequence[start2]][s_copy.sequence[start1 - 1]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end1 + 1]];
        }
        else if(start2 == 0){
                origin += d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end2 + 1]];
                after = d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end2 + 1]] + d.matrizAdj[s_copy.sequence[start2]][s_copy.sequence[start1 - 1]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end1 + 1]];
        }
            else{
                origin += d.matrizAdj[s_copy.sequence[start2 - 1]][s_copy.sequence[start2]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end2 + 1]];
                after = d.matrizAdj[s_copy.sequence[start1]][s_copy.sequence[start2 - 1]] + d.matrizAdj[s_copy.sequence[end1]][s_copy.sequence[end2 + 1]] + d.matrizAdj[s_copy.sequence[start2]][s_copy.sequence[start1 - 1]] + d.matrizAdj[s_copy.sequence[end2]][s_copy.sequence[end1 + 1]];
            }
    }

    if(start1 > start2){
        s_copy.sequence.erase(s_copy.sequence.begin() + start1, s_copy.sequence.begin() + start1 + size1);
        s_copy.sequence.insert(s_copy.sequence.begin() + start1, segment2.begin(), segment2.end());
        s_copy.sequence.erase(s_copy.sequence.begin() + start2, s_copy.sequence.begin() + start2 + size2);
        s_copy.sequence.insert(s_copy.sequence.begin() + start2, segment1.begin(), segment1.end());
    }
    else{
        s_copy.sequence.erase(s_copy.sequence.begin() + start2, s_copy.sequence.begin() + start2 + size2);
        s_copy.sequence.insert(s_copy.sequence.begin() + start2, segment1.begin(), segment1.end());
        s_copy.sequence.erase(s_copy.sequence.begin() + start1, s_copy.sequence.begin() + start1 + size1);
        s_copy.sequence.insert(s_copy.sequence.begin() + start1, segment2.begin(), segment2.end());
    }

    s_copy.sequence.push_back(s_copy.sequence[0]);

    after += d.matrizAdj[s_copy.sequence[s_copy.sequence.size() - 2]][s_copy.sequence[s_copy.sequence.size() - 1]];
    cost = after - origin;
    s_copy.cost += cost;

    return s_copy;
}

Solucao solve(Solucao& s, Data& d, int maxIter, int maxIterIls){
    auto start = std::chrono::high_resolution_clock::now();
    srand(time(NULL));
    Solucao bestOfAll;
    for(int i = 0; i < maxIter; i++){
        Solucao s = Construcao(d);
        calcularcost(s, d);
        Solucao best = s;
        if (i == 0){
            bestOfAll.sequence = s.sequence;
            bestOfAll.cost = s.cost;
        }
        int iterIls = 0;
        cout << "Iteração:      " << i + 1 << "\n"; // debug
        while(iterIls <= maxIterIls){
            BuscaLocal(s, d);
            if(improve(best.cost, s.cost)){
                best.sequence = s.sequence;
                best.cost = s.cost;
                iterIls = 0;
            }
            s = Pertubacao(best, d);
            // if(s.cost < 0){
            //     exibirSolucao(s, d);
            //     break;
            // }
            iterIls++;
        }
        if (improve(bestOfAll.cost, best.cost)){
            bestOfAll.sequence = best.sequence;
            bestOfAll.cost = best.cost;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> float_ms = end - start;
    cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";
    cout << "Sequence size:  " << bestOfAll.sequence.size() - 1 << "\n";
    return bestOfAll;
}

int main(){
    srand(time(NULL));
    
    Data d = {{
        {0.0, 2.0, 1.0, 4.0, 9.0, 1.0, 3.0, 4.0, 8.0, 7.0, 5.0, 1.0},
        {2.0, 0.0, 5.0, 9.0, 7.0, 2.0, 6.0, 1.0, 3.0, 1.0, 2.0, 2.0},
        {1.0, 5.0, 0.0, 3.0, 8.0, 6.0, 2.0, 8.0, 9.0, 6.0, 4.0, 3.0},
        {4.0, 9.0, 3.0, 0.0, 2.0, 5.0, 7.0, 9.0, 2.0, 3.0, 6.0, 1.0},
        {9.0, 7.0, 8.0, 2.0, 0.0, 2.0, 5.0, 1.0, 5.0, 4.0, 8.0, 9.0},
        {1.0, 2.0 ,6.0, 5.0, 2.0, 0.0, 3.0, 6.0, 2.0, 4.0, 8.0, 1.0},
        {3.0, 6.0, 2.0, 7.0, 5.0, 3.0, 0.0, 5.0, 1.0, 2.0, 4.0, 3.0},
        {4.0, 1.0, 8.0, 9.0, 1.0, 6.0, 5.0, 0.0, 4.0, 5.0, 7.0, 3.0},
        {8.0, 3.0, 9.0, 2.0, 5.0, 2.0, 1.0, 4.0, 0.0, 6.0, 3.0, 8.0},
        {7.0, 1.0, 6.0, 3.0, 4.0, 4.0, 2.0, 5.0, 6.0, 0.0, 5.0, 1.0},
        {5.0, 2.0, 4.0, 6.0, 8.0, 8.0, 4.0, 7.0, 3.0, 5.0, 0.0, 2.0},
        {1.0, 2.0, 3.0, 1.0, 9.0, 1.0, 3.0, 3.0, 8.0, 1.0, 2.0, 0.0}
    }, 12};
    
    Solucao s = solve(s, d, 50, 12);
    exibirSolucao(s, d);
    // for(int i = 0; i < 10; i++)
    // cout << rand()%(10 - 1 + 1) + 1 << endl;

}
