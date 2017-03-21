#define BOOST_TEST_MODULE hash_tests

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>


#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>

#include "hashes.h"
#include "bits.h"

BOOST_AUTO_TEST_SUITE( hash_tests )

BOOST_AUTO_TEST_CASE( bit_fn_tests )
{
   unsigned short uShortInt1( 6 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(uShortInt1)>( uShortInt1 ).to_string(), "0000000000000110" );

   uShortInt1 = bits::bit_rotate_lt( uShortInt1, 3 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(uShortInt1)>( uShortInt1 ).to_string(), "0000000000110000" );

   uShortInt1 = bits::bit_rotate_rt( uShortInt1, 6 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(uShortInt1)>( uShortInt1 ).to_string(), "1100000000000000" );

   unsigned short uShortInt2( 127 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(uShortInt2)>( uShortInt2 ).to_string(), "0000000001111111" );

   unsigned short keepRight3 = bits::keepRightN( uShortInt2, 3 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(keepRight3)>( keepRight3 ).to_string(), "0000000000000111" );

   unsigned short keepLeft10 = bits::keepLeftN( uShortInt2, 10 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(keepLeft10)>( keepLeft10 ).to_string(), "0000000001000000" );

}


// BOOST_AUTO_TEST_CASE( hex_fns )
// {
//    std::uint32_t int1( 37 );

//    BOOST_CHECK_EQUAL( std::bitset<bits::binLength(int1)>( int1 ).to_string(),
//                       "00000000000000000000000000100101"                      );
//    bits::keepByteX( int1, 6 );
//    BOOST_CHECK_EQUAL( std::bitset<bits::binLength(int1)>( int1 ).to_string(),
//                       "00000000000000000000000000100000"                      );

// }


BOOST_AUTO_TEST_CASE( maj_ch_sigma_fns )
{
   uint32_t int1( 6 );

   uint32_t Sig0 = hashes::Sigma0< hashes::SHA256 >( int1 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(Sig0)>( Sig0 ).to_string(), "10000000001100000001100000000001" );
   uint32_t Sig1 = hashes::Sigma1< hashes::SHA256 >( int1 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(Sig1)>( Sig1 ).to_string(), "00011000110000000000001100000000" );
   uint32_t sig0 = hashes::sigma0< hashes::SHA256 >( int1 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(sig0)>( sig0 ).to_string(), "00001100000000011000000000000000" );
   uint32_t sig1 = hashes::sigma1< hashes::SHA256 >( int1 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(sig1)>( sig1 ).to_string(), "00000000000000111100000000000000" );

   unsigned short uShortInt1 = 6;
   unsigned short uShortInt2( 10 );
   unsigned short uShortInt3( 43 );

   unsigned short chRslt = hashes::Ch( uShortInt1, uShortInt2, uShortInt3 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(chRslt)>( chRslt ).to_string(), "0000000000101011" );

   unsigned short majRslt = hashes::Maj( uShortInt1, uShortInt2, uShortInt3 );
   BOOST_CHECK_EQUAL( std::bitset<bits::binLength(majRslt)>( majRslt ).to_string(), "0000000000001010" );
}

BOOST_AUTO_TEST_CASE( hash_fns )
{

   std::string empty_strg( "" );
   std::string  zero_strg( "0" );
   std::string   one_strg( "1" );
   std::string   two_strg( "2" );
   std::string strgToHash( "Once Upon A Time is a rather interesting show.  "   // 48
                           "I hope I will not get hooked on it, because I don't "  // 52
                           "really have the time for another show." );  // 38

   BOOST_CHECK_EQUAL( "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
                      hashes::hashStrg<hashes::SHA256>( empty_strg ) );
   BOOST_CHECK_EQUAL( "5feceb66ffc86f38d952786c6d696c79c2dbc239dd4e91b46729d73a27fb57e9",
                      hashes::hashStrg<hashes::SHA256>( zero_strg ) );
   BOOST_CHECK_EQUAL( "6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b",
                      hashes::hashStrg<hashes::SHA256>( one_strg ) );
   BOOST_CHECK_EQUAL( "d4735e3a265e16eee03f59718b9b5d03019c07d8b6c51f90da3a666eec13ab35",
                      hashes::hashStrg<hashes::SHA256>( two_strg ) );

   BOOST_CHECK_EQUAL( "152b616fae41298a64df8d248edbd2a8cdf01384c5b2440a360a499a35f92690",
                      hashes::hashStrg<hashes::SHA256>( strgToHash ) );


//   SHA224("")
//   0x d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f
//   SHA256("")
//   0x e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
//   SHA384("")
//   0x 38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b
//   SHA512("")
//   0x cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e
//   SHA512/224("")
//   0x 6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4
//   SHA512/256("")
//   0x c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a



}

BOOST_AUTO_TEST_SUITE_END()
