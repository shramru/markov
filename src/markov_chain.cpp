//
// Created by vladislav on 23.10.16.
//

#include "markov_chain.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "hash_combine.hpp"

std::function<size_t (const std::vector<std::wstring>&)>
        MarkovChain::vectorHash = [](const std::vector<std::wstring>& v) {
            size_t size = v.size();
            size_t seed = 0;
            for (size_t i = 0; i < size; ++i)
                ::hash_combine(seed, v[i], i);
            return seed;
        };

std::function<bool (const std::vector<std::wstring>&, const std::vector<std::wstring>&)>
        MarkovChain::vectorEqual = [](const std::vector<std::wstring>& a, const std::vector<std::wstring>& b) {
            if (a.size() != b.size()) return false;
            size_t size = a.size();
            for (size_t i = 0; i < size; ++i) {
                if (a[i] != b[i]) return false;
            }
            return true;
        };

MarkovChain::MarkovChain()
        : baseToNode(BUCKET_SIZE, baseHash, baseEqual),
          wordsToBase(BUCKET_SIZE, vectorHash, vectorEqual),
          basesWeak(baseLess) {}

MarkovChain MarkovChain::fromText(const std::wstring& text, int n) {
    MarkovChain chain;
    std::wstring words = removePunctuation(text);
    std::wstringstream ss(toLowerCase(words));

    /*add first base*/
    std::wstring word;
    BasePtr base = std::make_shared<Base>();
    chain.bases[base->id] = base;
    std::vector<std::wstring> wordsBase;
    for (int i = 0; i < n; ++i) {
        ss >> word;
        NodePtr nodePtr = std::make_shared<Node>(word);
        base->nodes.push_back(nodePtr);
        chain.nodes[nodePtr->id] = nodePtr;
        wordsBase.push_back(word);
    }
    chain.wordsToBase[wordsBase] = base;
    chain.basesWeak.insert(base);

    ss >> word;
    NodePtr prev = std::make_shared<Node>(word);
    chain.nodes[prev->id] = prev;
    chain.baseToNode[base] = prev;
    ++base->childToCount[prev];
    /*first base end*/

    while (ss >> word) {
        NodePtr node = std::make_shared<Node>(word);
        BasePtr newBase = std::make_shared<Base>();
        for (size_t i = 1; i < base->nodes.size(); ++i) {
            newBase->nodes.push_back(base->nodes[i]);
        }
        newBase->nodes.push_back(prev);

        NodeWPtr& wNode = chain.baseToNode[newBase];
        if (wNode.lock() == nullptr) {
            wNode = node;
            chain.bases[newBase->id] = newBase;
            chain.basesWeak.insert(newBase);
            wordsBase.clear();
            for (const auto& w : newBase->nodes)
                wordsBase.push_back(w.lock()->value);
            chain.wordsToBase[wordsBase] = newBase;
        } else {
            newBase = chain.basesWeak.find(newBase)->lock();
        }

        base->childToBase[prev] = newBase;
        prev = node;
        chain.nodes[prev->id] = prev;
        ++newBase->childToCount[prev];
        base = newBase;
    }

    return chain;
}

MarkovChain MarkovChain::fromSavedFile(const std::string& filename) {
    MarkovChain chain;
    std::wifstream fs(filename);
    if (!fs) {
        fs.close();
        throw std::invalid_argument("File not accessible");
    }

    size_t nodesSize, basesSize;
    fs >> nodesSize >> basesSize;

    for (size_t i = 0; i < nodesSize && fs; ++i) {
        NodePtr node = std::make_shared<Node>();
        fs >> *node;
        chain.nodes[node->id] = node;
    }

    for (size_t i = 0; i < basesSize && fs; ++i) {
        BasePtr base = std::make_shared<Base>();

        readBase(fs, base, chain.nodes, chain.bases);
        chain.bases[base->id] = base;
    }

    size_t bnSize;
    fs >> bnSize;
    for (size_t i = 0; i < basesSize && fs; ++i) {
        long bID, nID;
        fs >> bID >> nID;
        BasePtr base = chain.bases[bID];
        NodePtr node = chain.nodes[nID];
        chain.baseToNode[base] = node;

        std::vector<std::wstring> wds;
        for (const auto& n : base->nodes)
            wds.push_back(n.lock()->value);
        chain.wordsToBase[wds] = base;
    }

    fs.close();
    return chain;
}

void MarkovChain::save(const std::string &filename) {
    std::wofstream fs(filename);
    if (!fs) {
        fs.close();
        throw std::invalid_argument(filename);
    }

    fs << nodes.size() << ' ' << bases.size() << '\n';

    for (const auto& n : nodes)
        fs << *n.second;
    for (const auto& b : bases)
        fs << *b.second;

    fs << baseToNode.size() << '\n';
    for (const auto& p : baseToNode)
        fs << p.first.lock()->id << ' ' << p.second.lock()->id << '\n';

    fs << std::flush;
    fs.close();
}

std::wstring MarkovChain::next(const std::vector<std::wstring> &base, int n) {
    std::vector<std::wstring> lowerBase;
    for (auto& s : base)
        lowerBase.push_back(toLowerCase(s));

    std::wstring result;
    for (const auto& b : base)
        result += b + L' ';
    result.pop_back();

    BasePtr basePtr =  wordsToBase[lowerBase].lock();
    if (basePtr == nullptr)
        return result;

    NodePtr node = baseToNode[basePtr].lock();
    if (node == nullptr)
        return result;

    for(int i = 0; i < n; ++i) {
        result += L' ';
        result += node->value;

        BasePtr childBase = basePtr->childToBase[node].lock();
        if (childBase == nullptr) break;
        basePtr = childBase;

        node = std::max_element(childBase->childToCount.begin(), childBase->childToCount.end(),
                                [] (const auto& a, const auto& b) {
                                    return a.second < b.second;
                                })->first.lock();
    }

    return result;
}

void MarkovChain::readBase(std::wifstream &fs, BasePtr &base, std::map<long, NodePtr> &nodes,
                           std::map<long, BasePtr, std::greater<long>> &bases) {
    base->nodes.clear();
    base->childToBase.clear();
    base->childToCount.clear();

    fs >> base->id;

    size_t nodesSize;
    fs >> nodesSize;
    base->nodes.reserve(nodesSize);
    for (size_t i = 0; i < nodesSize; ++i) {
        long id;
        fs >> id;
        base->nodes.push_back(nodes[id]);
    }

    size_t ccSize;
    fs >> ccSize;
    base->childToCount.reserve(ccSize);
    for (size_t i = 0; i < ccSize; ++i) {
        long id, count;
        fs >> id >> count;
        base->childToCount[nodes[id]] = count;
    }

    size_t cbSize;
    fs >> cbSize;
    base->nodes.reserve(cbSize);
    for (size_t i = 0; i < cbSize; ++i) {
        long nId, bId;
        fs >> nId >> bId;
        base->childToBase[nodes[nId]] = bases[bId];
    }

}

std::wstring MarkovChain::removePunctuation(const std::wstring &text) {
    std::wstring result;
    const std::locale loc;
    std::remove_copy_if(text.begin(), text.end(), std::back_inserter(result),
                        [&] (auto c) { return std::ispunct(c, loc); }
    );
    return result;
}

std::wstring MarkovChain::toLowerCase(const std::wstring &text) {
    std::wstring result(text.size(), 0);
    const std::locale loc;
    std::transform(text.begin(), text.end(), result.begin(),
                   [&] (auto c) { return std::tolower(c, loc); });
    return result;
}