//
// Created by vladislav on 23.10.16.
//

#include "markovchain.hpp"
#include <algorithm>
#include <sstream>

MarkovChain::Base::Base()
        : childToCount(10, nodeHash, nodeEqual), childToBase(10, nodeHash, nodeEqual) {}

MarkovChain::Node::Node(const std::wstring &value)
        : value(value) {
    base = std::make_shared<Base>();
}

MarkovChain::Node::Node(const std::wstring &value, const BasePtr &base)
        : value(value), base(base) {}

void MarkovChain::Node::setBaseFromPrevious(const std::shared_ptr<Node> &prev) {
    for (size_t i = 1; i < prev->base->nodes.size(); ++i) {
        base->nodes.push_back(prev->base->nodes[i]);
    }
    base->nodes.push_back(prev);
}

MarkovChain::MarkovChain() : baseToNode(10, baseHash, baseEqual) {}

MarkovChain MarkovChain::fromText(const std::wstring& text, int n) {
    MarkovChain chain;
    std::wstring words = removePunctuation(text);
    std::wstringstream ss(toLowerCase(words));
    words.clear();

    std::wstring word;
    BasePtr base = std::make_shared<Base>();
    std::vector<std::wstring> wordsBase;
    for (int i = 0; i < n; ++i) {
        ss >> word;
        base->nodes.push_back(std::make_shared<Node>(word));
        wordsBase.push_back(word);
    }
    chain.wordsToBase[wordsBase] = base;

    ss >> word;
    NodePtr prev = std::make_shared<Node>(word, base);
    chain.baseToNode[base] = prev;
    prev->base = base;
    ++base->childToCount[prev];

    while (ss >> word) {
        NodePtr node = std::make_shared<Node>(word);
        node->setBaseFromPrevious(prev);

        NodePtr& cNode = chain.baseToNode[node->base];
        if (cNode == nullptr) {
            cNode = node;
            wordsBase.clear();
            for (const auto& w : node->base->nodes)
                wordsBase.push_back(w->value);
            chain.wordsToBase[wordsBase] = node->base;
        }

        ++node->base->childToCount[node];
        prev->base->childToBase[prev] = node->base;
        prev = node;
    }

    return chain;
}

MarkovChain MarkovChain::fromSavedFile(const std::wstring& filename) {
    MarkovChain chain;

    return chain;
}

void MarkovChain::save(const std::wstring &filename) {

}

std::wstring MarkovChain::next(const std::vector<std::wstring> &base, int n) {
    std::wstring result;
    for (const auto& b : base)
        result += b + L' ';
    result.pop_back();

    BasePtr basePtr =  wordsToBase[base];
    if (basePtr == nullptr)
        return result;

    NodePtr node = baseToNode[basePtr];
    if (node == nullptr)
        return result;

    for(int i = 0; i < n; ++i) {
        result += L' ';
        result += node->value;

        BasePtr childBase = node->base->childToBase[node];
        if (childBase == nullptr) break;

        node = std::max_element(childBase->childToCount.begin(), childBase->childToCount.end(),
                                [] (const auto& a, const auto& b) {
                                    return a.second < b.second;
                                })->first;
    }

    return result;
}

std::wstring MarkovChain::removePunctuation(const std::wstring &text) {
    std::wstring result;
    std::locale loc;
    std::remove_copy_if(text.begin(), text.end(), std::back_inserter(result),
                        [&] (auto c) { return std::ispunct(c, loc); }
    );
    return result;
}

std::wstring MarkovChain::toLowerCase(const std::wstring &text) {
    std::wstring result(text.size(), 0);
    std::locale loc;
    std::transform(text.begin(), text.end(), result.begin(),
                   [&] (auto c) { return std::tolower(c, loc); });
    return result;
}