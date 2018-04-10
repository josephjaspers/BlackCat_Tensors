/*
 * Transpose_Wrapper.h
 *
 *  Created on: Dec 19, 2017
 *      Author: joseph
 */

#ifndef EXPRESSION_UNARY__MATRIXTRANSPOSITION_H_
#define EXPRESSION_UNARY__MATRIXTRANSPOSITION_H_
#include <cmath>

#include "../BlackCat_Internal_GlobalUnifier.h"
#include "../BC_MetaTemplateFunctions/Typeclass_FunctionType.h"
#include "../BC_MetaTemplateFunctions/Adhoc.h"
namespace BC {
template<class T, class parent>
struct unary_expression_transpose_mat : expression<T, unary_expression_transpose_mat<T, unary_expression_transpose_mat<T , parent>>>
{
	/*
	 * Rows and Cols are the CURRENT rows and cols of the set,
	 * which means
	 */

	using this_type = unary_expression_transpose_mat<T,parent>;
	using functor_type = typename Tensor_FunctorType<T>::type;

	functor_type array;
	const parent& root;
	unary_expression_transpose_mat(const parent& p, functor_type data) : array(data), root(p) {}

	__attribute__((always_inline)) auto& operator [](int index) {
		return array[(int)floor(index / root.cols()) + (index % root.cols()) * root.rows()];
	}
	__attribute__((always_inline)) const auto& operator[](int index) const {
		return array[(int) floor(index / root.cols()) + (index % root.cols()) * root.rows()];
	}
};

}
#endif /* EXPRESSION_UNARY__MATRIXTRANSPOSITION_H_ */
