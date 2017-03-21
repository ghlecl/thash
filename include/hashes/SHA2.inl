#include "../always_inline.h"

namespace hashes
{


//------------------------------------------------------------------------------
/*!
 *  @brief Ch function of SHA2 family of hashes.
 */
template< typename IntType >
ALWAYS_INLINE IntType Ch( IntType x, IntType y, IntType z )
{
   static_assert( std::is_integral< IntType >::value,
                  "Ch not defined for non-integral types" );
   return ( x & y ) ^ ( (~x) & z );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Maj function of SHA2 family of hashes.
 */
template< typename IntType >
ALWAYS_INLINE IntType Maj( IntType x, IntType y, IntType z )
{
   static_assert( std::is_integral< IntType >::value,
                  "Maj not defined for non-integral types" );
   // Associativity is left to right
   return ( x & y ) ^ ( x & z ) ^ ( y & z );
}


namespace details
{

//------------------------------------------------------------------------------
/*!
 *  @brief Empty class.  Sigma function dispatching utility.
 *
 */
struct Zero
{};



//------------------------------------------------------------------------------
/*!
 *  @brief Empty class.  Sigma function dispatching utility.
 *
 */
struct One
{};



//------------------------------------------------------------------------------
/*!
 *  @brief Empty class.  Sigma function dispatching utility.
 *
 */
struct Min
{};



//------------------------------------------------------------------------------
/*!
 *  @brief Empty class.  Sigma function dispatching utility.
 *
 */
struct Cap
{};


//------------------------------------------------------------------------------
/*!
 *  @brief Rotate/shift values to use in SHA2 family sigma functions.
 *
 *  Defined only for types deriving from SHA2 (empty) class.
 *
 *  Only contains a static std::array< std::uint_fast8_t, 3 > array with the
 *  three indexes needed for the SHA2 sigma functions.
 *
 */
template< typename SHA2Hash, typename MinOrCap, typename ZeroOrOne >
struct sigma_fn_params
{
   static_assert( is_sha2_derived< SHA2Hash >::value,
                  "SHA2Hash type parameter must be derived from SHA2" );
   static constexpr std::array< std::uint_fast8_t, 3 > params =
         sigma_fn_params< typename SHA2Hash::family, MinOrCap, ZeroOrOne  >::params;
};

// Required definitions outside class for odr-used value
// http://stackoverflow.com/questions/25830111/constexpr-static-data-member-giving-undefined-reference-error
// https://groups.google.com/a/isocpp.org/forum/#!topic/std-discussion/dFsEG9u6XWk
template< typename SHA2Hash, typename MinOrCap, typename ZeroOrOne >
constexpr std::array< std::uint_fast8_t, 3 >
sigma_fn_params<SHA2Hash, MinOrCap, ZeroOrOne>::params;



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA256, Cap, Zero >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 2, 13, 22 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA256, Cap, One >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 6, 11, 25 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA256, Min, Zero >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 7, 18, 3 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA256, Min, One >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 17, 19, 10 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA512, Cap, Zero >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 28, 34, 39 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA512, Cap, One >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 14, 18, 41 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA512, Min, Zero >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 1, 8, 7 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Template specialization.
 */
template<>
struct sigma_fn_params< SHA512, Min, One >
{
   static constexpr std::array< std::uint_fast8_t, 3 > params = {{ 19, 61, 6 }};
};



//------------------------------------------------------------------------------
/*!
 *  @brief Last step of SHA2 sigma function for "capital" sigma (0 or 1).
 *
 *  Used for tag dispatching to fake partial function template specialization.
 *  Compiler should remove the unused empty object of the empty class when
 *  called.
 */
template< typename T >
ALWAYS_INLINE T do_sigma( T x, std::uint_fast8_t val, Cap )
{ return bits::bit_rotate_rt( x, val ); }



//------------------------------------------------------------------------------
/*!
 *  @brief Last step of SHA2 sigma function for "minuscule" sigma (0 or 1).
 *
 *  Used for tag dispatching to fake partial function template specialization.
 *  Compiler should remove the unused empty object of the empty class when
 *  called.
 */
template< typename T >
ALWAYS_INLINE T do_sigma( T x, std::uint_fast8_t val, Min )
{ return x >> val; }



//------------------------------------------------------------------------------
/*!
 *  @brief Last step of SHA2 sigma function.
 */
template< typename Algo, typename MinOrCap >
ALWAYS_INLINE typename Algo::word_t
inner_sigma( typename Algo::word_t x, std::uint_fast8_t val )
{
   typedef typename Algo::word_t word_t;
   return do_sigma< word_t >( x, val, MinOrCap() );
};



//------------------------------------------------------------------------------
/*!
 *  @brief Sigma family of functions described in the sha2 specification.
 *
 *  Needs to be parametrized using the Algorithm enum class and the four Sigma
 *  parameter classes.
 *
 *  This in turn defines the input size (32 bits or 64 bits), whether a rotation
 *  or a shift is used as a last operation (minuscule s sigmas shift, capital s
 *  sigmas rotate) and what offset values to apply when rotating/shifting.
 *
 */
template< typename Algo, typename MinOrCap, typename ZeroOrOne >
ALWAYS_INLINE typename Algo::word_t
sigma( typename Algo::word_t x )
{
   // Get appropriate indexes.  Type must match sha_family_traits::*igma* types.
   std::array< std::uint_fast8_t, 3 > idxs =
                           sigma_fn_params< Algo, MinOrCap, ZeroOrOne >::params;

   // Apply function
   using bits::bit_rotate_rt;
   return bit_rotate_rt( x, idxs[0] ) ^
          bit_rotate_rt( x, idxs[1] ) ^
          inner_sigma<Algo, MinOrCap>( x, idxs[2] );
}

} // namespace details



//------------------------------------------------------------------------------
/*!
 *  @brief Sigma0 (Capital, zero) of SHA2 family of hashes.
 */
template< typename Algo >
ALWAYS_INLINE typename Algo::word_t
Sigma0( typename Algo::word_t x )
{
   return details::sigma<Algo, details::Cap, details::Zero>( x );
}



//------------------------------------------------------------------------------
/*!
 *  @brief sigma0 (capital, zero) of SHA2 family of hashes.
 */
template< typename Algo >
ALWAYS_INLINE typename Algo::word_t
sigma0( typename Algo::word_t x )
{
   return details::sigma<Algo, details::Min, details::Zero>( x );
}



//------------------------------------------------------------------------------
/*!
 *  @brief Sigma0 (Capital, zero) of SHA2 family of hashes.
 */
template< typename Algo >
ALWAYS_INLINE typename Algo::word_t
Sigma1( typename Algo::word_t x )
{
   return details::sigma<Algo, details::Cap, details::One>( x );
}



//------------------------------------------------------------------------------
/*!
 *  @brief sigma1 (capital, zero) of SHA2 family of hashes.
 */
template< typename Algo >
ALWAYS_INLINE typename Algo::word_t
sigma1( typename Algo::word_t x )
{
   return details::sigma<Algo, details::Min, details::One>( x );
}

} // namespace hashes
