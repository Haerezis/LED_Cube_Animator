#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <functional>
  
template <typename T>
inline void hash_combine(std::size_t& seed,const T& v)
{
  seed ^= std::hash<T>()(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
}

#endif
