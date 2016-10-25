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

    std::wifstream fs(filename);

    if (!fs) {
        std::cout << "Invalid filename or file busy" << std::endl;
        return 1;
    }

    if (n == 0) {
        std::cout << "Invalid chain degree" << std::endl;
        return 1;
    }

    std::cout << "Loading text from " << filename << std::endl;
    std::wstring content((std::istreambuf_iterator<wchar_t>(fs)), std::istreambuf_iterator<wchar_t>());

    std::cout << "Learning..." << std::endl;
    MarkovChain markovChain = MarkovChain::fromText(content, n);

    std::cout << "Saving to " << output << std::endl;
    try {
        markovChain.save(output);
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what();
    }

    std::cout << "Chain successfully created" << std::endl;
    return 0;
}