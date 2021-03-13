#pragma once

#include <cinttypes>
#include <string>

namespace CompileTime
{
	namespace __internal
	{
		constexpr static uint32_t default_offset_basis_32 = static_cast<uint32_t>( 0x811C9DC5u );
		constexpr static uint32_t prime_32 = static_cast<uint32_t>( 0x01000193u );

		constexpr static uint64_t default_offset_basis_64 = static_cast<uint64_t>( 14695981039346656037ull );
		constexpr static uint64_t prime_64 = static_cast<uint64_t>( 1099511628211ull );
	}

#if defined(_MSC_VER)
	__pragma( warning( push ) );
	__pragma( warning( disable:4307 ) );
#endif
	constexpr inline uint32_t FNV32Hash( const char* const aString, const uint32_t val = __internal::default_offset_basis_32 )
	{
		return ( aString[ 0 ] == '\0' ) ? val : FNV32Hash( &aString[ 1 ], static_cast<uint32_t>( ( val * static_cast<unsigned long long>( __internal::prime_32 ) ) ^ static_cast<uint32_t>( aString[ 0 ] ) ) );
	}


	constexpr inline uint16_t FNV16Hash( const char* const aString, const uint32_t val = __internal::default_offset_basis_32 )
	{
		const uint32_t hval = FNV32Hash( aString, val );
		return ( hval >> 16 ) ^ ( hval & 0xFFFF );
	}

	constexpr inline uint64_t FNV64Hash( const char* const aString, const uint64_t val = __internal::default_offset_basis_64 )
	{
		return ( aString[ 0 ] == '\0' ) ? val : FNV64Hash( &aString[ 1 ], ( val * __internal::prime_64 ) ^ static_cast<uint64_t>( aString[ 0 ] ) );
	}

#if defined(_MSC_VER)
	__pragma( warning( pop ) );
#endif
}

namespace Hashing
{
	template <typename S> struct fnv_internal;
	template <typename S> struct fnv1;
	template <typename S> struct fnv1a;

	template <> struct fnv_internal<uint32_t>
	{
		constexpr static uint32_t default_offset_basis = static_cast<uint32_t>( 0x811C9DC5u );
		constexpr static uint32_t prime = static_cast<uint32_t>( 0x01000193u );
	};

	template <> struct fnv1<uint32_t> : public fnv_internal<uint32_t>
	{
		constexpr static inline uint32_t hash( char const* const aString, const uint32_t val = default_offset_basis )
		{
			return ( aString[ 0 ] == '\0' ) ? val : hash( &aString[ 1 ], ( val * prime ) ^ static_cast<uint32_t>( aString[ 0 ] ) );
		}
	};

	template <> struct fnv1a<uint32_t> : public fnv_internal<uint32_t>
	{
		constexpr static inline uint32_t hash( char const* const aString, const uint32_t val = default_offset_basis )
		{
			return ( aString[ 0 ] == '\0' ) ? val : hash( &aString[ 1 ], ( val ^ static_cast<uint32_t>( aString[ 0 ] ) ) * prime );
		}
	};

	template <> struct fnv1<uint16_t> : public fnv_internal<uint32_t>
	{
		constexpr static inline uint16_t hash( char const* const aString, const uint32_t val = default_offset_basis )
		{
			const uint32_t hval = fnv1<uint32_t>::hash( aString, val );
			return ( hval >> 16 ) ^ ( hval & 0xFFFF );
		}
	};

	template <> struct fnv1a<uint16_t> : public fnv_internal<uint32_t>
	{
		constexpr static inline uint16_t hash( char const* const aString, const uint32_t val = default_offset_basis )
		{
			const uint32_t hval = fnv1a<uint32_t>::hash( aString, val );
			return ( hval >> 16 ) ^ ( hval & 0xFFFF );
		}
	};

	template <> struct fnv_internal<uint64_t>
	{
		constexpr static uint64_t default_offset_basis = static_cast<uint64_t>( 14695981039346656037ull );
		constexpr static uint64_t prime = static_cast<uint64_t>( 1099511628211ull );
	};

	template <> struct fnv1<uint64_t> : public fnv_internal<uint64_t>
	{
		constexpr static inline uint64_t hash( char const* const aString, const uint64_t val = default_offset_basis )
		{
			return ( aString[ 0 ] == '\0' ) ? val : hash( &aString[ 1 ], ( val * prime ) ^ static_cast<uint64_t>( aString[ 0 ] ) );
		}
	};

	template <> struct fnv1a<uint64_t> : public fnv_internal<uint64_t>
	{
		constexpr static inline uint64_t hash( char const* const aString, const uint64_t val = default_offset_basis )
		{
			return ( aString[ 0 ] == '\0' ) ? val : hash( &aString[ 1 ], ( val ^ static_cast<uint64_t>( aString[ 0 ] ) ) * prime );
		}
	};
}

constexpr uint16_t operator"" _fnv16( const char* v, size_t size ) { (void)size; return CompileTime::FNV16Hash( v ); }
constexpr uint32_t operator"" _fnv32( const char* v, size_t size ) { (void)size; return CompileTime::FNV32Hash( v ); }