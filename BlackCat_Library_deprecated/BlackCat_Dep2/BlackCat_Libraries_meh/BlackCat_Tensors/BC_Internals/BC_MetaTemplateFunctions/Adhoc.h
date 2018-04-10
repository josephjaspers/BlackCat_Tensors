/*
 * BC_MetaTemplate_EssentialMethods.h
 *
 *  Created on: Dec 11, 2017
 *      Author: joseph
 */

#ifndef ADHOC_H_
#define ADHOC_H_

#include "../BlackCat_Internal_GlobalUnifier.h"

namespace BC {
	namespace MTF {



		//class variant of bool (good when you only want to use classes as templates)
		template<bool value>
		struct BOOL {
				static constexpr bool conditional = value;
		};



		template<class, class>  struct expression_substitution;
		template<class sub, template<class...> class derived, class scalar_type, class... modifiers>
		struct expression_substitution<sub, derived<scalar_type, modifiers...>>{
				using type = derived<sub, modifiers...>;
		};


		template<class T>
		struct determine_scalar {
				using type = T;
		};
		template<template<class...> class tensor, class T, class... set>
		struct determine_scalar<tensor<T, set...>> {
				using type = typename determine_scalar<T>::type;
		};

		template<class T, class voider = void>
		struct determine_functor {
				using type = T;
		};

		template<class>struct isPrimitive;

		template<class T>
		struct determine_functor<T, typename std::enable_if<MTF::isPrimitive<T>::conditional, void>::type> {
				using type = T*;
		};

		template<class tensor>
		struct determine_evaluation;

		template<template<class...> class tensor, class T, class... traits>
		struct determine_evaluation<tensor<T, traits...>> {
				using scalar_type = typename determine_scalar<T>::type;
				using type = tensor<scalar_type, traits...>;
		};
	}
}
#endif /* ADHOC_H_ */
