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
    if (key.size() <= 0)
        return;
    if (root == nullptr) {
        root = new TrieNode(key[0]);
    }
    TrieNode* currentNode = root;
    for (int i = 1; i < key.size(); i++) {
        if (currentNode->children.size() == 0) {
            TrieNode newNode = new TrieNode(key[i]);
            currentNode->children.emplace_back(newNode);
            currentNode = newNode;
            // continue;
        } else {
            for (int n = 0; n < currentNode->children.size(); n++) {
                if (key == currentNode->children[i].label) {
                    currentNode = currentNode->children[i];
                    // continue;
                }
            }
        }
        if (i == key.size() - 1) {
            currentNode->values.emplace_back(value);
            return;
        }
    }
}

#endif // TRIE_INCLUDED
