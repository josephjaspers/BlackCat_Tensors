/*
 * BC_Tensor_Base.h
 *
 *  Created on: Dec 12, 2017
 *      Author: joseph
 */

#ifndef TENSOR_LV4_MATH_HEAD_H_
#define TENSOR_LV4_MATH_HEAD_H_

#include "Tensor_Lv3_ArrayType_Utility.h"
#include "Tensor_Lv3_ExpressionType_Utility.h"
#include <utility>
namespace BC {

template<
class T,										//The internal array type
class derived,									//The derived class
class math_lib,					    		    //Math_library
class inner_shape,
class outer_shape = typename DEFAULT_LD<inner_shape>::type
>
struct Tensor_Mathematics_Head :

		BC_MTF::IF_ELSE<Tensor_FunctorType<T>::supports_utility_functions,								//if supports_utility_functions
		BC_Tensor_Utility_Array<T, derived, math_lib, inner_shape, outer_shape>,						//		inherit from Utility_Array
		BC_Tensor_Utility_Expression<T, derived, math_lib, inner_shape, outer_shape>>::type				//else
																										//		inherit from Utility_expression
	{



	using absolute_base_type   = Tensor_Mathematics_Head<T, derived, math_lib, inner_shape>; 	//Matching type with outer_shape adjustment
	using absolute_type        = typename Tensor_Mathematics_Head<T, derived, math_lib, inner_shape>::this_type; //gets the derived class with default outer_shape
	using my_type		       = Tensor_Mathematics_Head<T, derived, math_lib, inner_shape, outer_shape>;
	using this_type    	  	   = derived;
	using functor_type 		   = typename  Tensor_Type<T, derived, math_lib, Shape<inner_shape, outer_shape>>::functor_type;

	using parent_class =
			typename BC_MTF::IF_ELSE<Tensor_FunctorType<T>::supports_utility_functions,
			BC_Tensor_Utility_Array<T, derived, math_lib, inner_shape, outer_shape>,
			BC_Tensor_Utility_Expression<T, derived, math_lib, inner_shape, outer_shape>>::type;

	using grandparent_class = typename parent_class::grandparent_class;

	typedef std::initializer_list<int> _shape;
	Tensor_Mathematics_Head(_shape dims) : parent_class(nullptr) {
		this->initDefault(dims);
		this->DYNAMIC_ARRAY_INITIALIZE();
	}
	Tensor_Mathematics_Head(_shape inner, _shape outer) : parent_class(nullptr) {
		this->init_inner_shape(inner);
		this->init_outer_shape(outer);
	    this->DYNAMIC_ARRAY_INITIALIZE();
	}
	template<class... params>
	Tensor_Mathematics_Head(_shape inner, _shape outer, const params&... p) : parent_class(p...) {
		this->init_inner_shape(inner);
		this->init_outer_shape(outer);
	    this->DYNAMIC_ARRAY_INITIALIZE();
	}

	template<class... params> explicit Tensor_Mathematics_Head(const params&...  p) : parent_class(p...) {}
	Tensor_Mathematics_Head() = default;

private:

	template<class U, class deriv>
	struct expression_tensor {

		using add = typename BC_Substitute_Type::Identity<binary_expression<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape>::functor_type>, this_type>::type;
		using sub = typename BC_Substitute_Type::Identity<binary_expression<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape>::functor_type>, this_type>::type;
		using div = typename BC_Substitute_Type::Identity<binary_expression<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape>::functor_type>, this_type>::type;
		using mul = typename BC_Substitute_Type::Identity<binary_expression<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape>::functor_type>, this_type>::type;

		using add_scal =  typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>>::functor_type>, this_type>::type;
		using sub_scal =  typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>>::functor_type>, this_type>::type;
		using div_scal =  typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>>::functor_type>, this_type>::type;
		using mul_scal =  typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>>::functor_type>, this_type>::type;


		using Dadd = typename BC_Substitute_Type::Identity<binary_expression<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<0>>::functor_type>, this_type>::type;
		using Dsub = typename BC_Substitute_Type::Identity<binary_expression<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<0>>::functor_type>, this_type>::type;
		using Ddiv = typename BC_Substitute_Type::Identity<binary_expression<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<0>>::functor_type>, this_type>::type;
		using Dmul = typename BC_Substitute_Type::Identity<binary_expression<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<0>>::functor_type>, this_type>::type;


		template<int... any>
		struct D {
			using Shape = Static_Inner_Shape<0, any...>;
		};
	};
public:

		template<class U, class deriv, class inner, class outer>
		auto operator = (const Tensor_Mathematics_Head<U, deriv, math_lib, inner, outer>& tensor){
			if (this->size() != tensor.size()) {
				throw std::invalid_argument("pointwise operation size mismatch");
			}
			math_lib::copy(this->data(), tensor.data(), this->size());
			this->size() > OPENMP_SINGLE_THREAD_THRESHHOLD ?
							math_lib::copy(this->data(), tensor.data(), this->size()) :
							math_lib::copy_single_thread(this->data(), tensor.data(), this->size());

			return this->eval();
		}


//pointwise methods

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::add operator + (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& tensor) {
			return typename expression_tensor<U, deriv>::add(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::sub operator - (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& tensor) {
			return typename expression_tensor<U, deriv>::sub(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::div operator / (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& tensor) {
			return typename expression_tensor<U, deriv>::div(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::mul operator % (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& tensor) {
			return typename expression_tensor<U, deriv>::mul(this->data(), tensor.data());
		}

//Code for handling dynamic Tensors

		template<class U, class deriv, class ld, class inner>
		typename expression_tensor<U, deriv>::Dadd operator + (const Tensor_Mathematics_Head<U, deriv, math_lib, inner, ld>& tensor) {
			if (this->size() != tensor.size()) {
				throw std::invalid_argument("pointwise operation size mismatch");
			}
			return typename expression_tensor<U, deriv>::add(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld, class inner>
		typename expression_tensor<U, deriv>::Dsub operator - (const Tensor_Mathematics_Head<U, deriv, math_lib, inner, ld>& tensor) {
			if (this->size() != tensor.size()) {
				throw std::invalid_argument("pointwise operation size mismatch");
			}
			return typename expression_tensor<U, deriv>::sub(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld, class inner>
		typename expression_tensor<U, deriv>::Ddiv operator / (const Tensor_Mathematics_Head<U, deriv, math_lib, inner, ld>& tensor) {
			if (this->size() != tensor.size()) {
				throw std::invalid_argument("pointwise operation size mismatch");
			}
			return typename expression_tensor<U, deriv>::div(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld, class inner>
		typename expression_tensor<U, deriv>::Dmul operator % (const Tensor_Mathematics_Head<U, deriv, math_lib, inner, ld>& tensor) {
			if (this->size() != tensor.size()) {
				throw std::invalid_argument("pointwise operation size mismatch");
			}
			return typename expression_tensor<U, deriv>::mul(this->data(), tensor.data());
		}


//Scalar methods //Any class that has a size of 1 functions like a Scalar class
		template<class U, class deriv, class inner, class ld>
		typename expression_tensor<U, deriv>::add_scalar operator + (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& tensor) {
			return typename expression_tensor<U, deriv>::add_scalar(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::sub_scalar operator - (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& tensor) {
			return typename expression_tensor<U, deriv>::sub_scalar(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::div_scalar operator / (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& tensor) {
			return typename expression_tensor<U, deriv>::div_scalar(this->data(), tensor.data());
		}

		template<class U, class deriv, class ld>
		typename expression_tensor<U, deriv>::mul_scalar operator % (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& tensor) {
			return typename expression_tensor<U, deriv>::mul_scalar(this->data(), tensor.data());
		}
};
}

/*
 * What's happening?
 *
 * 	Each of these pointwise operator accepts a Tensor_Base<U (anytype), deriv (any derived), math_lib(same), dimensions...(same)>
 *  and returns a derived class of type binary_expression<T, math_function, functor_type, param::functor_type>.
 *
 *  The replace_front metatemplate function is called to assist in the conversion from type T to binary_expression.
 *  IE Vector<T, 100> will be replaced to Vector<bin_expr, 100>.
 */


/*
 *
 * VERBOSE DEPRECATED VARIANTS
 *
 */

//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type  //return object
//	operator + (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}

//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type  //return object
//	operator - (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}
//
//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type  //return object
//	operator / (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}
//
//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type  //return object
//	operator % (const Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, inner_shape, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}

/*
 * Verbose Scalar Methods
 */

//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type  //return object
//	operator + (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::add, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}
//
//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type  //return object
//	operator - (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::sub, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}
//
//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type  //return object
//	operator / (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::div, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}
//
//	template<class U, class deriv, class ld>
//	typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type  //return object
//	operator % (const Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>& rv) {
//		return typename BC_Substitute_Type::Identity<binary_expression_scalar_R<T, BC::mul, functor_type, typename Tensor_Mathematics_Head<U, deriv, math_lib, Static_Inner_Shape<1>, ld>::functor_type>, this_type>::type(this->data(), rv.data());
//	}

#endif /* TENSOR_LV4_MATH_HEAD_H_ */
