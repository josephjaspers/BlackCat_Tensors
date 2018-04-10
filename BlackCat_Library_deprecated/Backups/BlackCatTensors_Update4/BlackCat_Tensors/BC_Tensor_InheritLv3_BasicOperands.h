/*
 * BC_Tensor_InheritLv3_MathInterface_Lower.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef HBC_TENSOR_INHERITLV3_BASICOPERANDS_H_
#define HBC_TENSOR_INHERITLV3_BASICOPERANDS_H_

#include "BC_Tensor_InheritLv2_Ace.h"

/*
 * Defines basic math operations -- This is really an extension of Tensor_Ace  -- fundamentally these could be in a single class, seperated for code clarity
 */

template<class T, class ml, int ... dimensions>
struct Tensor_King : public Tensor_Ace<T, ml, dimensions...> {

	using array_type = typename Tensor_Ace<T, ml, dimensions...>::array_type;					//The type of the internal memory array
	using functor_type = typename Tensor_Ace<T, ml, dimensions...>::functor_type;				//The functor type (either T* or some T expression)
	using identity_type = typename Tensor_Ace<T, ml, dimensions...>::identity_type;			//The identity (IE Scalar, Vector, Matrix, etc)
	using lv = Tensor_King<T, ml, dimensions...>;  									//This type (typedef for this class)


	template<class... params> Tensor_King<T, ml, dimensions...>(const params&... p) : Tensor_Ace<T, ml, dimensions...>(p...) {}
	template<class... params> Tensor_King<T, ml, dimensions...>(params&... p) : Tensor_Ace<T, ml, dimensions...>(p...) {}
							  Tensor_King<T, ml, dimensions...>() = default;

	template<class U>
	auto operator +(const Tensor_King<U, ml, dimensions...>& rv) {
		return Tensor_King<binary_expression<array_type, BC::add, functor_type, typename Tensor_King<U, ml, dimensions...>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	auto operator - (const Tensor_King<U, ml, dimensions...>& rv) {
		return Tensor_King<binary_expression<array_type, BC::sub, functor_type, typename Tensor_King<U, ml, dimensions...>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	auto operator /(const Tensor_King<U, ml, dimensions...>& rv) {
		return Tensor_King<binary_expression<array_type, BC::div, functor_type, typename Tensor_King<U, ml, dimensions...>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	auto operator %(const Tensor_King<U, ml, dimensions...>& rv) {
		return Tensor_King<binary_expression<array_type, BC::mul, functor_type, typename Tensor_King<U, ml, dimensions...>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	Tensor_King<binary_expression_scalar_R<array_type, BC::add, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...> operator +(const Tensor_King<U, ml, 1>& rv) {
		return Tensor_King<binary_expression_scalar_R<array_type, BC::add, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	Tensor_King<binary_expression_scalar_R<array_type, BC::sub, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...> operator - (const Tensor_King<U, ml, 1>& rv) {
		return Tensor_King<binary_expression_scalar_R<array_type, BC::sub, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	Tensor_King<binary_expression_scalar_R<array_type, BC::div, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...> operator /(const Tensor_King<U, ml, 1>& rv) {
		return Tensor_King<binary_expression_scalar_R<array_type, BC::div, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}

	template<class U>
	Tensor_King<binary_expression_scalar_R<array_type, BC::mul, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...> operator %(const Tensor_King<U, ml, 1>& rv) {
		return Tensor_King<binary_expression_scalar_R<array_type, BC::mul, functor_type, typename Tensor_King<U, ml, 1>::functor_type>, ml, dimensions...>(this->data(), rv.data());
	}
};

#endif /* HBC_TENSOR_INHERITLV3_BASICOPERANDS_H_ */












