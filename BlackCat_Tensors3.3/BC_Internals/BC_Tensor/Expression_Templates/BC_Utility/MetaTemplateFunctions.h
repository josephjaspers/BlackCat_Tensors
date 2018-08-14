/*
 * BC_MetaTemplate_Simple.h
 *
 *  Created on: Dec 12, 2017
 *      Author: joseph
 */

#ifndef SIMPLE_H_
#define SIMPLE_H_
#include <type_traits>
#include "../BlackCat_Internal_Definitions.h"
namespace BC {
namespace MTF {
__BCinline__ static constexpr int max(int x) { return x; } template<class... integers>
__BCinline__ static constexpr int max(int x, integers... ints) { return x > max (ints...) ? x : max(ints...); }
__BCinline__ static constexpr int min(int x) { return x; } template<class... integers>
__BCinline__ static constexpr int min(int x, integers... ints) { return x < min (ints...) ? x : min(ints...); }

	//short_hand for const cast
	template<class T> auto& cc(const T& var) { return const_cast<T&>(var); }

	template<class> struct isPrimitive 					{ static constexpr bool conditional = false; };
	template<> struct isPrimitive<bool> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<short> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<unsigned short> 		{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<int> 					{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<unsigned> 			{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<long> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<unsigned long> 		{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<long long> 			{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<unsigned long long> 	{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<char> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<unsigned char>		{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<float> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<double> 				{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<long double> 			{ static constexpr bool conditional = true; };
	template<> struct isPrimitive<wchar_t> 				{ static constexpr bool conditional = true; };

	template<class T> static constexpr bool is_primitive_type() { return  MTF::isPrimitive<T>::conditional; }

	template<class... T> struct isIntList {
static constexpr bool conditional = true;
	};
	template<class T, class... Ts> struct isIntList<T,Ts...> {
		static constexpr bool conditional = false;
	};
	template<class... Ts> struct isIntList<int,Ts...> {
		static constexpr bool conditional = true && isIntList<Ts...>::conditional;
	};

	template<class... ts>
	static constexpr bool is_integer_sequence = isIntList<ts...>::conditional;

	template<class> struct lst;

	template<class T> struct lst {
		using front = T;
		using last = T;
	};
	template<template<class...> class LIST, class T, class... V>
	struct lst<LIST<T, V...>>  {
		using front = T;
		using last = T;
	};
	template<template<class...> class LIST, class T, class V>
	struct lst<LIST<T, V>>  {
		using front = T;
		using last = V;
	};

	template<class> struct shell_of;
	template<template<class ...> class param, class... ptraits>
	struct shell_of<param<ptraits...>> { template<class... ntraits> using type = param<ntraits...>; };


	template<class T, class... Ts>
	struct one_of_impl {
		static constexpr bool conditional = false;
	};
	template<class T, class U, class... Ts>
	struct one_of_impl<T, U, Ts...> {
		static constexpr bool conditional = one_of_impl<T, Ts...>::conditional;
	};
	template<class T, class... Ts>
	struct one_of_impl<T, T, Ts...> {
		static constexpr bool conditional = true;
	};

	template<class T, class... Ts> static constexpr bool is_one_of() { return one_of_impl<T, Ts...>::conditional; }


	//-------------------------------------if else chain
	template<bool x, class T> struct IF_IMPL;
	template<class T> struct IF_IMPL<true, T>  { using type = T; };

	template<class... > struct IF_BLOCK_IMPL;
	template<class T, class... Ts> struct IF_BLOCK_IMPL<T, Ts...> {
		using type = T;
	};
	template<bool x, class T, class... Ts>
	struct IF_BLOCK_IMPL<IF_IMPL<x, T>, Ts...> {
		using type = std::conditional_t<x, T, typename IF_BLOCK_IMPL<Ts...>::type>;
	};
	template<bool x, class T, class U, class... Ts>
	struct IF_BLOCK_IMPL<IF_IMPL<x, T>, U, Ts...> {
		using type = std::conditional_t<x, T, typename IF_BLOCK_IMPL<U, Ts...>::type>;
	};

	template<bool x, class t>
	using IF = typename IF_IMPL<x, t>::type;

	template<class... Ts>
	using IF_BLOCK = typename IF_BLOCK_IMPL<Ts...>::type;

	template<class from, class to, class enabler = void>
	struct is_castable_impl {
		static constexpr bool conditional = false;
	};

	template<class from, class to>
	struct is_castable_impl<from, to, decltype(static_cast<to>(std::declval<from>()))> {
		static constexpr bool conditional = true;

	};

	template<class from, class to> static constexpr bool is_castable() {
		return is_castable_impl<from, to>::conditional;
	}

}
}
#endif /* SIMPLE_H_ */