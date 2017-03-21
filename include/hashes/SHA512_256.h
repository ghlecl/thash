#ifndef HDQRT_HASH_SHA512_256_H_
#define HDQRT_HASH_SHA512_256_H_

#include <cstdint>
#include <array>

#include "SHA512.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA512_256 : public SHA512
{
   typedef SHA512 family;

   typedef typename SHA512::word_t word_t;
   static constexpr std::uint_fast16_t nb_of_sha_vars = SHA512::nb_of_sha_vars;
   static constexpr uint_fast16_t rounds = SHA512::rounds;
   static constexpr uint_fast32_t chunk_size = SHA512::chunk_size;
   static constexpr std::uint_fast16_t len_encode_len = SHA512::len_encode_len;
   static constexpr std::uint_fast16_t digest_len = 256;
};
//constexpr std::array< typename SHA512_256::word_t, 8 > SHA512_256::initHashVals;
//constexpr std::array< typename SHA512_256::word_t, SHA512_256::rounds > SHA512_256::K;


} // namespace hashes

#include "SHA512_256.inl"

#endif // HDQRT_HASH_SHA512_256_H_
