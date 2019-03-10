#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct TrieNode {
        TrieNode(const std::string& l) {
            label = l;
        }
        char label;
        std::vector<ValueType> values;
        std::vector<TrieNode*> children;
    };
    TrieNode* root;
    void resetHelper(TrieNode* n);
    void insertHelper(const std::string& key, const ValueType& value, TrieNode* n);
};

template<typename ValueType>
Trie<ValueType>::Trie() {
    root = nullptr;
}

template<typename ValueType>
Trie<ValueType>::~Trie() {
    reset();
}

template<typename ValueType>
void Trie<ValueType>::reset() {
    resetHelper(root);
}

template<typename ValueType>
void Trie<ValueType>::resetHelper(TrieNode* n) {
    std::vector<TrieNode*>* childrenPointer = &(n->children);
    if (childrenPointer->size() <= 0) {
        return;
    }
    for (int i = 0; i < childrenPointer->size(); i++) {
        resetHelper(n->children[i]);
    }
    delete n;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
    insertHelper(key, value, root);
}

template<typename ValueType>
void Trie<ValueType>::insertHelper(const std::string& key, const ValueType& value, TrieNode* n) {
    if (n == nullptr) {
        
    }
}

#endif // TRIE_INCLUDED
