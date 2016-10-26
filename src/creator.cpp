#include <iostream>
#include <fstream>
#include "markov_chain.hpp"
#include "call_with_time.hpp"

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
        MarkovChain markovChain = callWithTime<MarkovChain>(std::cout, &MarkovChain::fromTextFile, filename, n);
        std::cout << "Saving to " << output << std::endl;
        callWithTime(std::cout, std::bind(&MarkovChain::save, &markovChain, output));
        markovChain.save(output);
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "Chain successfully created" << std::endl;
    return 0;
}