//
// Created by vladislav on 24.10.16.
//

#include "markov_chain.hpp"
#include "hash_combine.hpp"
#include <iostream>

long MarkovChain::Base::currentId = 0;

std::function<size_t (const MarkovChain::BasePtr&)>
        MarkovChain::baseHash = [](const BasePtr& b) {
    size_t size = b->nodes.size();
    auto aaa = b->nodes;
    size_t seed = 0;
    for (size_t i = 0; i < size; ++i)
        ::hash_combine(seed, b->nodes[i]->value, i);
    return seed;
};

std::function<bool (const MarkovChain::BasePtr&, const MarkovChain::BasePtr&)>
        MarkovChain::baseEqual = [](const BasePtr& a, const BasePtr& b) {
    if (a->nodes.size() != b->nodes.size()) return false;
    for (size_t i = 0; i < a->nodes.size(); ++i) {
        if (a->nodes[i]->value != b->nodes[i]->value)
            return false;
    }
    return true;
};

std::wostream &operator<<(std::wostream &strm, const MarkovChain::Base &b) {
    strm << b.id << '\n';

    strm << b.nodes.size() << '\n';
    for (const auto& n : b.nodes)
        strm << n->id << ' ';
    strm << '\n';

    strm << b.childToCount.size() << '\n';
    for (const auto& p : b.childToCount)
        strm << p.first->id << ' ' << p.second << '\n';

    strm << b.childToBase.size() << '\n';
    for (const auto& p : b.childToBase)
        strm << p.first->id << ' ' << p.second->id << '\n';

    return strm;
}

std::wistream &operator>>(std::wistream &strm, MarkovChain::Base &b) {
    b.nodes.clear();
    b.childToBase.clear();
    b.childToCount.clear();

    strm >> b.id;

    size_t nodesSize;
    strm >> nodesSize;
    b.nodes.reserve(nodesSize);
    for (size_t i = 0; i < nodesSize; ++i) {
        MarkovChain::NodePtr node = std::make_shared<MarkovChain::Node>();
        strm >> node->id;
        b.nodes.push_back(node);
    }

    size_t ccSize;
    strm >> ccSize;
    b.childToCount.reserve(ccSize);
    for (size_t i = 0; i < ccSize; ++i) {
        MarkovChain::NodePtr node = std::make_shared<MarkovChain::Node>();
        int count;
        strm >> node->id >> count;
        b.childToCount[node] = count;
    }

    size_t cbSize;
    strm >> cbSize;
    b.nodes.reserve(cbSize);
    for (size_t i = 0; i < cbSize; ++i) {
        MarkovChain::NodePtr node = std::make_shared<MarkovChain::Node>();
        MarkovChain::BasePtr base = std::make_shared<MarkovChain::Base>();
        strm >> node->id >> base->id;
        b.childToBase[node] = base;
    }

    return strm;
}

MarkovChain::Base::Base()
        : id(++currentId), childToCount(BUCKET_SIZE, nodeHash, nodeEqual),
          childToBase(BUCKET_SIZE, nodeHash, nodeEqual) {}