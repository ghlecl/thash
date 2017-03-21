#ifndef HDQRT_HASH_SHA256_H_
#define HDQRT_HASH_SHA256_H_

#include <cstdint>
#include <array>

#include "SHA2.h"

namespace hashes
{

//------------------------------------------------------------------------------
struct SHA256 : public SHA2
{
   typedef SHA256 family;
   typedef std::uint32_t word_t;
   static constexpr std::uint_fast16_t nb_of_sha_vars = SHA2::nb_of_sha_vars;
   typedef std::array< word_t, nb_of_sha_vars > hash_type;

   static constexpr uint_fast16_t rounds = 64;
   static constexpr uint_fast32_t chunk_size = 512;
   static constexpr std::uint_fast16_t len_encode_len = 64;

   static constexpr std::array< word_t, 8 > initHashVals = { {
                 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  } };

   static constexpr std::array< word_t, rounds > K = { {
                  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2  } };

   static constexpr std::uint_fast16_t digest_len = 256;
};
// Required definitions outside class for odr-used value
// http://stackoverflow.com/questions/25830111/constexpr-static-data-member-giving-undefined-reference-error
// https://groups.google.com/a/isocpp.org/forum/#!topic/std-discussion/dFsEG9u6XWk
constexpr std::array< typename SHA256::word_t, 8 > SHA256::initHashVals;
constexpr std::array< typename SHA256::word_t, SHA256::rounds > SHA256::K;

} // namespace hashes

#include "SHA256.inl"

#endif // HDQRT_HASH_SHA256_H_
