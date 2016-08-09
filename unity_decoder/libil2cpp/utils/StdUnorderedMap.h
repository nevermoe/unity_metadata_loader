#pragma once

#if !IL2CPP_HAS_UNORDERED_CONTAINER // no c++11
#include <map>
#define unordered_map std::map
#elif defined(__clang__)
# if __has_include(<tr1/unordered_map>)
#  include <tr1/unordered_map>
using std::tr1::unordered_map;
# else
#  include <unordered_map>
using std::unordered_map;
# endif
#elif defined(__ARMCC_VERSION) // no c++11
#include <map>
#define unordered_map std::map
#define NO_UNORDERED_MAP 1
#elif defined(__GNUC__)
#  include <tr1/unordered_map>
using std::tr1::unordered_map;
#else
# include <unordered_map>
using std::unordered_map;
#endif
