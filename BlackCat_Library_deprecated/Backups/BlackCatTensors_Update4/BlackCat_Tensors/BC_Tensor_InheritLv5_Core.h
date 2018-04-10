/*
 * BC_Tensor_InheritLv5_Core.h
 *
 *  Created on: Dec 3, 2017
 *      Author: joseph
 */

#ifndef BC_TENSOR_INHERITLV5_CORE_H_
#define BC_TENSOR_INHERITLV5_CORE_H_

#include "BC_Tensor_InheritLv3_BasicOperands.h"
#include "BC_Mathematics_CPU.h"
#include "BC_Tensor_InheritLv4_Queen.h"

template<class...>
struct _traits;

/*
 * Inherits from the Tensor_Queen if Utility functions are supported, else inherits from tensor King
 */


template<class T, class ml, int... dimensions>
struct Tensor_Core :
		public virtual BC_MTF::IF<Tensor_FunctorType<T>::supports_utility, Tensor_Queen<T, ml, dimensions...>, Tensor_King<T, ml, dimensions...>>::type {

	using parent_class = typename BC_MTF::IF<Tensor_FunctorType<T>::supports_utility, Tensor_Queen<T, ml, dimensions...>, Tensor_King<T, ml, dimensions...>>::type;
//	using Math_Library = CPU;

	Tensor_Core<T, ml, dimensions...>() = default;

	template<class... params>
	Tensor_Core(const params&... p) : parent_class(p...) {};
};

#endif /* BC_TENSOR_INHERITLV5_CORE_H_ */
