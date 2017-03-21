#ifndef HDQRT_HASH_HASHBASE_H_
#define HDQRT_HASH_HASHBASE_H_

#include <string>
#include <cstddef> // for std::size_t

namespace hashes
{

//------------------------------------------------------------------------------
/*!
 *  @brief Empty base class for all hash algorithms.
 *
 *  A hash algorithm must define the following
 *    - word_t
 */
struct HashBase
{};


template< typename Algo >
void padLastChunk( std::string& msg, std::size_t origMsgLen );


} // namespace hashes


#endif // HDQRT_HASH_HASHBASE_H_
