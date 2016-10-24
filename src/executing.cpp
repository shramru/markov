#include <iostream>
#include "markov_chain.hpp"

int main(int argc, char** argv) {
    std::locale::global(std::locale("en_US.UTF-8"));

    MarkovChain markovChain = MarkovChain::fromSavedFile(L"");
    std::wstring res = markovChain.next({}, 1);

    std::wcout << res << std::endl;

    return 0;
}