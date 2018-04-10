/*
 * BC_Expression_Unary.h
 *
 *  Created on: Nov 25, 2017
 *      Author: joseph
 */

#ifndef BC_EXPRESSION_UNARY_H_
#define BC_EXPRESSION_UNARY_H_

#include "BC_Tensor_Super_Queen.h"

template<class operation, class T, class ml, int ... dimensions>
struct unary_expression : public Tensor_Queen<unary_expression<operation, dimensions...>, ml, dimensions...> {

	using this_type = unary_expression<operation, T, dimensions...>;

	operation oper;
	T data;

	inline __attribute__((always_inline)) unary_expression(T dat) :
			data(dat) {
	}

	inline __attribute__((always_inline))    T operator [](int i) const {
		return oper(data[i]);
	}
};



#endif /* BC_EXPRESSION_UNARY_H_ */
