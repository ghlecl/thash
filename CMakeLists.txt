cmake_minimum_required(VERSION 3.0)

if( POLICY CMP0054 )
   cmake_policy( SET CMP0054 NEW )
endif()

# Some default locations to look for things
if ( ${APPLE} )
   list( APPEND CMAKE_PREFIX_PATH "/sw/local/" )
   list( APPEND CMAKE_PREFIX_PATH "/Users/Shared/" )
elseif ( ${WIN32} )
   list( APPEND CMAKE_PREFIX_PATH "C:/local/"
                                  "C:/local/boost/"
                                  "C:/local/boost_1_55_0/"
                                  "F:/local/boost/x86/"
                                  "F:/local/"
                                  "F:/local/boost"
   )
endif()


project( hash )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )


#-------------------------------------------------------------------------------
# Set the libraries to static
set( BUILD_SHARED_LIBS FALSE )


#------------------------------------------------------------------------------
# - Find and setup Boost C++ librairies for usage
macro( setup_boost )

   if ( NOT DEFINED HASH_BOOST_FOUND OR NOT ${HASH_BOOST_FOUND} )
      # Set min version
      set( Boost_MIN_REQ_VERSION 1.49.0 )

      # Set search parameters
      if ( NOT DEFINED BUILD_SHARED_LIBS OR NOT ${BUILD_SHARED_LIBS} )
         message( STATUS "Setting static library usage for Boost." )
         set( Boost_USE_STATIC_LIBS TRUE )
      else ()
         set( Boost_USE_STATIC_LIBS FALSE )
         set( Boost_USE_STATIC_RUNTIME FALSE )
      endif()

      if ( MSVC )
         set( BOOST_ALL_NO_LIB TRUE )
         set( Boost_USE_MULTITHREADED TRUE )
      endif ( MSVC )

      # Acutal find command
      set( COMPONENTS_TO_FIND system filesystem program_options date_time regex unit_test_framework )
      find_package( Boost ${Boost_MIN_REQ_VERSION} COMPONENTS ${COMPONENTS_TO_FIND} )

      # Record status in the cache to prevent further searches
      set( HASH_BOOST_FOUND ${Boost_FOUND} CACHE BOOL "Boost_Found" FORCE )
      mark_as_advanced( HASH_BOOST_FOUND )
      set( HASH_BOOST_INCLUDE_DIRS ${Boost_INCLUDE_DIR} CACHE PATH "Boost_INCLUDE_DIR" FORCE )
      mark_as_advanced( HASH_BOOST_INCLUDE_DIRS )
      set( HASH_BOOST_LIBRARIES ${Boost_LIBRARIES} CACHE PATH "Boost_LIBRARIES" FORCE )
      mark_as_advanced( HASH_BOOST_LIBRARIES )

   else()
      set( Boost_FOUND ${HASH_BOOST_FOUND} )
      set( Boost_INCLUDE_DIR ${HASH_BOOST_INCLUDE_DIRS} )
      set( Boost_LIBRARIES ${HASH_BOOST_LIBRARIES} )
   endif()

   # Adjust some behavior of found boost lib according to OS
   if( "${Boost_FOUND}" AND "${WIN32}" )
      add_definitions( "-D_WIN32_WINNT=0x0501" )
   endif()

   if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
      add_definitions( -DBOOST_ASIO_DISABLE_STD_ADDRESSOF )  # For asio
      add_definitions( -DBOOST_NO_DEFAULTED_FUNCTIONS )  # For program_options
   endif()

   # Hide boost dir variable that is somehow always set to not found
   if( ${HASH_BOOST_FOUND} )
      mark_as_advanced( Boost_DIR )
   endif()
endmacro()


#------------------------------------------------------------------------------
# - Sets the typical flags needed to compile with the compilers usually used
#   on the unix systems and their cousins (Linux, OSX, Sun, etc.).
#
#   The compilers are Clang and GCC.  If MinGW is used on Windows, it will use
#   the same flags.
#
#   CMake has a set of flags for every build type, so some flags are defined
#   a few times.
#
#   The optimization selected for the release build type is O2, which is a
#   compromise between super speed and executable size.  The higher the number,
#   the larger the executable and the faster the results (in theory).
function( set_nix_cxx_flags )
   # Unless otherwise specificed, set CMAKE_BUILD_TYPE to Debug
   # Set a default build type if none was specified

   if ( (DEFINED CMAKE_BUILD_TYPE) AND (NOT ${CMAKE_BUILD_TYPE} STREQUAL "") )
      set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} PARENT_SCOPE )
      set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} )
   else ()
      set( CMAKE_BUILD_TYPE "Debug" PARENT_SCOPE )
      set( CMAKE_BUILD_TYPE "Debug" )
   endif()

   message( STATUS "Setting build type to '${CMAKE_BUILD_TYPE}'.  'Debug' is default CMake value." )

   # Compiler flags
   if ( ${UNIX} )
      # Set compiler flags common to GCC and Clang
      if ( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) OR ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) )
         if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
            set( CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wno-macro-redefined -UNDEBUG" PARENT_SCOPE )
         elseif ( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
            set( CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Wno-macro-redefined -DNDEBUG" PARENT_SCOPE )
         elseif ( "${CMAKE_BUILD_TYPE}" STREQUAL "None" )
            set( CMAKE_CXX_FLAGS "-Wall -Wno-macro-redefined ${CXXVERS_STRG}" PARENT_SCOPE )
         endif ()
      endif()
   endif()
endfunction( set_nix_cxx_flags )


set_nix_cxx_flags()
setup_boost()
include_directories( ${Boost_INCLUDE_DIR} )


file( GLOB INLINE_FILES "${CMAKE_CURRENT_LIST_DIR}/include/*.inl" )
file( GLOB HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/include/*.h" )
include_directories( "${CMAKE_CURRENT_LIST_DIR}/include/" )

add_executable( hashing main.cpp ${HEADER_FILES} ${INLINE_FILES} )
set_property( TARGET hashing PROPERTY CXX_STANDARD 14 )
target_link_libraries( hashing ${Boost_LIBRARIES} )
