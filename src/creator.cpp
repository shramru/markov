#include <iostream>
#include <fstream>
#include "markov_chain.hpp"

int main(int argc, char** argv) {
    std::locale::global(std::locale(getenv("LANG")));

    if (argc < 4) {
        std::cout << "Please enter filename, chain degree and output file in arguments." << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    int n = std::atoi(argv[2]);
    std::string output(argv[3]);

    if (n == 0) {
        std::cout << "Invalid chain degree" << std::endl;
        return 1;
    }

    try {
        std::cout << "Learning..." << std::endl;
        auto t1 = clock();
        MarkovChain markovChain = MarkovChain::fromTextFile(filename, n);
        auto t2 = clock();
        std::cout << "Time spent: " << (t2 - t1) * 1000. / CLOCKS_PER_SEC << " msec" << std::endl;

        std::cout << "Saving to " << output << std::endl;

        t1 = clock();
        markovChain.save(output);
        t2 = clock();
        std::cout << "Time spent: " << (t2 - t1) * 1000. / CLOCKS_PER_SEC << " msec" << std::endl;
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what();
    }

    std::cout << "Chain successfully created" << std::endl;
    return 0;
}