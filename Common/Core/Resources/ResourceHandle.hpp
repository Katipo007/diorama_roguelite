#pragma once

#include <string>
#include <memory>

#include "Concepts.hpp"

namespace Resources
{
	template<Resources::Concepts::Resource RESOURCE>
	using ResourceHandle = std::shared_ptr<const RESOURCE>;

#if 0
	template<typename RESOURCE>
	class ResourceHandle
	{
	public:
		ResourceHandle() : ptr( nullptr ) {}
		ResourceHandle( std::shared_ptr<const RESOURCE> ptr ) : ptr( ptr ) {}
		ResourceHandle( const ResourceHandle& to_copy ) : ptr( to_copy.ptr ) {}
		ResourceHandle& operator=( const ResourceHandle& to_copy )
		{
			ptr = to_copy.ptr;

			return *this;
		}
		ResourceHandle( ResourceHandle&& to_move ) noexcept : ptr( std::move( to_move.ptr ) ) {}
		ResourceHandle& operator=( ResourceHandle&& to_move ) noexcept
		{
			if (&to_move != this)
				ptr = std::move( to_move.ptr );

			return *this;
		}
		virtual ~ResourceHandle() = default;

		bool IsNull() const { return !ptr; }
		operator bool() const noexcept { return !!ptr; }

		const RESOURCE& operator*() const { ASSERT( ptr ); return *ptr; }
		const RESOURCE* operator->() const { return ptr.get(); }

		bool operator==( const ResourceHandle<RESOURCE>& rhs ) const noexcept { return (this->ptr == rhs.ptr); }
		bool operator!=( const ResourceHandle<RESOURCE>& rhs ) const noexcept { return (this->ptr != rhs.ptr); }

	protected:
		std::shared_ptr<const RESOURCE> ptr;
	};
#endif
}
