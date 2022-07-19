#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace std;

struct Data{
    vector<vector<int>> matrizAdj;
    int vertices;
};

struct Solucao{
    vector<int> sequence;
    double cost;
};

struct InsertionInfo{
    int inserir;
    int local;
    double cost;
};

bool comparison(const InsertionInfo& a, const InsertionInfo& b){
    return a.cost < b.cost;
}

void calcularcost(Solucao& s, Data& d){
    s.cost = 0;
    for(int i = 0; i < s.sequence.size() - 1; i++){
        s.cost += d.matrizAdj[s.sequence[i]-1][s.sequence[i+1]-1];
    }
}

void exibirSolucao(Solucao& s, Data& d){
    cout << "Sequence:  ";
    for(int i = 0; i < s.sequence.size() - 1; i++){
        cout << s.sequence[i] << " -> ";
    }
    cout << s.sequence.back() << endl;
    calcularcost(s, d);
    cout << "Cost:  " << s.cost << endl;
}

Solucao Construcao(Data& d){
    Solucao s;
    vector<int> V;
    vector<int> CL;

    for (int i = 1; i <= d.vertices; i++){
        V.push_back(i);                             
    }

    CL = V;
    srand(time(NULL));
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

double calculateSwapCost(Solucao& s, Data& d, int first, int second){
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

bool bestImprovementSwap(Solucao &s, Data& d){
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

double calculate2OptCost(Solucao& s, Data& d, int first, int second){
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

bool bestImprovement2Opt(Solucao &s, Data &d){
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

double calculateOrOptCost(Solucao& s, Data& d, int first, int second, int amount){
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

bool bestImprovementOrOpt(Solucao &s, Data &d, int amount){
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


void BuscaLocal(Solucao& s, Data& d){

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

void clean(vector<int>& available, Solucao& s,  int position, int size){
    if(s.sequence[position] == s.sequence.front()){
        available.erase(available.begin() + position, available.begin() + position + size + 1);
        int max = ceil(s.sequence.size() / 10.0);
        if(max < 2)
            max = 2;
        vector<int> sizes;
        for(int i = 0; i < max; i++){
            sizes.push_back(i+2);
        }
        random_shuffle(sizes.begin(), sizes.end());
        int size = sizes.front();

        vector<int> pos;
        for(int i = 0; i < available.size() - size + 1; i++){
            pos.push_back(i);
        }
        random_shuffle(pos.begin(), pos.end());
        int position = pos.front();
        vector<int> second_slice;
        for(int i = position; i < position + size; i++){
            second_slice.push_back(available[i]);
        }
        cout << "Second slice: ";
        cout <<   endl << "1" << endl;
        for(int i = 0; i < second_slice.size(); i++){
            cout << second_slice[i] << " ";
        }
        cout << endl;
    }

    if(s.sequence[position + size - 1] == s.sequence.back()){
        available.erase(available.begin() + position - 1, available.begin() + position + size);
        int max = ceil(s.sequence.size() / 10.0);
        if(max < 2)
            max = 2;
        vector<int> sizes;
        for(int i = 0; i < max; i++){
            sizes.push_back(i+2);
        }
        random_shuffle(sizes.begin(), sizes.end());
        int size = sizes.front();

        vector<int> pos;
        for(int i = 0; i < available.size() - size + 1; i++){
            pos.push_back(i);
        }
        random_shuffle(pos.begin(), pos.end());
        int position = pos.front();
        vector<int> second_slice;
        for(int i = position; i < position + size; i++){
            second_slice.push_back(available[i]);
        }
        cout << "Second slice: ";
        cout <<  endl <<  "2" << endl;
        for(int i = 0; i < second_slice.size(); i++){
            cout << second_slice[i] << " ";
        }
        cout << endl;
    }

    if(s.sequence[position] != s.sequence.front() && s.sequence[position + size - 1] != s.sequence.back()){
        if(!(position > size) && !(s.sequence.size() - (position + size) > size)){
            int max = ceil(s.sequence.size() / 10.0);
            if(max < 2)
                max = 2;
            vector<int> sizes;
            for(int i = 0; i < max; i++){
                sizes.push_back(i+2);
            }
            random_shuffle(sizes.begin(), sizes.end());
            int size = sizes.front();

            vector<int> pos;
            for(int i = 0; i < s.sequence.size() - size + 1; i++){
                pos.push_back(i);
            }
            random_shuffle(pos.begin(), pos.end());
            int position = pos.front();

            vector<int> slice_1;
            for(int i = position; i < position + size; i++){
                slice_1.push_back(s.sequence[i]);
            }
            // debug
            cout << "First slice: ";
            for(int i = 0; i < slice_1.size(); i++){
                cout << slice_1[i] << " ";
            }
            cout << endl;
            // debug

            vector<int> available = s.sequence;
            clean(available, s, position, size);
                    
        }else{
            
            if(position > size){
                available.erase(available.begin() + position - 1, available.end());
                int max = ceil(s.sequence.size() / 10.0);
                if(max < 2)
                    max = 2;
                vector<int> sizes;
                for(int i = 0; i < max; i++){
                    sizes.push_back(i+2);
                }
                random_shuffle(sizes.begin(), sizes.end());
                int size = sizes.front();

                vector<int> pos;
                for(int i = 0; i < available.size() - size + 1; i++){
                    pos.push_back(i);
                }
                random_shuffle(pos.begin(), pos.end());
                int position = pos.front();
                vector<int> second_slice;
                for(int i = position; i < position + size; i++){
                    second_slice.push_back(available[i]);
                }
                cout << "Second slice: ";
                cout <<  endl << "3" << endl;
                for(int i = 0; i < second_slice.size(); i++){
                    cout << second_slice[i] << " ";
                }
                cout << endl;
            }
            
            if(s.sequence.size() - (position + size) > size){
                available.erase(available.begin(), available.begin() + position + size + 1);
                int max = ceil(s.sequence.size() / 10.0);
                if(max < 2)
                    max = 2;
                vector<int> sizes;
                for(int i = 0; i < max; i++){
                    sizes.push_back(i+2);
                }
                random_shuffle(sizes.begin(), sizes.end());
                int size = sizes.front();
            
                vector<int> pos;
                for(int i = 0; i < available.size() - size + 1; i++){
                    pos.push_back(i);
                }
                random_shuffle(pos.begin(), pos.end());
                int position = pos.front();
                vector<int> second_slice;
                for(int i = position; i < position + size; i++){
                    second_slice.push_back(available[i]);
                }
                cout << "Second slice: ";
                cout << endl << "4" << endl;
                for(int i = 0; i < second_slice.size(); i++){
                    cout << second_slice[i] << " ";
                }
                cout << endl;
            }
        }
    }
}

void Pertubacao(Solucao& s){

    int max = ceil(s.sequence.size() / 10.0);
    if(max < 2)
        max = 2;
    vector<int> sizes;
    for(int i = 0; i < max; i++){
        sizes.push_back(i+2);
    }
    random_shuffle(sizes.begin(), sizes.end());
    int size = sizes.front();

    vector<int> pos;
    for(int i = 0; i < s.sequence.size() - size + 1; i++){
        pos.push_back(i);
    }
    random_shuffle(pos.begin(), pos.end());
    int position = pos.front();

    // vector<int> slice_1;
    // for(int i = position; i < position + size; i++){
    //     slice_1.push_back(s.sequence[i]);
    // }
    // // debug
    // cout << "First slice: ";
    // for(int i = 0; i < slice_1.size(); i++){
    //     cout << slice_1[i] << " ";
    // }
    // cout << endl;
    // debug

    vector<int> available = s.sequence;
    clean(available, s, position, size);
    int change = 1;
    if(change){
        cout << "deu tudo certo!" << endl;
        change = 0;
    }
}

int main(){

    Data d = {{
        {0, 2, 1, 4, 9, 1},
        {2, 0, 5, 9, 7, 2},
        {1, 5, 0, 3, 8, 6},
        {4, 9, 3, 0, 2, 5},
        {9, 7, 8, 2, 0, 2},
        {1, 2 ,6, 5, 2, 0}
    }, 6};

    // Solucao s = {{5, 6, 2, 1, 3, 4, 5}, 0};
    // exibirSolucao(s, d);

    Solucao s1;
    s1 = Construcao(d);
    cout << "- solucao apos Construcao() : " << endl;
    exibirSolucao(s1, d);
    cout << endl;
    
    BuscaLocal(s1, d);
    cout << "- solucao apos BuscaLocal() : " << endl;
    exibirSolucao(s1, d);
    cout << endl;
    
    Pertubacao(s1);
    cout << "- solucao apos Pertubacao() : " << endl;
    exibirSolucao(s1, d);
    cout << endl;

}
