#ifndef HDQRT_HASH_SHA2_H_
#define HDQRT_HASH_SHA2_H_

#include <cstdint>

namespace hashes
{


//------------------------------------------------------------------------------
struct SHA2 : public HashBase
{
   static constexpr std::uint_fast16_t nb_of_sha_vars = 8;
};

struct SHA256;
struct SHA512;

template< typename IntType >
IntType Ch( IntType x, IntType y, IntType z );

template< typename IntType >
IntType Maj( IntType x, IntType y, IntType z );


//------------------------------------------------------------------------------
template< typename Hash >
using is_sha2 = std::integral_constant< bool, std::is_base_of< SHA2, Hash >::value >;

template< typename Hash >
using is_sha2_derived = std::integral_constant< bool,
                                                ( ( is_sha2< Hash >::value ) &&
                                                  ( std::is_same< SHA2, Hash >::value ) ) >;


template< typename Algo >
typename Algo::word_t Sigma0( typename Algo::word_t x );

template< typename Algo >
typename Algo::word_t sigma0( typename Algo::word_t x );

template< typename Algo >
typename Algo::word_t Sigma1( typename Algo::word_t x );

template< typename Algo >
typename Algo::word_t Sigma1( typename Algo::word_t x );


}

#include "SHA2.inl"

#endif // HDQRT_HASH_SHA2_H_
