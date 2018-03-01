/*
 * BC_MetaTemplate_Simple.h
 *
 *  Created on: Dec 12, 2017
 *      Author: joseph
 */

#ifndef SIMPLE_H_
#define SIMPLE_H_
#include <type_traits>
namespace BC {
namespace MTF {

		template<class T> struct isTrue { static constexpr bool conditional = true; };
		template<> struct isTrue<std::false_type> { static constexpr bool conditional = false; };

		//EQUIVALENT OF std::conditional
		template<bool iff, class THEN, class ELSE>
		struct IF_ELSE {
			using type = THEN;
		};
		template<class THEN, class ELSE>
		struct IF_ELSE<false, THEN, ELSE> {
			using type = ELSE;
		};


		//EQUIVALENT OF std::enable_if
		template<bool iff, class THEN>
		struct IF;
		template<class THEN>
		struct IF<true, THEN> {
			using type = THEN;
		};

	template<class>struct front;

	template<template<class...> class list, class... set, class first>
	struct front<list<first, set...>>
	{ using type = first; };

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

	template<class, class> struct same  { static constexpr bool conditional = false; };
	template<class T> struct same<T, T> { static constexpr bool conditional = true; };


	template<bool var, class a, class b>
	using ifte = typename std::conditional<var, a, b>::type; //ifte -- if than, else

	template<class T>
	static constexpr bool isPrim = MTF::isPrimitive<T>::conditional;

	template<class> struct shell_of;
	template<template<class ...> class param, class... ptraits>
	struct shell_of<param<ptraits...>> { template<class... ntraits> using type = param<ntraits...>; };

	}
}
#endif /* SIMPLE_H_ */
