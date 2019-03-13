#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name;
    string m_sequence;
    static bool isValidChar(char c);
    static bool isValidSequence(string s);
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    // This compiles, but may not be correct
    m_name = nm;
    m_sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    if (!genomeSource)
        return false;
    
    // clear any objects in genomes vector
    genomes.clear();
    
    int genomesEncountered = 0;
    string genomeName = "";
    string genomeStr = "";
    string line;
    while (getline(genomeSource, line)) {
        if (genomesEncountered == 0 && line[0] != '>')
            return false; // if file starts with non-name line, return false
        if (line[0] == '>') {
            if (genomesEncountered > 0)
                genomes.emplace_back(Genome(genomeName, genomeStr));
            genomesEncountered++;
            genomeName = "";
            genomeStr = "";
            
            if (line.length() <= 1)
                return false;
            genomeName = line.substr(1, line.length()-1);
        } else {
            if (!isValidSequence(line))
                return false;
        }
    }
    genomes.emplace_back(Genome(genomeName, genomeStr));
    return true;
}

int GenomeImpl::length() const
{
    return m_sequence.length();
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if (position > this->length() || (position + length) > this->length())
        return false;
    fragment = m_sequence.substr(position, length);
    return true;
}

bool GenomeImpl::isValidChar(char c) {
    if (c == 'A' || c == 'C' || c == 'T' || c == 'G' || c == 'N')
        return true;
    return false;
}

bool GenomeImpl::isValidSequence(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isValidChar(s[i]))
            return false;
    }
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
