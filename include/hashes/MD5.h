#ifndef HDQRT_HASH_MD5_H_
#define HDQRT_HASH_MD5_H_

#include <cstdint>
#include <array>

#include "HashBase.h"

namespace hashes
{

struct MD5 : public HashBase
{
   typedef MD5 family;
   typedef std::uint32_t word_t;
   static constexpr std::uint_fast16_t nb_of_work_vars = 4;

   static constexpr std::uint_fast16_t rounds = 64;
   static constexpr std::uint_fast32_t chunk_size = 512;
   static constexpr std::uint_fast16_t len_encode_len = 64;

   static constexpr std::array< word_t, 4 > initHashVals = { {
                  0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 } };

   static constexpr std::array< word_t, 64 > K = { {
                   0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                   0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                   0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                   0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                   0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                   0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                   0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                   0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                   0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                   0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 } };

   static constexpr std::uint_fast16_t digest_len = 128;
};


} // namespace hashes

#include "MD5.inl"

#endif // HDQRT_HASH_MD5_H_
