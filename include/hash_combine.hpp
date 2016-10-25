//
// Created by vladislav on 24.10.16.
//

#ifndef MARKOV_HASH_COMBINE_HPP
#define MARKOV_HASH_COMBINE_HPP

template <class T>
inline void hash_combine(std::size_t& seed, const T& v, std::size_t pos) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2) + pos;
}

#endif //MARKOV_HASH_COMBINE_HPP
