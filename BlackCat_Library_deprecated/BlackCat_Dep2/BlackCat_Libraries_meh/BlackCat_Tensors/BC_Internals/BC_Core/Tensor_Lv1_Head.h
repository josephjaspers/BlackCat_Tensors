/*
 * Tensor_Core.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef TENSOR_HEAD_H_
#define TENSOR_HEAD_H_

#include "../BlackCat_Internal_GlobalUnifier.h"
#include "Tensor_IdentityClasses.h"
namespace BC {

	template<class derived, class TRAITS>
	struct Tensor_Head : public MTF::expression_substitution<Tensor_Head<derived, TRAITS>, typename TRAITS::identity>::type {

			using evaluation_type 	= typename TRAITS::evaluation_type;	//The type returned when instant evaluation is required
			using functor_type 		= typename TRAITS::functor_type;		//The data-type passed to expressions
			using scalar_type  		= typename TRAITS::scalar_type;		//The array_type of GenuineTensors and the Evaluation Type for ExpressionTensors
			using identity     		= typename TRAITS::identity;			//Enumeration type for Scalar/Vector/Matrix/Cube/Tensor
			using math_library 		= typename TRAITS::math_library;

			using this_type = derived;
			using traits  	= TRAITS;

			template<class param_deriv, class param_traits>
			struct impl {

					using param_functor_type = typename Tensor_Head<param_deriv, param_traits>::functor_type;

					using add_expression = typename MTF::expression_substitution<binary_expression<scalar_type, add, functor_type, param_functor_type>, derived>::type;
					using sub_expression = typename MTF::expression_substitution<binary_expression<scalar_type, sub, functor_type, param_functor_type>, derived>::type;
					using div_expression = typename MTF::expression_substitution<binary_expression<scalar_type, div, functor_type, param_functor_type>, derived>::type;
					using mul_expression = typename MTF::expression_substitution<binary_expression<scalar_type, mul, functor_type, param_functor_type>, derived>::type;

					//Scalar
					using scalar_add_expression = typename MTF::expression_substitution<binary_expression_scalar_R<scalar_type, add, functor_type, param_functor_type>, derived>::type;
					using scalar_sub_expression = typename MTF::expression_substitution<binary_expression_scalar_R<scalar_type, sub, functor_type, param_functor_type>, derived>::type;
					using scalar_div_expression = typename MTF::expression_substitution<binary_expression_scalar_R<scalar_type, div, functor_type, param_functor_type>, derived>::type;
					using scalar_mul_expression = typename MTF::expression_substitution<binary_expression_scalar_R<scalar_type, mul, functor_type, param_functor_type>, derived>::type;
			};
private:
			auto  super_size() const { return static_cast<const derived&>(*this).size(); };
public:
			template<class Tensor> void  assert_same_size(const Tensor& t) {
				if (this->super_size() != t.super_size()) {
					throw std::invalid_argument("size mismatch");
				}
			}

			const derived& asBase() const { return static_cast<const derived&>(*this); }
				  derived& asBase() 	  { return static_cast<	     derived&>(*this); }


			const functor_type& data() const { return static_cast<const derived&>(*this).array; }
				  functor_type& data()		 { return static_cast<		derived&>(*this).array; }

			const evaluation_type eval() const { return static_cast<evaluation_type>(*this); }
				  evaluation_type eval() 	   { return static_cast<evaluation_type>(*this); }


			template<class pDeriv, class pTraits>
			typename impl<pDeriv, pTraits>::add_expression operator +(const Tensor_Head<pDeriv, pTraits>& param) {
				assert_same_size(param);
				return typename impl<pDeriv, pTraits>::add_expression(this->data(), param.data());
			}
			template<class pDeriv, class pTraits>
			typename impl<pDeriv, pTraits>::sub_expression operator -(const Tensor_Head<pDeriv, pTraits>& param) {
				assert_same_size(param);
				return typename impl<pDeriv, pTraits>::sub_expression(this->data(), param.data());
			}
			template<class pDeriv, class pTraits>
			typename impl<pDeriv, pTraits>::div_expression operator /(const Tensor_Head<pDeriv, pTraits>& param) {
				assert_same_size(param);
				return typename impl<pDeriv, pTraits>::div_expression(this->data(), param.data());
			}
			template<class pDeriv, class pTraits>
			typename impl<pDeriv, pTraits>::mul_expression operator %(const Tensor_Head<pDeriv, pTraits>& param) {
				assert_same_size(param);
				return typename impl<pDeriv, pTraits>::mul_expression(this->data(), param.data());
			}

			template<class core>
			typename impl<typename core::this_type, typename core::traits>::add_expression operator +(const SCALAR<core>& param) {
				return typename impl<typename core::this_type, typename core::traits>::add_expression(this->data(), param.data());
			}
			template<class core>
			typename impl<typename core::this_type, typename core::traits>::sub_expression operator -(const SCALAR<core>& param) {
				return typename impl<typename core::this_type, typename core::traits>::sub_expression(this->data(), param.data());
			}
			template<class core>
			typename impl<typename core::this_type, typename core::traits>::div_expression operator /(const SCALAR<core>& param) {
				return typename impl<typename core::this_type, typename core::traits>::div_expression(this->data(), param.data());
			}
			template<class core>
			typename impl<typename core::this_type, typename core::traits>::mul_expression operator %(const SCALAR<core>& param) {
				return typename impl<typename core::this_type, typename core::traits>::mul_expression(this->data(), param.data());
			}
	};

}

#endif /* TENSOR_CORE_H_ */
