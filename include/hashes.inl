
#include <sstream>
#include <cstdio>

//#include "hashes.h"
#include "always_inline.h"

namespace hashes
{

//------------------------------------------------------------------------------
/*!
 *  @brief Add BIG_ENDIAN representation of the input len to the input message.
 */
template< typename Algo >
ALWAYS_INLINE void
addBigEndianRep( std::string& msg, std::uint64_t& len )
{
   // Add the size with most significant bit first, representation

   auto bytes = bits::unpack( len );
   msg.push_back( bytes[0] );
   msg.push_back( bytes[1] );
   msg.push_back( bytes[2] );
   msg.push_back( bytes[3] );
   msg.push_back( bytes[4] );
   msg.push_back( bytes[5] );
   msg.push_back( bytes[6] );
   msg.push_back( bytes[7] );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Overload for SHA512 algorithm.
 *
 *  Current implementation considers that the largest integer type is 64 bits.
 *  Thus, to write a 128 bits representation, 64 zero bits are added before
 *  adding the len representation.
 */
template<>
ALWAYS_INLINE void
addBigEndianRep<SHA512>( std::string& msg, std::uint64_t& len )
{
   auto bytes = bits::unpack( len );
   msg.append( 8, 0x0 );
   msg.push_back( bytes[0] );
   msg.push_back( bytes[1] );
   msg.push_back( bytes[2] );
   msg.push_back( bytes[3] );
   msg.push_back( bytes[4] );
   msg.push_back( bytes[5] );
   msg.push_back( bytes[6] );
   msg.push_back( bytes[7] );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Pad last chunk before calculating final digest.
 *
 *  Default implementation does the following:
 *    - adds 1 to message
 *    - pads message with zeros so that :
 *       Algo::chunk_size == last_chunk_len + Algo::len_encode_len + 1
 *    - appends the BIG_ENDIAN representation of the original length
 *
 *  Throws std::logic_error if system architecture does not appear to be 8-bit
 *  based.
 *
 */
template< typename Algo >
inline void
padLastChunk( std::string& msg, std::uint64_t origMsgLen )
{
   // Guard
   static_assert( CHAR_BIT == 8, "System architecture for hashing support must "
                                 "be 8 bit based." );


   using bits::binLength;
   std::uint64_t msgLenInBits( binLength( msg[0] ) * origMsgLen );

   unsigned int zerosToAdd( Algo::chunk_size - Algo::len_encode_len - 1 -
                                       ( binLength( msg[0] ) * msg.length() ) );

   // Prevent copies for reallocation by doing it in one swoop if necessary
   msg.reserve( origMsgLen + 1 + zerosToAdd +
                           ( bits::binLength<std::uint64_t>() / CHAR_BIT ) );


   // Add the 1 bit plus 7 zeros of padding.  Supposes char == 8 bits
   msg.push_back( ( 1 << 7 ) );
   zerosToAdd -= 7;
   for( ; zerosToAdd != 0; zerosToAdd -=8 ) msg.push_back( 0 );


   // Add the length
   addBigEndianRep<Algo>( msg, msgLenInBits );
}



//------------------------------------------------------------------------------
template<typename Algo>
ALWAYS_INLINE void
initializeHash( Hash<Algo>& theHash )
{
   //typedef std::uint_fast16_t uint_t;
   theHash.state = Algo::initHashVals;
}



//------------------------------------------------------------------------------
/*!
 *  @brief Apply a round for a hashing algorithm.
 *
 *  The default implementation does nothing.  Must be specialized for every
 *  algorithm.
 *
 *  Specialized implementations can (not mandatory, but usually) assume that the
 *  message chunck it gets is of correct bit size.
 *
 */
template< typename Algo >
inline void
applyRounds( typename Hash<Algo>::hash_type& theHash,
            std::array< typename Algo::word_t, Algo::rounds > const& W )
{}



//------------------------------------------------------------------------------
template<>
inline void
applyRounds<SHA256>( typename Hash<SHA256>::hash_type& theHash,
                     std::array< typename SHA256::word_t, SHA256::rounds > const& W )
{
   typedef SHA256 Algo;
   typedef typename Algo::word_t word_t;

   word_t T1( 0 ), T2( 0 );
   // local copies prevent constant dereferencing (performance)
   word_t a( theHash[0] ), b( theHash[1] ),
          c( theHash[2] ), d( theHash[3] ),
          e( theHash[4] ), f( theHash[5] ),
          g( theHash[6] ), h( theHash[7] );

   for( std::uint_fast16_t idx( 0 ); idx != Algo::rounds; ++idx )
   {
      T1 = h + Sigma1<Algo>( e ) + Ch( e, f, g ) + Algo::K[idx] + W[idx];
      T2 = Sigma0<Algo>( a ) + Maj( a, b, c );
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
   }

   // Update the state
   theHash[0] += a;
   theHash[1] += b;
   theHash[2] += c;
   theHash[3] += d;
   theHash[4] += e;
   theHash[5] += f;
   theHash[6] += g;
   theHash[7] += h;
}

template<>
ALWAYS_INLINE void
applyRounds<SHA224>( typename Hash<SHA224>::hash_type& theHash,
                     std::array< typename SHA224::word_t, SHA224::rounds > const& W )
{
   applyRounds<SHA256>( theHash, W );
}




//------------------------------------------------------------------------------
/*!
 *  @brief Apply a round for a hashing algorithm.
 *
 *  The default implementation does nothing.  Must be specialized for every
 *  algorithm.
 *
 *  Specialized implementations can (not mandatory, but usually) assume that the
 *  message chunck it gets is of correct bit size.
 *
 */
template< typename Algo >
inline void processChunk( Hash<Algo>& theHash, std::string const& chunck )
{}



//------------------------------------------------------------------------------
/*!
 *  @brief Apply one round of the algorithm.
 *
 *  Assumes chunk is correct size.
 */
template<>
inline void processChunk<SHA256>( Hash<SHA256>& theHash, std::string const& chunk )
{
   typedef SHA256 Algo;
   typedef typename Algo::word_t word_t;
   typedef std::uint_fast16_t uint_t;
   using bits::pack;
   using bits::byte_swap;

   // Create the 64 word work array W
   std::array< word_t, Algo::rounds > W;

   for( std::size_t wordIdx( 0 ); wordIdx != 16; ++wordIdx )
   {
      auto charIdx = wordIdx * 4;
      W[wordIdx] = pack( chunk[charIdx], chunk[charIdx+1],
                                 chunk[charIdx+2], chunk[charIdx+3]  );
   }

   for( uint_t idx( 16 ); idx != Algo::rounds; ++idx )
   {
      W[idx] = sigma1<Algo>( W[idx - 2] ) + W[idx - 7] +
                                    sigma0<Algo>( W[idx - 15] ) + W[idx - 16];
   }


   // Apply the rounds
   applyRounds<Algo>( theHash.state, W );
}



//------------------------------------------------------------------------------
template< typename Algo >
inline typename std::enable_if< std::is_base_of< HashBase, Algo >::value, std::string >::type
getDigest( Hash<Algo>& theHash )
{
   using bits::Endianness;
   using bits::endianness;
   using bits::byte_swap;
   using bits::to_hex;

   // Create string object to hold digest.  Reserve appropriate length.
   std::string digest( "" );
   digest.reserve( Algo::digest_len );

   for( typename Hash<Algo>::word_t const& cur : theHash.state )
   {
      digest.append( to_hex( cur ) );
   }

   return digest;
}



//------------------------------------------------------------------------------
/*!
 *  @brief Hash a string with the given algorithm.
 */
template <typename Algo>
inline std::string hashStrg( std::string const& input )
{
   // Sanity check
   auto elemBinLength = bits::binLength( input[0] );
   if( Algo::chunk_size % elemBinLength != 0 )
   {
      throw std::logic_error( "Size do not match. byte != 8 bits" );
   }


   Hash<Algo> theHash;
   initializeHash<Algo>( theHash );


   auto chunkStart( input.begin() );
   auto elemsPerChunk = Algo::chunk_size / elemBinLength;
   auto chunkEnd( chunkStart + elemsPerChunk );
   auto origEnd( input.end() );
   while( chunkEnd < origEnd )
   {
      processChunk<Algo>( theHash, std::string( chunkStart, chunkEnd ) );
      chunkStart = chunkEnd;
      chunkEnd += elemsPerChunk;
   }


   auto lastChunk = std::string( chunkStart, input.end() );
   padLastChunk<Algo>( lastChunk, static_cast<std::uint64_t>( input.length() ) );
   processChunk<Algo>( theHash, lastChunk );


   return getDigest<Algo>( theHash );
}

} // namespace hashes
