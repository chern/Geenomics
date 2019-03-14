#include "provided.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

using namespace std;

void printDNAMatchVector(vector<DNAMatch>& m) {
    for (int i = 0; i < m.size(); i++) {
        cout << m.at(i).genomeName << " of length " << m.at(i).length << " at position " << m.at(i).position << endl;
    }
}

int main() {
    Genome g1("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
    Genome g2("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
    Genome g3("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");
    
    GenomeMatcher gm(4);
    gm.addGenome(g1);
    gm.addGenome(g2);
    gm.addGenome(g3);
    
    vector<DNAMatch> matches;
    bool result;
    
    cout << "GAAG, 4, true" << endl;
    result = gm.findGenomesWithThisDNA("GAAG", 4, true, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GAATAC, 4, true" << endl;
    result = gm.findGenomesWithThisDNA("GAATAC", 4, true, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GAATAC, 6, true" << endl;
    result = gm.findGenomesWithThisDNA("GAATAC", 6, true, matches);
    if (!result) {
        cout << "!result\n" << endl;
    }
    
    cout << "GAATAC, 6, false" << endl;
    result = gm.findGenomesWithThisDNA("GAATAC", 6, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GTATAT, 6, false" << endl;
    result = gm.findGenomesWithThisDNA("GTATAT", 6, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GAATACG, 6, false" << endl;
    result = gm.findGenomesWithThisDNA("GAATACG", 6, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GAAGGGTT, 5, false" << endl;
    result = gm.findGenomesWithThisDNA("GAAGGGTT", 5, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "GAAGGGTT, 6, false" << endl;
    result = gm.findGenomesWithThisDNA("GAAGGGTT", 6, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    }
    
    cout << "ACGTGCGAGACTTAGAGCC, 12, false" << endl;
    result = gm.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCC", 12, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    } else {
        cout << "!result\n" << endl;
    }
    
    cout << "ACGTGCGAGACTTAGAGCG, 12, false" << endl;
    result = gm.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCG", 12, false, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    } else {
        cout << "!result\n" << endl;
    }
    
    cout << "GAAG, 3, true" << endl;
    result = gm.findGenomesWithThisDNA("GAAG", 3, true, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    } else {
        cout << "!result\n" << endl;
    }
    
    cout << "GAAG, 5, true" << endl;
    result = gm.findGenomesWithThisDNA("GAAG", 5, true, matches);
    if (result) {
        printDNAMatchVector(matches);
        cout << endl;
    } else {
        cout << "!result\n" << endl;
    }
    
    cout << "Reached end of main()" << endl;
}
