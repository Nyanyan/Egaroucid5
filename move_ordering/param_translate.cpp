#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define n_data (349504 * 6)

int main(){
    ifstream ifs("learned_data/param.txt");
    if (ifs.fail()){
        cerr << "evaluation file not exist" << endl;
        return 0;
    }
    ofstream fout;
    fout.open("learned_data/move_ordering.egmo", ios::out|ios::binary|ios::trunc);
    if (!fout){
        cerr << "can't open eval.egev" << endl;
        return 0;
    }
    string line;
    short elem;
    short max_elem = -10000, min_elem = 10000;

    for (int i = 0; i < n_data; ++i){
        getline(ifs, line);
        elem = stoi(line);
        max_elem = max(max_elem, elem);
        min_elem = min(min_elem, elem);
        fout.write((char*)&elem, 2);
    }
    fout.close();
    ifs.close();
    cerr << max_elem << " " << min_elem << endl;
    return 0;
}