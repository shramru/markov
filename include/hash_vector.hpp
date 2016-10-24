//
// Created by vladislav on 24.10.16.
//

#ifndef MARKOV_HASH_VECTOR_HPP
#define MARKOV_HASH_VECTOR_HPP

#include <vector>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v, std::size_t pos) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2) + pos;
}

namespace std {

    template<class T>
    struct hash<vector<T>> {
        size_t operator()(const vector<T>& in) const {
            size_t size = in.size();
            size_t seed = 0;
            for (size_t i = 0; i < size; ++i)
                ::hash_combine(seed, in[i], i);
            return seed;
        }
    };
}

#endif //MARKOV_HASH_VECTOR_HPP
