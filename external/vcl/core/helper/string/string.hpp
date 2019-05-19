#pragma once

#include <string>
#include <vector>
#include <array>

namespace vcl
{

std::string zero_fill(const std::string& input, size_t number_of_zero);

template <typename T>
std::string to_string(const T& x)
{
    return std::to_string(x);
}

template <typename T, size_t N>
std::string to_string(std::array<T,N> v, const std::string& separator=" ")
{
    using vcl::to_string;

    std::string s;

    if( N==0 )
        return "";

    for(std::size_t k=0; k<N-1; ++k) {
        s += to_string(v[k])+separator;
    }
    s += to_string(v[N-1]);

    return s;
}

template <typename T>
std::string to_string(std::vector<T> v, const std::string& separator=" ")
{
    using vcl::to_string;

    std::string s;
    std::size_t N = v.size();

    if( N==0 )
        return "";

    for(std::size_t k=0; k<N-1; ++k) {
        s += to_string(v[k])+separator;
    }
    s += to_string(v[N-1]);

    return s;
}


}
