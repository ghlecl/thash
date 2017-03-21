#ifndef HDQRT_HASH_SHA512_224_H_
#define HDQRT_HASH_SHA512_224_H_

#include <cstdint>
#include <array>

#include "SHA512.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA512_224 : public SHA512
{
   typedef SHA512 family;

   typedef typename SHA512::word_t word_t;
   static constexpr std::uint_fast16_t nb_of_sha_vars = SHA512::nb_of_sha_vars;
   static constexpr uint_fast16_t rounds = SHA512::rounds;
   static constexpr uint_fast32_t chunk_size = SHA512::chunk_size;
   static constexpr std::uint_fast16_t len_encode_len = SHA512::len_encode_len;
   static constexpr std::uint_fast16_t digest_len = 224;
};
//constexpr std::array< typename SHA512_224::word_t, 8 > SHA512_224::initHashVals;
//constexpr std::array< typename SHA512_224::word_t, SHA512_224::rounds > SHA512_224::K;

} // namespace hashes

#include "SHA512_224.inl"

#endif // HDQRT_HASH_SHA512_224_H_
