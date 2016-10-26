//
// Created by vladislav on 27.10.16.
//

#ifndef MARKOV_CALL_WITH_TIME_HPP
#define MARKOV_CALL_WITH_TIME_HPP

#include <iostream>

inline void printTime(std::ostream& output, clock_t time) {
    output << "Time spent: " << time * 1000. / CLOCKS_PER_SEC << " msec" << std::endl;
}

inline void printTime(std::wostream& output, clock_t time) {
    output << L"Time spent: " << time * 1000. / CLOCKS_PER_SEC << L" msec" << std::endl;
}

inline void callWithTime(auto& output, auto&& f, auto&&... args) {
    auto t1 = clock();
    f(args...);
    auto t2 = clock();
    printTime(output, t2 - t1);
}

template <class Ret>
inline Ret callWithTime(auto& output, auto&& f, auto&&... args) {
    auto t1 = clock();
    Ret res = f(args...);
    auto t2 = clock();
    printTime(output, t2 - t1);
    return res;
}


#endif //MARKOV_CALL_WITH_TIME_HPP
