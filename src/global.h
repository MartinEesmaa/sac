#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include "libsac/span.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define NDEBUG

typedef std::vector<double> vec1D;
typedef std::vector<std::vector<double>> vec2D;
typedef std::vector <double*>ptr_vec1D;
typedef span<int32_t> span_i32;
typedef span<const int32_t> span_ci32;
typedef span<const double> span_f64;


#if defined(__x86_64__) || defined(__i386__)
    #ifndef __ANDROID__
        #if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
        #define USE_AVX256
    #else
        #undef USE_AVX256
    #endif
#else
    #undef USE_AVX256
#endif

//#define USE_AVX512

#endif
#endif
