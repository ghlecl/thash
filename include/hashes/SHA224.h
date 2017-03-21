#ifndef HDQRT_HASH_SHA224_H_
#define HDQRT_HASH_SHA224_H_

#include <cstdint>
#include <array>

#include "SHA256.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA224 : public SHA256
{
   typedef SHA256 family;

   typedef typename SHA256::word_t word_t;
   static constexpr std::uint_fast16_t nb_of_sha_vars = SHA256::nb_of_sha_vars;

   static constexpr uint_fast16_t rounds = SHA256::rounds;
   static constexpr uint_fast32_t chunk_size = SHA256::chunk_size;
   static constexpr std::uint_fast16_t len_encode_len = 64;

   static constexpr std::uint_fast16_t digest_len = 224;
};

} // namespace hashes

#include "SHA224.inl"

#endif // HDQRT_HASH_SHA224_H_
