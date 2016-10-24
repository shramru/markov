#include <iostream>
#include <fstream>
#include "markovchain.hpp"

int main() {
    std::locale::global(std::locale("en_US.UTF-8"));

    std::wfstream fs("/home/vladislav/book1.txt");
    std::wstring content((std::istreambuf_iterator<wchar_t>(fs)), std::istreambuf_iterator<wchar_t>());

    MarkovChain markovChain = MarkovChain::fromText(content, 3);

    auto s = markovChain.next({ L"которая", L"провожала", L"ее" }, 60);

    return 0;
}