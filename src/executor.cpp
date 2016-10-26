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
        std::wcout << L"Loading file..." << std::endl;
        auto t1 = clock();
        MarkovChain markovChain = MarkovChain::fromSavedFile(filename);
        auto t2 = clock();
        std::wcout << L"Time spent: " << (t2 - t1)*1000. / CLOCKS_PER_SEC << L" msec" << std::endl;

        while (std::wcin) {
            std::wcout << L"Please enter " << n << L" words:" << std::endl;
            std::vector<std::wstring> words((size_t) n);
            for (int i = 0; i < n; ++i)
                std::wcin >> words[i];

            std::wcout << L"Please enter count of generating words:" << std::endl;
            int k = 0;
            std::wcin >> k;
            if (k < 1)
                throw std::invalid_argument("invalid count");

            t1 = clock();
            std::wstring res = markovChain.next(words, k);
            t2 = clock();
            std::wcout << L"Time spent: " << (t2 - t1)*1000. / CLOCKS_PER_SEC << L" msec" << std::endl;

            std::wcout << res << std::endl;
        }
    } catch (std::exception& e) {
        std::wcout << L"Error: " << e.what();
    }
    return 0;
}