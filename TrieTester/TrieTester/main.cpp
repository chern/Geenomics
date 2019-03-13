#include <iostream>
#include <vector>
#include "Trie.h"

int main() {
    Trie<int> t;
    t.insert("hit", 1);
    t.insert("hit", 2);
    t.insert("hi", 9);
    t.insert("hi", 17);
    t.insert("hip", 10);
    t.insert("hip", 20);
    t.insert("a", 14);
    t.insert("to", 22);
    t.insert("to", 23);
    t.insert("tap", 19);
    t.insert("tap", 6);
    t.insert("tap", 32);
    
    std::vector<int> result1 = t.find("hit", true); // should return {1, 2}
    for (int i = 0; i < result1.size(); i++) {
        std::cout << result1[i] << std::endl;
    }
    
    std::cout << "Reached end." << std::endl;
}
