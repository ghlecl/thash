#ifndef HDQRT_HASHES_HASHES_H_
#define HDQRT_HASHES_HASHES_H_

#include <cstdint>
#include <limits>
#include <array>
#include <string>
#include <type_traits>

#include <chrono>
#include <iostream>

#include "bits.h"
#include "hashes/hash_list.h"

namespace hashes
{


//------------------------------------------------------------------------------
template< typename Algo >
struct Hash
{
   typedef typename Algo::word_t word_t;
   typedef typename std::array< word_t, Algo::nb_of_sha_vars > hash_type;
   hash_type state;
};



template< typename Algo >
void addBigEndianRep( std::string& msg, std::uint64_t& len );

template< typename Algo >
void padLastChunk( std::string& msg, std::size_t origMsgLen );

template<typename Algo>
void initializeHash( Hash<Algo>& theHash );

template< typename Algo >
void applyRounds( typename Hash<Algo>::hash_type& theHash,
                  std::array< typename Algo::word_t, Algo::rounds >  const& W );

template< typename Algo >
void processChunk( typename Hash<Algo>::hash_type& theHash, std::string const& chunk );


template< typename Algo >
typename std::enable_if< std::is_base_of< HashBase, Algo >::value, std::string >::type
getDigest( Hash<Algo>& theHash );

template <typename Algo>
std::string hashStrg( std::string const& input );

} // namespace hashes

#include "hashes.inl"

#endif // HDQRT_HASH_HASHES_H_







//   //

//   if( zerosToAdd < 7 )
//   {

//      auto bitsToKeep( msgElemBitSize - zerosToAdd + 1 );
//      auto newLast( bits::keepLeftN( msg.back(), bitsToKeep ) );
//      newLast = bits::setNthBit( newLast, zerosToAdd + 1 );
//      msg.back() = newLast;
//   }


