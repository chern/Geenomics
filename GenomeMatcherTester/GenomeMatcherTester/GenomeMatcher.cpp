#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

struct GenomeItem {
    Genome* genome;
    int position;
};

inline
bool compareGenomeMatches(const GenomeMatch& gm1, const GenomeMatch& gm2) {
    // return true if gm1 should come first
    if (gm1.percentMatch > gm2.percentMatch)
        return true;
    if (gm1.percentMatch < gm2.percentMatch)
        return false;
    return gm1.genomeName < gm2.genomeName;
}

inline
bool compareGenomeItemsByName(const GenomeItem& gi1, const GenomeItem& gi2) {
    // return true if gi1 should come first
    return gi1.genome->name() < gi2.genome->name();
}

inline
bool compareDNAMatches(const DNAMatch& dm1, const DNAMatch& dm2) {
    // returns true if dm1 should come first
    if (dm1.length > dm2.length)
        return true;
    if (dm1.length < dm2.length)
        return false;
    return dm1.position < dm2.position;
}

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    ~GenomeMatcherImpl();
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    int m_minSearchLength;
    vector<Genome*> m_genomes;
    Trie<GenomeItem> m_trie;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    m_minSearchLength = minSearchLength;
}

GenomeMatcherImpl::~GenomeMatcherImpl() {
    vector<Genome*>::iterator it = m_genomes.begin();
    while (it != m_genomes.end()) {
        Genome* g = *it;
        delete g;
        g = nullptr;
        it = m_genomes.erase(it);
    }
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    string genomeSequence = "";
    genome.extract(0, genome.length(), genomeSequence);
    Genome* allocatedGenome = new Genome(genome.name(), genomeSequence);
    m_genomes.emplace_back(allocatedGenome);
    for (int i = 0; i <= genome.length() - minimumSearchLength(); i++) {
        GenomeItem gen;
        gen.genome = allocatedGenome;
        gen.position = i;
        string g = "";
        if (genome.extract(i, minimumSearchLength(), g)) {
            m_trie.insert(g, gen);
        }
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    if (fragment.length() < minimumLength || minimumLength < minimumSearchLength())
        return false;
    bool foundAtLeastOneGenome = false;
    matches.clear();
    
    vector<GenomeItem> potentialMatches = m_trie.find(fragment.substr(0, minimumSearchLength()), exactMatchOnly);
    sort(potentialMatches.begin(), potentialMatches.end(), compareGenomeItemsByName);
    cout << "potentialMatches.size() = " << potentialMatches.size() << endl;
    for (int i = 0; i < potentialMatches.size(); i++) {
        Genome* g = potentialMatches[i].genome;
        int matchLength = minimumSearchLength();
        
        for (int f = minimumSearchLength(); f < fragment.size(); f++) {
            string nextGenomeLetter = "";
            g->extract(potentialMatches[i].position + f, 1, nextGenomeLetter);
            string nextFragmentLetter = fragment.substr(f, 1);
            if (exactMatchOnly) {
                if (nextFragmentLetter == nextGenomeLetter) {
                    matchLength++;
                    continue;
                } else {
                    break;
                }
            } else { // if !exactMatchOnly
                string genomeUpToThisPoint = "";
                g->extract(potentialMatches[i].position, f, genomeUpToThisPoint);
                if (genomeUpToThisPoint == fragment.substr(0, f)) {
                    // up to this index f, the fragment and the genome sequence match, so a different letter can be allowed
                    matchLength++;
                    continue;
                } else {
                    // otherwise, check if the next letters are equal
                    if (nextFragmentLetter == nextGenomeLetter) {
                        matchLength++;
                        continue;
                    } else {
                        break;
                    }
                }
            }
        }
        
        if (matchLength >= minimumLength) {
            foundAtLeastOneGenome = true;
            DNAMatch newMatch;
            newMatch.genomeName = g->name();
            newMatch.length = matchLength;
            newMatch.position = potentialMatches[i].position;
            if (matches.size() > 0) {
                // because the potential matches were sorted alphabetically by name, if there was a previous DNAMatch of the same genome inserted, it would show up right before this current one
                // thus, compare it to find the one of a higher length
                if (matches.at(matches.size() - 1).genomeName == newMatch.genomeName) {
                    if (compareDNAMatches(newMatch, matches.at(matches.size() - 1))) {
                        matches.at(matches.size() - 1) = newMatch;
                    }
                } else {
                    matches.emplace_back(newMatch);
                }
            } else {
                matches.emplace_back(newMatch);
            }
        }
    }
    
    for (int m = 0; m < matches.size(); m++) {
        
    }
    
    return foundAtLeastOneGenome;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if (fragmentMatchLength < minimumSearchLength())
        return false;
    bool foundAtLeastOneGenome = false;
    int numSequences = query.length() / fragmentMatchLength;
    map<string, int> genomesNumMatches;
    
    for (int i = 0; i < numSequences; i++) {
        string queryExtraction = "";
        query.extract(i * fragmentMatchLength, fragmentMatchLength, queryExtraction);
        vector<DNAMatch> matches;
        findGenomesWithThisDNA(queryExtraction, fragmentMatchLength, exactMatchOnly, matches);
        for (int i = 0; i < matches.size(); i++) {
            map<string, int >::iterator it = genomesNumMatches.find(matches[i].genomeName);
            if (it != genomesNumMatches.end()) {
                it->second++;
            } else {
                genomesNumMatches[matches[i].genomeName] = 1;
            }
        }
    }
    
    results.clear();
    map<string, int>::iterator genomesNumMatchesIt = genomesNumMatches.begin();
    while (genomesNumMatchesIt != genomesNumMatches.end()) {
        double p = static_cast<double>(genomesNumMatchesIt->second / numSequences);
        if (p >= matchPercentThreshold) {
            foundAtLeastOneGenome = true;
            GenomeMatch gm;
            gm.genomeName = genomesNumMatchesIt->first;
            gm.percentMatch = p;
            results.emplace_back(gm);
        }
        genomesNumMatchesIt++;
    }
    sort(results.begin(), results.end(), compareGenomeMatches);
    
    return foundAtLeastOneGenome;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
