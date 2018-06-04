/*
 * Base_Functions.h
 *
 *  Created on: May 15, 2018
 *      Author: joseph
 */

#ifndef BASE_FUNCTIONS_H_
#define BASE_FUNCTIONS_H_

//This should be included at the bottom of Tensor_Base.h
//Reshape and Chunk are curried functions
//This file defines a set of unary_functions
namespace BC {
//alternate names from transposition
template<class deriv>
static auto trans(Tensor_Operations<deriv>& tensor) {
	 return tensor.t();
}
//zero and one are the equivalent but delayed evaluation version,
//modifies the calling the tensor, but if and only if evaluated
template<class deriv>
static auto zero(Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::zero());
}
template<class deriv>
static auto one(Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::one());
}

//converts all NaN/Inf values to 0
template<class deriv>
static auto fix(Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::fix());
}
template<class deriv>
static auto abs(const Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::abs());
}
template<class deriv>
static auto negation(const Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::negation());
}

//if 0 return 0 else return 1
template<class deriv>
static auto logical(const Tensor_Operations<deriv>& tensor) {
	 return tensor.un_expr(function::logical());
}

}


#endif /* BASE_FUNCTIONS_H_ */