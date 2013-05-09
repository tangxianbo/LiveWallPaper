#pragma once
#include <stdio.h>


#if defined(_WIN32) || defined(ANDROID) || defined(__linux__)
    
    inline size_t HASH_FUNCTION(size_t seed, char ch)
    {
        return seed ^ (ch + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }
    #define HASH_RECURSE_00(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_01(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_01(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_02(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_02(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_03(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_03(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_04(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_04(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_05(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_05(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_06(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_06(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_07(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_07(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_08(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_08(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_09(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_09(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_10(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_10(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_11(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_11(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_12(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_12(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_13(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_13(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_14(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_14(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_15(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_15(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_16(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_16(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_17(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_17(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_18(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_18(seed, str) (*(str + 1) == 0 ? HASH_FUNCTION((seed), *(str)) : HASH_RECURSE_19(HASH_FUNCTION((seed), *(str)), (str + 1)))
    #define HASH_RECURSE_19(seed, str) HASH_FUNCTION((seed), *(str))

    #define CTHASH(str) HASH_RECURSE_00(0, (str))
#else
    constexpr size_t _Hash(const char (&str)[1])
    {
        return *str + 0x9e3779b9;
    }

    template <size_t N>
    constexpr size_t _Hash(const char (&str)[N])
    {
        typedef const char (&truncated_str)[N - 1];
        #define seed _Hash((truncated_str)str)
        return seed ^ (*(str + N - 1) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        #undef seed
    }

    template <size_t N>
    constexpr size_t _CTHash(const char (&str)[N])
    {
        typedef const char (&truncated_str)[N - 1];
        return _Hash<N - 1>((truncated_str)str);
    }

    #define CTHASH(str) _CTHash(str)
#endif