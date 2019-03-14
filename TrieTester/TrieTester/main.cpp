#include <iostream>
#include <vector>
#include "Trie.h"

using namespace std;

int main() {
    Trie<int> t;
    t.insert("hit", 1);
    t.insert("hit", 2);
    t.insert("hi", 9);
    t.insert("hi", 17);
    t.insert("hip", 10);
    t.insert("hip", 20);
    t.insert("hat", 7);
    t.insert("hat", 8);
    t.insert("hat", 9);
    t.insert("a", 14);
    t.insert("to", 22);
    t.insert("to", 23);
    t.insert("tap", 19);
    t.insert("tap", 6);
    t.insert("tap", 32);
    
//    vector<int> v1 = {1, 2, 3};
//    vector<int> v2 = {21, 444, 2124, 652, 891};
//    v1.insert(v1.end(), v2.begin(), v2.end());
//    for (int i = 0; i < v1.size(); i++) {
//        cout << v1[i] << endl;
//    }
    
    vector<int> result1 = t.find("hit", true); // should return {1, 2}
    cout << "result1 size: " << result1.size() << endl;
    for (int i = 0; i < result1.size(); i++) {
        cout << result1[i] << " ";
    }
    cout << endl;
    
    vector<int> result2 = t.find("hit", false);
    cout << "result2 size: " << result2.size() << endl;
    for (int i = 0; i < result2.size(); i++) {
        cout << result2[i] << " ";
    }
    cout << endl;
    
    vector<int> result3 = t.find("sit", false);
    cout << "result3 size: " << result3.size() << endl;
    
    vector<int> result4 = t.find("hat", false);
    cout << "result4 size: " << result4.size() << endl;
    for (int i = 0; i < result4.size(); i++) {
        cout << result4[i] << " ";
    }
    cout << endl;
    
    vector<int> result5 = t.find("hat", true);
    cout << "result5 size: " << result5.size() << endl;
    for (int i = 0; i < result5.size(); i++) {
        cout << result5[i] << " ";
    }
    cout << endl;
    
    cout << "Reached the end of main()" << endl;
}
