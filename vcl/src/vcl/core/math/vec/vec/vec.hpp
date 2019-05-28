
#pragma once

#include <array>
#include <cmath>
#include <cassert>

#include <iostream>

namespace vcl {

/** Generic fixed-size vector used to define more easily vec2/3/4 */
template <std::size_t N>
struct vec
{
    /** Internal data is a std::array of float values */
    std::array<float,N> data;

    /** Size of the vector (known at compile time) */
    std::size_t size() const;

    /** Get operator at given index */
    const float& operator[](std::size_t index) const;
    /** Set operator at given index */
    float& operator[](std::size_t index);
};


/** ************************************************** **/
/**           Functions that can be applied on vec<N>  **/
/** ************************************************** **/

/** Retrieve size of the vector (known at compile time) */
template <std::size_t N> std::size_t size(const vec<N>& v);

// Standard math operators
// **************************************************** //
template <std::size_t N> vec<N>& operator+=(vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N>& operator-=(vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N>& operator*=(vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N>& operator*=(vec<N>& a, float b);
template <std::size_t N> vec<N>& operator/=(vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N>& operator/=(vec<N>& a, float b);

template <std::size_t N> vec<N> operator+(const vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N> operator-(const vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N> operator*(const vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N> operator*(const vec<N>& a, float b);
template <std::size_t N> vec<N> operator*(float a, const vec<N>& b);
template <std::size_t N> vec<N> operator/(const vec<N>& a, const vec<N>& b);
template <std::size_t N> vec<N> operator/(const vec<N>& a, float b);

/** Allows to display vec content on the standard output */
template <std::size_t N> std::ostream& operator<<(std::ostream& s, const vec<N>& v);

/** Convert content of the vector into string */
template <std::size_t N> std::string to_string(const vec<N>& v, const std::string& separator=" ");

/** Scalar product between two vec */
template <std::size_t N> float dot(const vec<N>& a, const vec<N>& b);
/** Compute 2-norm of vec */
template <std::size_t N> float norm(const vec<N>& v);
/** Normalize vec to have a unit norm */
template <std::size_t N> vec<N> normalize(const vec<N>& v);

/** ******************************************* **/
/**           Template implementation           **/
/** ******************************************* **/

template <std::size_t N> std::size_t vec<N>::size() const
{
    return N;
}
template <std::size_t N> std::size_t size(const vec<N>& )
{
    return N;
}


template <std::size_t N> float& vec<N>::operator[](std::size_t index)
{
    assert(index<N);
    return data[index];
}

template <std::size_t N> const float& vec<N>::operator[](std::size_t index) const
{
    assert(index<N);
    return data[index];
}

template <std::size_t N> vec<N>& operator+=(vec<N>& a, const vec<N>& b)
{
    for(std::size_t k=0; k<N; ++k){
        a[k] += b[k];
    }
    return a;

}

template <std::size_t N> vec<N>& operator-=(vec<N>& a, const vec<N>& b)
{
    for(size_t k=0; k<N; ++k) {
        a[k] -= b[k];
    }
    return a;
}

template <std::size_t N> vec<N>& operator*=(vec<N>& a, const vec<N>& b)
{
    for(size_t k=0; k<N; ++k) {
        a[k] *= b[k];
    }
    return a;
}

template <std::size_t N> vec<N>& operator*=(vec<N>& a, float b)
{
    for(size_t k=0; k<N; ++k) {
        a[k] *= b;
    }
    return a;
}

template <std::size_t N> vec<N>& operator/=(vec<N>& a, const vec<N>& b)
{
    for(size_t k=0; k<N; ++k) {
        a[k] /= b[k];
    }
    return a;
}

template <std::size_t N> vec<N>& operator/=(vec<N>& a, float b)
{
    for(size_t k=0; k<N; ++k) {
        a[k] /= b;
    }
    return a;
}

template <std::size_t N> vec<N> operator+(const vec<N>& a, const vec<N>& b)
{
    vec<N> res=a;
    res += b;
    return res;
}
template <std::size_t N> vec<N> operator-(const vec<N>& a, const vec<N>& b)
{
    vec<N> res=a;
    res -= b;
    return res;
}

template <std::size_t N> vec<N> operator*(const vec<N>& a, const vec<N>& b)
{
    vec<N> res=a;
    res *= b;
    return res;
}

template <std::size_t N> vec<N> operator*(const vec<N>& a, float b)
{
    vec<N> res=a;
    res *= b;
    return res;
}

template <std::size_t N> vec<N> operator*(float a, const vec<N>& b)
{
    vec<N> res=b;
    res *= a;
    return res;
}

template <std::size_t N> vec<N> operator/(const vec<N>& a, const vec<N>& b)
{
    vec<N> res=a;
    res /= b;
    return res;
}

template <std::size_t N> vec<N> operator/(const vec<N>& a, float b)
{
    vec<N> res=a;
    res /= b;
    return res;
}

template <std::size_t N> std::string to_string(const vec<N>& v, const std::string& separator)
{
    std::string s;

    if( N==0 )
        return "";

    for(std::size_t k=0; k<N-1; ++k) {
        s += std::to_string(v[k])+separator;
    }
    s += std::to_string(v[N-1]);

    return s;
}

template <std::size_t N> std::ostream& operator<<(std::ostream& s, const vec<N>& v)
{
    s << to_string(v);
    return s;
}

template <std::size_t N> float dot(const vec<N>& a, const vec<N>& b)
{
    float res = 0.0f;
    for(std::size_t k=0; k<N; ++k) {
        res += a[k]*b[k];
    }
    return res;
}
template <std::size_t N> float norm(const vec<N>& v)
{
    float n2 = 0.0f;
    for(std::size_t k=0; k<N; ++k) {
        const float y = v[k];
        n2 += y*y;
    }
    const float n = std::sqrt(std::abs(n2));

    return n;
}
template <std::size_t N> bool is_equal(const vec<N>& a, const vec<N>& b)
{
    using vcl::is_equal;
    for(std::size_t k=0; k<N; ++k) {
        if( is_equal(a[k],b[k])==false )
            return false;
    }
    return true;
}

template <std::size_t N> vec<N> operator-(const vec<N>& a)
{
    vec<N> b;
    for(std::size_t k=0; k<N; ++k) {
        b[k] = -a[k];
    }
    return b;
}

#ifdef __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
template <std::size_t N>
vec<N> normalize(const vec<N>& v)
{
    const float n = norm(v);
    vec<N> u;

    if(n==0)
        u[0]=1;
    else
        u = v/n;
    return u;
}
#ifdef __linux__
#pragma GCC diagnostic pop
#endif


}
