#ifndef HDQRT_HASH_SHA384_H_
#define HDQRT_HASH_SHA384_H_

#include <cstdint>
#include <array>

#include "SHA512.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA384 : public SHA512
{
   typedef SHA512 family;

   typedef typename SHA512::word_t word_t;
   static constexpr std::uint_fast16_t nb_of_sha_vars = SHA256::nb_of_sha_vars;

   static constexpr uint_fast16_t rounds = SHA512::rounds;
   static constexpr uint_fast32_t chunk_size = SHA512::chunk_size;
   static constexpr std::uint_fast16_t len_encode_len = SHA512::len_encode_len;

   static constexpr std::array< word_t, 8 > initHashVals = { {
                            0xcbbb9d5dc1059ed8, 0x629a292a367cd507,
                            0x9159015a3070dd17, 0x152fecd8f70e5939,
                            0x67332667ffc00b31, 0x8eb44a8768581511,
                            0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4  } };

   static constexpr std::array< word_t, 80 > K = SHA512::K;

   static constexpr std::uint_fast16_t digest_len = 384;
};

constexpr std::array< typename SHA384::word_t, 8 > SHA384::initHashVals;
constexpr std::array< typename SHA384::word_t, SHA384::rounds > SHA384::K;


} // namespace hashes

#include "SHA384.inl"

#endif // HDQRT_HASH_SHA384_H_
