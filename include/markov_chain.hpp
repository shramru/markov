//
// Created by vladislav on 23.10.16.
//

#ifndef MARKOV_MARKOVCHAIN_HPP
#define MARKOV_MARKOVCHAIN_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <locale>
#include <vector>
#include "hash_vector.hpp"

class MarkovChain {

    struct Base;
    struct Node;
    typedef std::shared_ptr<Base> BasePtr;
    typedef std::shared_ptr<Node> NodePtr;

    struct Base {
        static int currentId;

        int id;
        std::vector<NodePtr> nodes;

        std::function<size_t (const NodePtr&)> nodeHash = [](const NodePtr& n) {
            return std::hash<std::wstring>()(n->value);
        };

        std::function<bool (const NodePtr&, const NodePtr&)> nodeEqual = [](const NodePtr& a, const NodePtr& b) {
            return a->value == b->value;
        };

        std::unordered_map<NodePtr, int, decltype(nodeHash), decltype(nodeEqual)> childToCount;
        std::unordered_map<NodePtr, BasePtr, decltype(nodeHash), decltype(nodeEqual)> childToBase;

        Base();
    };

    struct Node {
        static int currentId;

        int id;

        BasePtr base;
        std::wstring value;

        void setBaseFromPrevious(const NodePtr& prev);

        Node(const std::wstring& value);
        Node(const std::wstring& value, const BasePtr& base);
    };

    std::function<size_t (const BasePtr&)> baseHash = [](const BasePtr& b) {
        size_t size = b->nodes.size();
        size_t seed = 0;
        for (size_t i = 0; i < size; ++i)
            ::hash_combine(seed, b->nodes[i]->value, i);
        return seed;
    };

    std::function<bool (const BasePtr&, const BasePtr&)> baseEqual = [](const BasePtr& a, const BasePtr& b) {
        if (a->nodes.size() != b->nodes.size()) return false;
        for (size_t i = 0; i < a->nodes.size(); ++i) {
            if (a->nodes[i]->value != b->nodes[i]->value)
                return false;
        }
        return true;
    };

    std::unordered_map<BasePtr, NodePtr, decltype(baseHash), decltype(baseEqual)> baseToNode;
    std::unordered_map<std::vector<std::wstring>, BasePtr> wordsToBase;

    static std::wstring removePunctuation(const std::wstring& text);
    static std::wstring toLowerCase(const std::wstring& text);

    MarkovChain();

public:
    static MarkovChain fromText(const std::wstring& text, int n);
    static MarkovChain fromSavedFile(const std::wstring& filename);

    void save(const std::string& filename);
    std::wstring next(const std::vector<std::wstring>& base, int n);
};


#endif //MARKOV_MARKOVCHAIN_HPP
