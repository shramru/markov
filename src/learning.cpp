#include <iostream>
#include <fstream>
#include "markov_chain.hpp"

int main(int argc, char** argv) {
    std::locale::global(std::locale("en_US.UTF-8"));

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

    std::wfstream fs(filename);
    if (!fs.good()) {
        std::cout << "Invalid filename or file busy" << std::endl;
        return 1;
    }

    std::wstring content((std::istreambuf_iterator<wchar_t>(fs)), std::istreambuf_iterator<wchar_t>());

    MarkovChain markovChain = MarkovChain::fromText(content, n);
    markovChain.save(output);

    return 0;
}