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
        TrieNode(const char& l) {
            label = l;
        }
        char label;
        std::vector<ValueType> values;
        std::vector<TrieNode*> children;
    };
    TrieNode* root;
    void resetHelper(TrieNode* n);
    std::vector<ValueType> findHelper(const std::string& key, int index, bool exactMatchOnly, TrieNode* n) const;
};

template<typename ValueType>
Trie<ValueType>::Trie() {
    root = new TrieNode(' ');
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
    if (n->children.size() <= 0) {
        return;
    }
    for (int i = 0; i < n->children.size(); i++) {
        resetHelper(n->children[i]);
    }
    delete n;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
    if (key.size() <= 0)
        return;
    TrieNode* currentNode = root;
    for (int i = 0; i < key.size(); i++) {
        bool found = false;
        for (int c = 0; c < currentNode->children.size(); c++) {
            if (key[i] == currentNode->children[c]->label) {
                currentNode = currentNode->children[c];
                found = true;
                break;
            }
        }
        if (!found) {
            TrieNode* newNode = new TrieNode(key[i]);
            currentNode->children.emplace_back(newNode);
            currentNode = newNode;
        }
        if (i == key.size() - 1) {
            currentNode->values.emplace_back(value);
            return;
        }
    }
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
    return findHelper(key, 0, exactMatchOnly, root);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::findHelper(const std::string& key, int index, bool exactMatchOnly, TrieNode* n) const {
    std::vector<ValueType> matches;
    if (n == root) {
        bool firstCharMatches = false;
        for (int i = 0; i < root->children.size(); i++) {
            if (key[0] == root->children.at(i)->label) {
                firstCharMatches = true;
                return findHelper(key, index, exactMatchOnly, root->children.at(i));
            }
        }
        if (!firstCharMatches)
            return matches; // if first char doesn't match, then return empty vector
    }
    
    if (index == (key.size() - 1)) {
        if (key[index] == n->label || !exactMatchOnly) {
            return n->values;
        } else if (exactMatchOnly) {
            return matches; // empty vector
        }
    } else {
        bool continueThroughChildren = false;
        if (exactMatchOnly && key[index] == n->label) {
            continueThroughChildren = true;
        } else if (!exactMatchOnly) {
            if (key[index] == n->label) {
                continueThroughChildren = true;
            } else {
                exactMatchOnly = true;
                continueThroughChildren = true;
            }
        }
        if (continueThroughChildren) {
            for (int c = 0; c < n->children.size(); c++) {
                std::vector<ValueType> childrenMatches = findHelper(key, index+1, exactMatchOnly, n->children.at(c));
                matches.insert(matches.end(), childrenMatches.begin(), childrenMatches.end());
            }
        }
    }
    
    return matches;
}

#endif // TRIE_INCLUDED
