#include "ILS.h"

vector<int> Pertubacao(Solucao& s){
    Solucao best = s;
    int lim = ceil(best.sequence.size() / 10.0);
    int size1 = max(2, rand() % lim);
    int start1 = rand() % (best.sequence.size() - size1 + 1);
    int end1 = start1 + size1 - 1;
    int size2 = max(2, rand() % lim);
    int start2 = rand() % (best.sequence.size() - size2 + 1);
    int end2 = start2 + size2 - 1;

    while(end1 + 1 >= start2 || start1 < 1 || start2 < 1 || end1 == best.sequence.size() - 1 || end2 == best.sequence.size() - 1){
        size1 = max(2, rand() % lim);
        start1 = rand() % (best.sequence.size() - size1 + 1);
        end1 = start1 + size1 - 1;
        size2 = max(2, rand() % lim);
        start2 = rand() % (best.sequence.size() - size2 + 1);
        end2 = start2 + size2 - 1;
    }

    best.sequence.erase(best.sequence.begin() + start1, best.sequence.begin() + start1 + size1);
    best.sequence.erase(best.sequence.begin() + start2 - size1, best.sequence.begin() + start2 - size1 + size2);
    for(int i = 0; i < size1; i++)
        best.sequence.insert(best.sequence.begin() + start1 + i, s.sequence[start2 + i]);
    for(int i = 0; i < size2; i++)
        best.sequence.insert(best.sequence.begin() + start2 + i, s.sequence[start1 + i]);

    return best.sequence;
}