#include <iostream>
#include <fstream>
#include "markov_chain.hpp"
#include "call_with_time.hpp"

void generateWords(MarkovChain& markovChain) {
    while (std::wcin) {
        int n = markovChain.getDegree();

        std::wcout << L"Please enter " << n << L" words:" << std::endl;
        std::vector<std::wstring> words((size_t) n);
        for (int i = 0; i < n; ++i)
            std::wcin >> words[i];

        std::wcout << L"Please enter count of generating words:" << std::endl;
        int k = 0;
        std::wcin >> k;
        if (k < 1)
            throw std::invalid_argument("Invalid count");

        std::wstring res = callWithTime<std::wstring>(std::wcout, std::bind(&MarkovChain::next, &markovChain, words, k));
        std::wcout << res << std::endl;
    }
}

int main(int argc, char** argv) {
    std::locale::global(std::locale(getenv("LANG")));

    if (argc < 2) {
        std::wcout << L"Please enter filename in arguments." << std::endl;
        return 1;
    }

    std::string filename(argv[1]);

    try {
        std::wcout << L"Loading file..." << std::endl;
        MarkovChain markovChain = callWithTime<MarkovChain>(std::wcout, &MarkovChain::fromSavedFile, filename);
        generateWords(markovChain);
    } catch (std::exception& e) {
        std::wcout << L"Error: " << e.what() << std::endl;
    }
    return 0;
}