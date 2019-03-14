#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "provided.h"

using namespace std;

void someFunc() {
    string filename = "/Users/wchern/Documents/CS32_Geenomics/Gee-nomics_skeleton/data/Ferroplasma_acidarmanus.txt";
    
    ifstream strm(filename);
    if (!strm) {
        cout << "Cannot open " << filename << endl;
        return;
    }
    
    vector<Genome> vg;
    bool success = Genome::load(strm, vg);
    
    if (success) {
        cout << "Loaded " << vg.size() << " genomes successfully:" << endl;
        for (int k = 0; k != vg.size(); k++)
            cout << vg[k].name() << endl;
    } else
        cout << "Error loading genome data" << endl;
}

int main() {
    Genome g("oryx", "GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
    
    string f1, f2, f3;
    bool result1 = g.extract(0, 5, f1);
    bool result2 = g.extract(74, 6, f2);
    bool result3 = g.extract(74, 7, f3);
    
    if (result1) {
        cout << f1 << endl;
    }
    if (result2) {
        cout << f2 << endl;
    }
    if (result3) {
        cout << f3 << endl;
    }
    
    someFunc();
}
