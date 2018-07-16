/*
 * trait_determiners.h
 *
 *  Created on: May 20, 2018
 *      Author: joseph
 */

#ifndef TRAIT_DETERMINERS_H_
#define TRAIT_DETERMINERS_H_

#include "type_determiners.h"

/*
 * Determines the primary traits of an internal-type
 * functor
 * scalar
 * iterator
 * mathlibrary
 * --tensor_scalar is the specific function determining the scalar of a tensor (opposed to internal-tensor type)
 * --IE tensor_scalar<Matrix<A,B>>, while _scalar is for... _scalar<binary_expression<lv,rv,oper>>
 */

namespace BC {
namespace internal {
template<int, class, class> class Array;
}

template<class T> struct isPrimaryArray { static constexpr bool conditional = false; };
template<int d, class T, class ml> struct isPrimaryArray<internal::Array<d,T,ml>> { static constexpr bool conditional = true; };
template<class T> static constexpr bool is_array_core() { return isPrimaryArray<T>::conditional; }


template<class> struct determine_functor;
template<class> struct determine_scalar;
template<class> struct determine_iterator;
template<class> struct determine_mathlibrary;
template<class> struct determine_tensor_scalar;

template<class T> using _scalar = typename determine_scalar<std::decay_t<T>>::type;
template<class T> using _mathlib = typename determine_mathlibrary<std::decay_t<T>>::type;
template<class T> using _functor = typename determine_functor<std::decay_t<T>>::type;
template<class T> using _iterator = typename determine_iterator<std::decay_t<T>>::type;
template<class T> using _tensor_scalar = typename determine_tensor_scalar<std::decay_t<T>>::type;
template<class T> static constexpr int _dimension_of  = dimension_of<std::decay_t<T>>;

///DETERMINE_FUNCTOR----------------------------------------------------------------------------------------------
template<template<class...> class tensor, class functor, class... set>
struct determine_functor<tensor<functor, set...>>{

	using derived = tensor<functor,set...>;
	using type = std::conditional_t<std::is_base_of<BC_Type,functor>::value, functor, internal::Array<_dimension_of<derived>, _scalar<derived>, _mathlib<derived>>>;
};

//DETERMINE_SCALAR_TYPE----------------------------------------------------------------------------------------------
template<class> struct determine_tensor_scalar;

template<class T>
struct determine_scalar {
	static constexpr bool nested_core_type = false;
	using type = T;
};
template<int dims, class scalar, class ml>
struct determine_scalar<internal::Array<dims, scalar, ml>> {
	static constexpr bool nested_core_type = true;
	using type = scalar;
};

template<template<class...> class expression, class T, class... set>
struct determine_scalar<expression<T, set...>> {
	static constexpr bool nested_core_type = determine_scalar<T>::nested_core_type;
	using type = std::conditional_t<nested_core_type, typename determine_scalar<T>::type,

			std::conditional_t<is_tensor<expression<T, set...>>, T,

			expression<T, set...>>>;
};
///DETERMINE_ITERATOR---------------------------------------------------------------------------------------
template<class T>
struct determine_iterator {
	using type = decltype(std::declval<T>().memptr());
};

///DETERMINE_MATHLIB---------------------------------------------------------------------------------------
template<class T> struct determine_mathlibrary;
template<class T, class... Ts, template<class...> class list> struct determine_mathlibrary<list<T, Ts...>>
{ using type = typename determine_mathlibrary<T>::type;};

template<int x, class scalar, class ml>
struct determine_mathlibrary<internal::Array<x, scalar, ml>> {
	using type = ml;
};


template<class T, class ml> struct determine_mathlibrary<Scalar<T, ml>> { using type = ml; };
template<class T, class ml> struct determine_mathlibrary<Vector<T, ml>> { using type = ml; };
template<class T, class ml> struct determine_mathlibrary<Matrix<T, ml>> { using type = ml; };
template<class T, class ml> struct determine_mathlibrary<Cube<T, ml>> { using type = ml; };
template<class T, class ml> struct determine_mathlibrary<Tensor4<T, ml>> { using type = ml; };
template<class T, class ml> struct determine_mathlibrary<Tensor5<T, ml>> { using type = ml; };

}



#endif /* TRAIT_DETERMINERS_H_ */