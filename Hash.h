#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_
//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#include <stdint.h>

#ifdef WIN32
#ifdef _WIN64
#define MurmurHash3_128 MurmurHash3_x64_128
#else // _WIN64
#define MurmurHash3_128 MurmurHash3_x86_128
#endif // !_WIN64
#endif // WIN32

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );

void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );

void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_