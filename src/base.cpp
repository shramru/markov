//
// Created by vladislav on 24.10.16.
//

#include "markov_chain.hpp"
#include "hash_combine.hpp"
#include <iostream>

long MarkovChain::Base::currentId = 0;

std::function<size_t (const MarkovChain::BaseWPtr&)>
        MarkovChain::baseHash = [](const BaseWPtr& bw) {
    auto b = bw.lock();
    size_t size = b->nodes.size();
    size_t seed = 0;
    for (size_t i = 0; i < size; ++i) {
        ::hash_combine(seed, b->nodes[i].lock()->value, i);
    }
    return seed;
};

std::function<bool (const MarkovChain::BaseWPtr&, const MarkovChain::BaseWPtr&)>
        MarkovChain::baseEqual = [](const BaseWPtr& aw, const BaseWPtr& bw) {
    auto a = aw.lock(); auto b = bw.lock();
    if (a->nodes.size() != b->nodes.size()) return false;
    for (size_t i = 0; i < a->nodes.size(); ++i) {
        if (a->nodes[i].lock()->value != b->nodes[i].lock()->value)
            return false;
    }
    return true;
};

std::function<bool (const MarkovChain::BaseWPtr&, const MarkovChain::BaseWPtr&)>
        MarkovChain::baseLess = [] (const BaseWPtr& aw, const BaseWPtr& bw) {
    auto a = aw.lock(), b = bw.lock();
    std::wstring as, bs;
    for (size_t i = 0; i < a->nodes.size(); ++i) {
        as += a->nodes[i].lock()->value;
        bs += b->nodes[i].lock()->value;
    }
    return as < bs;
};

std::wostream &operator<<(std::wostream &strm, const MarkovChain::Base &b) {
    strm << b.id << '\n';

    strm << b.nodes.size() << '\n';
    for (const auto& n : b.nodes)
        strm << n.lock()->id << ' ';
    strm << '\n';

    strm << b.childToCount.size() << '\n';
    for (const auto& p : b.childToCount)
        strm << p.first.lock()->id << ' ' << p.second << '\n';

    strm << b.childToBase.size() << '\n';
    for (const auto& p : b.childToBase)
        strm << p.first.lock()->id << ' ' << p.second.lock()->id << '\n';

    return strm;
}

MarkovChain::Base::Base()
        : id(currentId++), childToCount(BUCKET_SIZE, nodeHash, nodeEqual),
          childToBase(BUCKET_SIZE, nodeHash, nodeEqual) {}