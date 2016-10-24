//
// Created by vladislav on 24.10.16.
//

#ifndef MARKOV_HASH_VECTOR_HPP
#define MARKOV_HASH_VECTOR_HPP

#include <vector>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v, size_t pos) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2) + pos;
}

namespace std {

    template<class T>
    struct hash<vector<T>> {
        typedef vector<T> argument_type;
        typedef size_t result_type;

        result_type operator()(const argument_type& in) const {
            size_t size = in.size();
            size_t seed = 0;
            for (size_t i = 0; i < size; ++i)
                ::hash_combine(seed, in[i], i);
            return seed;
        }
    };
}

#endif //MARKOV_HASH_VECTOR_HPP
