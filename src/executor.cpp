#include <iostream>
#include <fstream>
#include "markov_chain.hpp"

int main(int argc, char** argv) {
    std::locale::global(std::locale(getenv("LANG")));

    if (argc < 3) {
        std::wcout << L"Please enter filename and chain degree in arguments." << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    int n = std::atoi(argv[2]);
    if (n == 0) {
        std::wcout << L"Invalid chain degree" << std::endl;
        return 1;
    }

    try {
        MarkovChain markovChain = MarkovChain::fromSavedFile(filename);

        std::wcout << L"Please enter " << n << L" words:" << std::endl;
        std::vector<std::wstring> words((size_t)n);
        for (int i = 0; i < n; ++i)
            std::wcin >> words[i];

        std::wcout << L"Please enter count of generating words:" << std::endl;
        int k;
        std::wcin >> k;

        std::wstring res = markovChain.next(words, k);
        std::wcout << res << std::endl;
    } catch (std::exception& e) {
        std::wcout << L"Error: " << e.what();
    }
    return 0;
}