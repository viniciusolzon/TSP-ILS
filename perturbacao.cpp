#include "ILS.h"

Solucao Pertubacao(Solucao& s, Data& d){
    Solucao s_copy;
    s_copy.sequence = s.sequence;
    s_copy.cost = s.cost;

    int lim = ceil(s_copy.sequence.size() / 10.0);
    int coin = rand() % 2;

    double origin = 0.0, after = 0.0, cost = 0.0;
    origin = d.matrizAdj[s_copy.sequence[s_copy.sequence.size() - 2]][s_copy.sequence[s_copy.sequence.size() - 1]];

    // cout << "before change s_copy: ";
    // for(int i = 0; i < s_copy.sequence.size(); i++)
    //     cout << s_copy.sequence[i] << " ";
    // cout << "\n";

    s_copy.sequence.pop_back();

    int size1, start1, end1, size2, start2, end2;
    size1 = max(2, rand() % lim);
    start1 = rand() % (s_copy.sequence.size() - size1 + 1);
    end1 = start1 + size1 - 1;
    size2 = max(2, rand() % lim);

    if(s_copy.sequence.size() - 1 - end1 > size2 && start1 > size2){
        int coin2 = rand() % 2;
        if(coin2 == 0){
            int min = end1 + 2;
            int max = s_copy.sequence.size() - size2;
            start2 = rand()%(max - min + 1) + min;
            end2 = start2 + size2 - 1;
        }
        else{
            int max = start1 - 2;
            start2 = rand()% max;
            end2 = start2 + size2 - 1;
        }
    }
    else if(s_copy.sequence.size() - 1 - end1 > size2){
            int min = end1 + 2;
            int max = s_copy.sequence.size() - size2;
            start2 = rand()%(max - min + 1) + min;
            end2 = start2 + size2 - 1;
            
    }   else{
            int max = start1 - 2;
            start2 = rand() % max;
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

    cout << "before change s_copy: ";
    for(int i = 0; i < s_copy.sequence.size(); i++)
        cout << s_copy.sequence[i] << " ";
    cout << "\n";

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

    cout << "post change s_copy: ";
    for(int i = 0; i < s_copy.sequence.size(); i++)
        cout << s_copy.sequence[i] << " ";
    cout << "\n\n";

    s_copy.sequence.push_back(s_copy.sequence[0]);

    // cout << "post change s_copy: ";
    // for(int i = 0; i < s_copy.sequence.size(); i++)
    //     cout << s_copy.sequence[i] << " ";
    // cout << "\n\n";

    after += d.matrizAdj[s_copy.sequence[s_copy.sequence.size() - 2]][s_copy.sequence[s_copy.sequence.size() - 1]];
    cost = after - origin;
    s_copy.cost += cost;

    return s_copy;
}
