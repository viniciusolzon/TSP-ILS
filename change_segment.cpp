#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){

    vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> slice1 = {7, 8, 9};
    vector<int> slice2 = {1, 2};
    int size1 = 3;
    int size2 = 2;
    
    int slice_1_start = 6;
    int slice_2_start = 1;

    for(int i = 0; i < size1; i++)
        v[slice_2_start + i] = slice1[i];
    
    for(int i = 0; i < size2; i++)
        v[slice_1_start + i] = slice2[i];
    
    for(int i = 0; i < v.size() - 1; i++)
        cout << v[i] << endl;

    return 0;
}
