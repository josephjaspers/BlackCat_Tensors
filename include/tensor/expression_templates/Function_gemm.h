/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BC_EXPRESSION_TEMPLATES_FUNCTION_GEMM_H_
#define BC_EXPRESSION_TEMPLATES_FUNCTION_GEMM_H_

#include "Expression_Base.h"
#include "Tree_Lazy_Evaluator.h"
#include "blas_tools/Blas_tools.h"

namespace BC {
namespace exprs {


template<class lv, class rv, class System_Tag>
struct Binary_Expression<lv, rv, oper::gemm<System_Tag>>
: Expression_Base<Binary_Expression<lv, rv, oper::gemm<System_Tag>>>,
  oper::gemm<System_Tag> {

	static_assert(std::is_same<typename lv::value_type, typename rv::value_type>::value,
    		"MATRIX MULTIPLICATION ONLY AVAILABLE TO SAME TYPE TENSORS (FLOAT/DOUBLE)");

    using value_type	= typename lv::value_type;
    using system_tag	= System_Tag;
    using blas_impl		= BC::blas::implementation<system_tag>;
    using blas_util	    = BC::exprs::blas_tools::implementation<system_tag>;

    static constexpr bool transA = blas_expression_traits<lv>::is_transposed;
    static constexpr bool transB = blas_expression_traits<rv>::is_transposed;
    static constexpr bool lv_scalar = blas_expression_traits<lv>::is_scalar_multiplied;
    static constexpr bool rv_scalar = blas_expression_traits<rv>::is_scalar_multiplied;

    static constexpr int DIMS 	   = rv::DIMS;
    static constexpr int ITERATOR  = 1;


    lv left;
    rv right;


     Binary_Expression(lv left, rv right)
     : left(left), right(right) {}

    BCINLINE
    const auto inner_shape() const {
    	return make_lambda_array<DIMS>([&](int i) {
    		return i == 0 ? left.rows() : i == 1 ? right.cols() : 1;
    	});
    }
    BCINLINE
    const auto block_shape() const {
    	return make_lambda_array<DIMS>([&](int i) {
    		return i == 0 ? left.rows() : i == 1 ? size() : 1;
    	});
    }

    BCINLINE BC::size_t  size() const { return left.rows() * right.cols(); }
    BCINLINE BC::size_t  rows() const { return left.rows();  }
    BCINLINE BC::size_t  cols() const { return right.cols(); }
    BCINLINE BC::size_t  dimension(int i) const { return inner_shape()[i]; }
    BCINLINE BC::size_t  block_dimension(int i) const { return block_shape()[i]; }

    BCINLINE BC::size_t  M() const { return left.rows();  }
    BCINLINE BC::size_t  N() const { return right.cols(); }
    BCINLINE BC::size_t  K() const { return left.cols();  }


    template<class core, int alpha_mod, int beta_mod, class Context>
    void eval(tree::injector<core, alpha_mod, beta_mod> injection_values, Context& alloc) const {

        //get the data of the injection --> injector simply stores the alpha/beta scalar modifiers
        auto& injection = injection_values.data();

        //evaluate the left and right branches (computes only if necessary)
        auto A = greedy_evaluate(blas_expression_traits<lv>::remove_blas_modifiers(left), alloc);
        auto B = greedy_evaluate(blas_expression_traits<rv>::remove_blas_modifiers(right), alloc);

        //get the left and right side scalar values
        auto alpha_lv = blas_expression_traits<lv>::get_scalar(left);
		auto alpha_rv = blas_expression_traits<rv>::get_scalar(right);

		auto alpha = blas_util::template calculate_alpha<value_type, alpha_mod, lv_scalar, rv_scalar>(alloc, alpha_lv, alpha_rv);
        auto beta  = make_constexpr_scalar<typename expression_traits<decltype(alpha)>::allocation_tag, beta_mod, value_type>();//blas_impl::template scalar_constant<value_type, beta_mod>();

			//call matrix_mul
        blas_impl::gemm(alloc, transA, transB,  M(), N(), K(),
					alpha, A, A.leading_dimension(0),
					B, B.leading_dimension(0),
					beta, injection, injection.leading_dimension(0));
//
        BC::meta::constexpr_if<(BC::exprs::expression_traits<decltype(alpha)>::is_temporary)>(
            BC::meta::bind([&](auto alpha) {
        		alloc.template get_allocator_rebound<value_type>().deallocate(alpha, 1);
        	}, 	alpha));

    }
};


}
}


#endif /* EXPRESSION_BINARY_DOTPRODUCT_CU_ */
