#pragma once

#if !IL2CPP_HAS_UNORDERED_CONTAINER // no c++11
#include <set>
#define unordered_set std::set
#elif defined(__clang__)
# if __has_include(<tr1/unordered_set>)
#  include <tr1/unordered_set>
using std::tr1::unordered_set;
# else
#  include <unordered_set>
using std::unordered_set;
# endif
#elif defined(__GNUC__)
#  include <tr1/unordered_set>
using std::tr1::unordered_set;
#else
# include <unordered_set>
using std::unordered_set;
#endif
