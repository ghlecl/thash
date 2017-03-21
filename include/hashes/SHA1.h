#ifndef HDQRT_HASH_SHA1_H_
#define HDQRT_HASH_SHA1_H_

#include <cstdint>
#include <array>

#include "HashBase.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA1 : public HashBase
{
   typedef SHA1 family;

   typedef std::uint32_t word_t;
   static constexpr std::uint_fast16_t nb_of_work_vars = 5;

   static constexpr uint_fast16_t rounds = 80;
   static constexpr uint_fast32_t chunk_size = 512;
   static constexpr std::uint_fast16_t len_encode_len = 64;

   static constexpr std::array< word_t, 5 > initHashVals = { {
                  0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 } };

   static constexpr std::array< word_t, 4 > K = { {
                  0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 } };

   static constexpr std::uint_fast16_t digest_len = 160;
};


} // namespace hashes

#endif // HDQRT_HASH_SHA1_H_
