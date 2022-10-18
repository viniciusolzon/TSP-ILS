#include "ILS.h"

ILS :: ILS(Data d){
    dist = d;
    maxIter = 50;
    maxIterILS = d.vertices >= 150 ? d.vertices / 2 : d.vertices;
}

ILS :: ~ILS(){}

bool compare(const InsertionInfo &a, const InsertionInfo &b){
    return a.cost < b.cost;
}

double ILS :: epsilon(double a, double b){
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
}

bool ILS :: improve(double value_1, double value_2){
    return (value_1 - value_2) > epsilon(value_1, value_2);
}

void ILS :: calcularcost(Solucao *s, Data *d){
    s->cost = 0.0;
    for(int i = 0; i < s->sequence.size() - 1; i++)
        s->cost += d->matrizAdj[s->sequence[i]][s->sequence[i+1]];
}

void ILS :: solve(){
    auto start = std::chrono::high_resolution_clock::now(); // Inicia o cronômetro
    srand(time(NULL)); // Para conseguir gerar números aleatórios
    Solucao bestOfAll; // Cria a solução que guardará a melhor solução possível

    for(int i = 0; i < maxIter; i++){
        Solucao current = Construcao(&dist); // Cria a solução atual ( Muita gula e pouca aleatoriedade )
        calcularcost(&current, &dist); // Calcula o custo dessa solução com base na matriz de adjacência
        Solucao best = current; // Cria a solução que guardará a melhor solução dessa iteração
        if(i == 0)
            bestOfAll = current; // Se for a primeira solução criada ela já é atribuída para a melhor solução possível

        int iterIls = 0;
        while(iterIls <= maxIterILS){ // Enquanto houver melhoras nessa solução, reinicia-se o processo de melhora
            BuscaLocal(&current, &dist); // Tenta melhorar o máximo possível a solução atual
            if(improve(best.cost, current.cost)){ // Se a solução atual foi melhorada a melhor solução atual é atualizada pela atual
                best = current;
                iterIls = 0;
            }
            current = Pertubacao(&best, &dist); // Pertuba de forma aleatória a solução atual pra ver se ao alterá-la é possível melhorá-la
            iterIls++;
        }

        if(improve(bestOfAll.cost, best.cost)) // Se a melhor solução atual for melhor que a melhor solução possível, ela se torna a melhor solução possível
            bestOfAll = best;
    }

    auto end = std::chrono::high_resolution_clock::now(); // Para o cronômetro
    std::chrono::duration<double, std::milli> float_ms = end - start; // Calcula o tempo do cronômetro
    cout << "\nExecution time:  " << float_ms.count() / 1000.0000000000000 << " seconds" << "\n";

    best_solution = bestOfAll;
}

void ILS :: show_solution(){
    cout << "Sequence:  ";
    for(int i = 0; i < best_solution.sequence.size() - 1; i++)
        cout << best_solution.sequence[i] << " -> ";
    cout << best_solution.sequence.back() << endl;

    cout << "Cost:  " << setprecision(2) << fixed << best_solution.cost << "\n";
}
