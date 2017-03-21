#ifndef HDQRT_GENERAL_BITS_H_
#define HDQRT_GENERAL_BITS_H_

#include <cstdint>
#include <type_traits>
#include <iostream>


namespace bits
{

constexpr char hex_digits[] = "0123456789abcdef";

//------------------------------------------------------------------------------
/*!
 *  @brief Available system endianness.
 *
 *  Supports only BIG_ENDIAN or LITTLE_ENDIAN.  MIXED_ENDIAN is not supported
 *  at this time.
 */
enum class Endianness : std::uint_fast8_t
{
   LITTLE,
   BIG,
   UNKNOWN
};


//------------------------------------------------------------------------------
Endianness endianness();

//------------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& ostrm, Endianness endianness );

std::uint32_t pack( std::uint8_t const& x1, std::uint8_t const& x2,
                    std::uint8_t const& x3, std::uint8_t const& x4 );

std::uint32_t pack( std::uint16_t x1, std::uint16_t x2 );

template<typename UIntType>
std::array< std::uint8_t, sizeof(UIntType) > unpack( UIntType nb );

template< typename T >
typename std::enable_if< std::is_integral<T>::value, T >::type
byte_swap( T const& x );

template< typename IntType >
IntType bit_rotate_lt( IntType const& val, std::uint_fast16_t moves );


template< typename IntType >
IntType bit_rotate_rt( IntType const& val, std::uint_fast16_t moves );


template< typename IntType >
constexpr IntType binLength( IntType const& );


template< typename IntType >
constexpr IntType binLength();


template< typename IntType >
IntType keepLeftN( IntType const& val, std::uint_fast16_t keep );


template< typename IntType >
IntType keepRightN( IntType const& val, std::uint_fast16_t keep );


template< typename IntType >
constexpr typename
std::enable_if< std::is_integral<IntType>::value, IntType >::type
setNthBit( IntType const& val, std::uint_fast16_t set );


template<typename UIntType>
std::string to_hex( UIntType nb );


} // namespace bits

#include "bits.inl"

#endif // HDQRT_GENERAL_BITS_H_
