/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BC_EXPRESSION_TEMPLATES_EXPRESSION_UNARY_H_
#define BC_EXPRESSION_TEMPLATES_EXPRESSION_UNARY_H_

#include "expression_template_base.h"

namespace bc {
namespace tensors {
namespace exprs { 

template<class Operation, class ArrayType>
struct Un_Op:
		Expression_Base<Un_Op<Operation, ArrayType>>,
		Operation {

	using return_type = decltype(std::declval<Operation>()(
			std::declval<typename ArrayType::value_type>()));

	using value_type  = std::decay_t<return_type>;
	using system_tag  = typename ArrayType::system_tag;

	static constexpr int tensor_dim = ArrayType::tensor_dim;
	static constexpr int tensor_iterator_dim = ArrayType::tensor_iterator_dim;

	ArrayType array;

	BCINLINE
	const Operation& get_operation() const {
		return static_cast<const Operation&>(*this);
	}

	template<class... args> BCINLINE
	Un_Op(ArrayType v, const args&... args_):
			Operation(args_...),
			array(v) {}

	template<class... integers> BCINLINE
	value_type operator ()(integers... index) const {
		return Operation::operator()(array(index...));
	}

	BCINLINE
	value_type operator [](int index) const {
		return Operation::operator()(array[index]);
	}

	template<class... integers> BCINLINE
	value_type operator ()(integers... index) {
		return Operation::operator()(array(index...));
	}

	BCINLINE
	value_type operator [](int index) {
		return Operation::operator()(array[index]);
	}

	BCINLINE const auto inner_shape() const { return array.inner_shape(); }
	BCINLINE bc::size_t size() const { return array.size(); }
	BCINLINE bc::size_t rows() const { return array.rows(); }
	BCINLINE bc::size_t cols() const { return array.cols(); }
	BCINLINE bc::size_t dim(int i) const { return array.dim(i); }
};


template<class Operation, class Expression> BCHOT
auto make_un_expr(Expression expression, Operation operation=Operation()) {
	return Un_Op<Operation, Expression>(expression, operation);
}

} //ns BC
} //ns exprs
} //ns tensors


#endif /* EXPRESSION_UNARY_POINTWISE_CU_ */
