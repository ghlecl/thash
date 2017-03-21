#include "bits.h"

#include <iostream>
#include <algorithm>

#include "always_inline.h"



namespace bits
{

//------------------------------------------------------------------------------
/*!
 *  @brief Get system endianness.  Not exhaustive.
 *
 *  Supports only LITTLE_ENDIAN and BIG_ENDIAN.  MIXED_ENDIAN systems will
 *  result in unknown.
 *
 */
ALWAYS_INLINE Endianness endianness()
{
   std::uint32_t one( 1 );
   // (first == 1)   ? LITTLE : ( ( last == 1 ) ? BIG : UNKNOWN )
   return ( (reinterpret_cast<char*>( &one ))[0] == 1 ) ? Endianness::LITTLE :
          ( ( (reinterpret_cast<char*>( &one ))[sizeof( std::uint32_t )] == 1 ) ?
                           Endianness::BIG : Endianness::UNKNOWN );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Output endianness to a stream as a string.
 *
 *  Outputs one of the following :
 *     - LITTLE_ENDIAN
 *     - BIG_ENDIAN
 *     - UNKNOWN_ENDIAN
 *
 */
ALWAYS_INLINE std::ostream&
operator << ( std::ostream& ostrm, Endianness endianness )
{
   // Endianness::LITTLE ? "LITTLE_ENDIAN" : Endianness::BIG ? "BIG_ENDIAN" : "UNKNOWN_ENDIAN" )
   return ostrm << ( ( endianness == Endianness::LITTLE ) ? "LITTLE_ENDIAN" :
              ( ( endianness == Endianness::BIG ) ? "BIG_ENDIAN" : "UNKNOWN_ENDIAN" ) );
}



//------------------------------------------------------------------------------
/*!
 *  @brief
 *
 *  The first input is the most significant, the last is the least significant.
 *  As an example, with the following input (10, 101, 148, 32), the operation is
 *  equivalent to :
 *      - x1 =  10 => 00001010
 *      - x2 = 101 => 01100101
 *      - x3 = 148 => 10010100
 *      - x4 =  32 => 00100000
 *      - concatenation 00001010011001011001010000100000 (hex : 0x0A659420)
 *      - assign to uint32_t and return
 *  This is equivalent to uint32_t( 0x0A659420 ).  So no matter what
 *  architecture of computer, the result is that x1 becomes the most
 *  significant byte.
 */
ALWAYS_INLINE std::uint32_t pack( std::uint8_t const& x1, std::uint8_t const& x2,
                                  std::uint8_t const& x3, std::uint8_t const& x4 )
{
   return ( ( ( 0x000000FF & x1 ) << 24 ) | ( ( 0x000000FF & x2 ) << 16 ) |
                       ( ( 0x000000FF & x3 ) << 8 ) | ( 0x000000FF & x4 ) );
}



//------------------------------------------------------------------------------
/*!
 *  @brief
 */
ALWAYS_INLINE std::uint32_t pack( std::uint16_t x1, std::uint16_t x2 )
{
   return ( ( 0x0000FFFF & x1 ) << 16 | ( 0x0000FFFF & x2 )         );
}



//------------------------------------------------------------------------------
template<>
ALWAYS_INLINE std::array<std::uint8_t, 1> unpack<std::uint8_t>( std::uint8_t nb )
{
   std::array<std::uint8_t, 1> pack;
   pack[0] = nb;
   return pack;
}



//------------------------------------------------------------------------------
template<>
ALWAYS_INLINE std::array<std::uint8_t, 2> unpack<std::uint16_t>( std::uint16_t nb )
{
   std::array<std::uint8_t, 2> pack;
   pack[0] = ( ( nb & 0xff00 ) >> 8 );
   pack[1] =   ( nb & 0x00ff );
   return pack;
}


template<>
ALWAYS_INLINE std::array<std::uint8_t, 4> unpack<std::uint32_t>( std::uint32_t nb )
{
   std::array<std::uint8_t, 4> pack;
   pack[0] = ( ( nb & 0xff000000 ) >> 24 );
   pack[1] = ( ( nb & 0x00ff0000 ) >> 16 );
   pack[2] = ( ( nb & 0x0000ff00 ) >>  8 );
   pack[3] =   ( nb & 0x000000ff );
   return pack;
}


template<>
ALWAYS_INLINE std::array<std::uint8_t, 8> unpack<std::uint64_t>( std::uint64_t nb )
{
   std::array<std::uint8_t, 8> pack;
   pack[0] = ( ( nb & 0xff00000000000000 ) >> 56 );
   pack[1] = ( ( nb & 0x00ff000000000000 ) >> 48 );
   pack[2] = ( ( nb & 0x0000ff0000000000 ) >> 40 );
   pack[3] = ( ( nb & 0x000000ff00000000 ) >> 32 );
   pack[4] = ( ( nb & 0x00000000ff000000 ) >> 24 );
   pack[5] = ( ( nb & 0x0000000000ff0000 ) >> 16 );
   pack[6] = ( ( nb & 0x000000000000ff00 ) >>  8 );
   pack[7] =   ( nb & 0x00000000000000ff );
   return pack;
}


//------------------------------------------------------------------------------
/*!
 *  @brief Swap bytes of an integral variable.
 *
 *  Goes from LITTLE_ENDIAN to BIG_ENDIAN and vice-versa, but does not provide
 *  any support for MIXED_ENDIAN.
 */
template< typename T >
ALWAYS_INLINE typename std::enable_if< std::is_integral<T>::value, T >::type
byte_swap( T const& x )
{
   unsigned char *memp = reinterpret_cast<unsigned char*>(x);
   std::reverse(memp, memp + sizeof(T));
}



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization for 16 bit integers.
 */
template<>
ALWAYS_INLINE typename std::enable_if< std::is_integral<std::uint16_t>::value, std::uint16_t >::type
byte_swap<std::uint16_t>( std::uint16_t const& x )
{
    return ( (x >> 8) |
             (x << 8)   );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization for 32 bit integers.
 */
template<>
ALWAYS_INLINE typename std::enable_if< std::is_integral<std::uint16_t>::value, std::uint32_t >::type
byte_swap<std::uint32_t>( std::uint32_t const& x )
{
    return ( ( x >> 24 )                 |
             ( ( x << 8 ) & 0x00FF0000 ) |
             ( ( x >> 8 ) & 0x0000FF00 ) |
             ( x << 24 )                   );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization for 64 bit integers.
 */
template<>
ALWAYS_INLINE typename std::enable_if< std::is_integral<std::uint64_t>::value, std::uint64_t >::type
byte_swap<std::uint64_t>( std::uint64_t const& x )
{
    return ( ( x >> 56 )                          |
             ( ( x << 40 ) & 0x00FF000000000000 ) |
             ( ( x << 24 ) & 0x0000FF0000000000 ) |
             ( ( x <<  8 ) & 0x000000FF00000000 ) |
             ( ( x >>  8 ) & 0x00000000FF000000 ) |
             ( ( x >> 24 ) & 0x0000000000FF0000 ) |
             ( ( x >> 40 ) & 0x000000000000FF00 ) |
             ( x << 56 )                            );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Rotates (right) bits of integer type variable the required number of
 *         moves.
 */
template< typename IntType >
ALWAYS_INLINE IntType
bit_rotate_lt( IntType const& val, std::uint_fast16_t moves )
{
   static_assert( std::is_integral< IntType >::value,
                  "bit_rotate not defined for non-integral types" );
   std::uint16_t sizeInBits( sizeof( IntType ) * CHAR_BIT );
   return ( val << moves ) | ( val >> ( sizeInBits - moves ) );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Rotates (left) bits of integer type variable the required number of
 *         moves.
 */
template< typename IntType >
ALWAYS_INLINE IntType
bit_rotate_rt( IntType const& val, std::uint_fast16_t moves )
{
   static_assert( std::is_integral< IntType >::value,
                  "bit_rotate not defined for non-integral types" );
   std::uint16_t sizeInBits = sizeof( IntType ) * CHAR_BIT;
   return ( val >> moves ) | ( val << ( sizeInBits - moves ) );
}



//------------------------------------------------------------------------------
/*!
 *  @brief The length in bits of an integer variable.
 */
template< typename IntType >
ALWAYS_INLINE constexpr IntType binLength( IntType const& )
{
   return binLength<IntType>();
}



//------------------------------------------------------------------------------
/*!
 *  @brief The length in bits of an integer type.
 */
template< typename IntType >
ALWAYS_INLINE constexpr IntType binLength()
{
   return sizeof(IntType) * CHAR_BIT;
}



//------------------------------------------------------------------------------
/*!
 *  @brief Keep the left most N bits and set every other bit to 0.
 */
template< typename IntType >
ALWAYS_INLINE IntType keepLeftN( IntType const& val, std::uint_fast16_t keep )
{
   std::uint_fast16_t shift = binLength( val ) - keep;
   return IntType( val >> shift ) << shift;
}



//------------------------------------------------------------------------------
/*!
 *  @brief Keep the right most N bits and set every other bit to 0.
 */
template< typename IntType >
ALWAYS_INLINE IntType keepRightN( IntType const& val, std::uint_fast16_t keep )
{
   std::uint_fast16_t shift = binLength( val ) - keep;
   return IntType( val << shift ) >> shift;
}



//------------------------------------------------------------------------------
/*!
 *  @brief Set nth bit to 1.
 */
template< typename IntType >
ALWAYS_INLINE constexpr typename
std::enable_if< std::is_integral<IntType>::value, IntType >::type
setNthBit( IntType const& val, std::uint_fast16_t n )
{
   return val | ( 1 << n );
}



//------------------------------------------------------------------------------
/*!
 *  Input should be LITTLE_ENDIAN.  Output is BIG_ENDIAN so as to be human
 *  readable and does NOT have the 0x prefix.
 */
template<>
inline std::string to_hex<std::uint8_t>( std::uint8_t nb )
{
   std::string strg( "00" );
   strg[0] = hex_digits[ (nb & 0xf0) >> 4 ];
   strg[1] = hex_digits[ (nb & 0x0f) ];
   return strg;
}



//------------------------------------------------------------------------------
/*!
 *  Input should be LITTLE_ENDIAN.  Output is BIG_ENDIAN so as to be human
 *  readable and does NOT have the 0x prefix.
 */
template<>
inline std::string to_hex<std::uint16_t>( std::uint16_t nb )
{
   std::string strg( "0000" );
   strg[0] = hex_digits[ (nb & 0xf000) >> 12 ];
   strg[1] = hex_digits[ (nb & 0x0f00) >>  8 ];
   strg[2] = hex_digits[ (nb & 0x00f0) >>  4 ];
   strg[3] = hex_digits[ (nb & 0x000f) ];
   return strg;
}



//------------------------------------------------------------------------------
/*!
 *  Input should be LITTLE_ENDIAN.  Output is BIG_ENDIAN so as to be human
 *  readable and does NOT have the 0x prefix.
 */
template<>
inline std::string to_hex<std::uint32_t>( std::uint32_t nb )
{
   std::string strg( "00000000" );
   strg[0] = hex_digits[ (nb & 0xf0000000) >> 28 ];
   strg[1] = hex_digits[ (nb & 0x0f000000) >> 24 ];
   strg[2] = hex_digits[ (nb & 0x00f00000) >> 20 ];
   strg[3] = hex_digits[ (nb & 0x000f0000) >> 16 ];
   strg[4] = hex_digits[ (nb & 0x0000f000) >> 12 ];
   strg[5] = hex_digits[ (nb & 0x00000f00) >>  8 ];
   strg[6] = hex_digits[ (nb & 0x000000f0) >>  4 ];
   strg[7] = hex_digits[ (nb & 0x0000000f) ];
   return strg;
}



//------------------------------------------------------------------------------
/*!
 *  Input should be LITTLE_ENDIAN.  Output is BIG_ENDIAN so as to be human
 *  readable and does NOT have the 0x prefix.
 */
template<>
inline std::string to_hex<std::uint64_t>( std::uint64_t nb )
{
   std::string strg( "0000000000000000" );
   strg[0] =  hex_digits[ (nb & 0xf000000000000000) >> 60 ];
   strg[1] =  hex_digits[ (nb & 0x0f00000000000000) >> 56 ];
   strg[2] =  hex_digits[ (nb & 0x00f0000000000000) >> 52 ];
   strg[3] =  hex_digits[ (nb & 0x000f000000000000) >> 48 ];
   strg[4] =  hex_digits[ (nb & 0x0000f00000000000) >> 44 ];
   strg[5] =  hex_digits[ (nb & 0x00000f0000000000) >> 40 ];
   strg[6] =  hex_digits[ (nb & 0x000000f000000000) >> 36 ];
   strg[7] =  hex_digits[ (nb & 0x0000000f00000000) >> 32];
   strg[8] =  hex_digits[ (nb & 0x00000000f0000000) >> 28 ];
   strg[9] =  hex_digits[ (nb & 0x000000000f000000) >> 24 ];
   strg[10] = hex_digits[ (nb & 0x0000000000f00000) >> 20 ];
   strg[11] = hex_digits[ (nb & 0x00000000000f0000) >> 16 ];
   strg[12] = hex_digits[ (nb & 0x000000000000f000) >> 12 ];
   strg[13] = hex_digits[ (nb & 0x0000000000000f00) >>  8 ];
   strg[14] = hex_digits[ (nb & 0x00000000000000f0) >>  4 ];
   strg[15] = hex_digits[ (nb & 0x000000000000000f) ];
   return strg;
}

} // namespace bits
