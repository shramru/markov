//
// Created by vladislav on 24.10.16.
//

#include "markov_chain.hpp"
#include <iostream>

long MarkovChain::Node::currentId = 0;

std::function<size_t (const MarkovChain::NodeWPtr&)>
        MarkovChain::nodeHash = [](const NodeWPtr& n) {
    return std::hash<std::wstring>()(n.lock()->value);
};

std::function<bool (const MarkovChain::NodeWPtr&, const MarkovChain::NodeWPtr&)>
        MarkovChain::nodeEqual = [](const NodeWPtr& a, const NodeWPtr& b) {
    return a.lock()->value == b.lock()->value;
};

std::wostream &operator<<(std::wostream &strm, const MarkovChain::Node &n) {
    return strm << n.id << ' ' << n.value << std::endl;
}

std::wistream &operator>>(std::wistream &strm, MarkovChain::Node &n) {
    return strm >> n.id >> n.value;
}

MarkovChain::Node::Node() : id(++currentId) {}

MarkovChain::Node::Node(const std::wstring &value)
        : id(++currentId), value(value) {}