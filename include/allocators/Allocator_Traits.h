/*
 * Allocator_Traits.h
 *
 *  Created on: Jan 1, 2019
 *      Author: joseph
 */

#ifndef BC_ALLOCATOR_ALLOCATOR_TRAITS_H_
#define BC_ALLOCATOR_ALLOCATOR_TRAITS_H_

#include <memory>

namespace BC {

class host_tag;
class device_tag;

namespace allocators {
namespace detail {

	template<class T>
	using query_system_tag = typename T::system_tag;

	template<class T>
	using query_managed_memory
			= std::conditional_t<T::managed_memory, std::true_type, std::false_type>;
}

/// Inherits from std::allocator_traits, defines additional features relative to gpu-memory management.
template<class Allocator>
struct allocator_traits : std::allocator_traits<Allocator> {

	using system_tag =
			BC::traits::conditional_detected_t<
			detail::query_system_tag, Allocator, host_tag>;

	static constexpr bool is_managed_memory =
			BC::traits::conditional_detected_t<
			detail::query_managed_memory, Allocator, std::false_type>::value;
};

}
}



#endif /* ALLOCATOR_TRAITS_H_ */