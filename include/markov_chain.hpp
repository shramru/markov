//
// Created by vladislav on 23.10.16.
//

#ifndef MARKOV_MARKOVCHAIN_HPP
#define MARKOV_MARKOVCHAIN_HPP

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

class MarkovChain {

    static const size_t BUCKET_SIZE = 10;

    struct Base; struct Node;
    typedef std::shared_ptr<Base> BasePtr;
    typedef std::shared_ptr<Node> NodePtr;

    friend std::wostream &operator<<(std::wostream&, const MarkovChain::Node&);
    friend std::wistream &operator>>(std::wistream&, MarkovChain::Node&);
    friend std::wostream &operator<<(std::wostream&, const MarkovChain::Base&);
    friend std::wistream &operator>>(std::wistream&, MarkovChain::Base&);

    static std::function<size_t (const NodePtr&)> nodeHash;
    static std::function<bool (const NodePtr&, const NodePtr&)> nodeEqual;
    static std::function<size_t (const BasePtr&)> baseHash;
    static std::function<bool (const BasePtr&, const BasePtr&)> baseEqual;
    static std::function<size_t (const std::vector<std::wstring>&)> vectorHash;
    static std::function<bool (const std::vector<std::wstring>&, const std::vector<std::wstring>&)> vectorEqual;

    static std::wstring removePunctuation(const std::wstring& text);
    static std::wstring toLowerCase(const std::wstring& text);

    struct Base {
        static long currentId;

        long id;
        std::vector<NodePtr> nodes;
        std::unordered_map<NodePtr, int, decltype(nodeHash), decltype(nodeEqual)> childToCount;
        std::unordered_map<NodePtr, BasePtr, decltype(nodeHash), decltype(nodeEqual)> childToBase;

        Base();
    };

    struct Node {
        static long currentId;

        long id;
        std::wstring value;

        Node();
        Node(const std::wstring& value);
    };

    std::unordered_map<BasePtr, NodePtr, decltype(baseHash), decltype(baseEqual)> baseToNode;
    std::unordered_map<std::vector<std::wstring>, BasePtr, decltype(vectorHash), decltype(vectorEqual)> wordsToBase;

    std::map<long, NodePtr> nodes;
    std::map<long, BasePtr, std::greater<int>> bases;

    MarkovChain();

public:
    static MarkovChain fromText(const std::wstring& text, int n);
    static MarkovChain fromSavedFile(const std::string& filename);

    void save(const std::string& filename);
    std::wstring next(const std::vector<std::wstring>& base, int n);
};

#endif //MARKOV_MARKOVCHAIN_HPP
