//
// Created by vladislav on 23.10.16.
//

#ifndef MARKOV_MARKOVCHAIN_HPP
#define MARKOV_MARKOVCHAIN_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "hash_vector.hpp"

class MarkovChain {

    struct Base; struct Node;
    typedef std::shared_ptr<Base> BasePtr;
    typedef std::shared_ptr<Node> NodePtr;

    class Base {
        static long currentId;
        static std::function<size_t (const NodePtr&)> nodeHash;
        static std::function<bool (const NodePtr&, const NodePtr&)> nodeEqual;

    public:
        long id;
        std::vector<NodePtr> nodes;
        std::unordered_map<NodePtr, int, decltype(nodeHash), decltype(nodeEqual)> childToCount;
        std::unordered_map<NodePtr, BasePtr, decltype(nodeHash), decltype(nodeEqual)> childToBase;

        Base();
    };

    class Node {
        static long currentId;

    public:
        long id;
        BasePtr base;
        std::wstring value;

        void setBaseFromPrevious(const NodePtr& prev);

        Node(const std::wstring& value);
        Node(const std::wstring& value, const BasePtr& base);
    };

    static std::function<size_t (const BasePtr&)> baseHash;
    static std::function<bool (const BasePtr&, const BasePtr&)> baseEqual;

    static std::wstring removePunctuation(const std::wstring& text);
    static std::wstring toLowerCase(const std::wstring& text);

    std::unordered_map<BasePtr, NodePtr, decltype(baseHash), decltype(baseEqual)> baseToNode;
    std::unordered_map<std::vector<std::wstring>, BasePtr> wordsToBase;

    MarkovChain();

public:
    static MarkovChain fromText(const std::wstring& text, int n);
    static MarkovChain fromSavedFile(const std::wstring& filename);

    void save(const std::string& filename);
    std::wstring next(const std::vector<std::wstring>& base, int n);
};


#endif //MARKOV_MARKOVCHAIN_HPP
