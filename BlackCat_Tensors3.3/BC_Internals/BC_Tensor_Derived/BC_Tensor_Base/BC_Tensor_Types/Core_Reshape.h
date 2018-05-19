
/*
 * Tensor_Reshape.h
 *
 *  Created on: Mar 31, 2018
 *      Author: joseph
 */

#ifndef TENSOR_RESHAPE_H_
#define TENSOR_RESHAPE_H_

#include "Core_Base.h"

namespace BC {
/*
 * Accepts an a tensor_core type wrapped in the new_tensor
 *
 * IE if you have a Vector<Core<Vector<float, ml>, ml>  and wish to Reshape to a Matrix
 * The resulting reshape will be-- Matrix<Tensor_Reshape<Matrix<Core<Vector<float, ml>,ml>>>>
 *
 * This is somewhat awkward and atypical of the other Core traits, but it is essential to be able to pass
 * the constexpr int DIMS in some form. The choice to utilize this method opposed to expanding the number of template arguments
 * was to ensure consistency across the determiners.h which are imperative to the template-metaprogramming.
 */

template<class PARENT>
struct Tensor_Reshape {

	template<int dimension>
	struct implementation : Core_Base<implementation<dimension>, dimension>{

	using scalar = _scalar<PARENT>;

	__BCinline__ static constexpr int DIMS() { return dimension; };
	__BCinline__ static constexpr int CONTINUOUS() { return dimension; }

	operator const PARENT() const	{ return parent; }

	PARENT parent;
	scalar* array;

	int is[DIMS()];
	int os[DIMS()];


	template<class... integers> __BCinline__
	implementation(scalar* array_, PARENT parent, integers... ints) : array(array_), parent(parent) {
		static_assert(sizeof...(integers) == DIMS(), "DIMENSIONALITY OF RESHAPE MUST EQUAL THE PARAMETER INTEGERS FOR RESHAPE");
		init<0>(ints...);
	}

	__BCinline__ const auto innerShape() const 	{ return is; }
	__BCinline__ const auto outerShape() const 	{ return os; }

	__BCinline__ const auto getIterator() const { return parent.getIterator(); }
	__BCinline__	   auto getIterator()   	{ return parent.getIterator(); }



	template<int dim, class... integers> __BCinline__
	void init(integers... ints) {

		auto vars = BC::array(ints...);

		is[0] = vars[0];
		os[0] = vars[0];
		for (int i = 1; i < vars.size(); ++i) {
			is[i] = vars[i];
			os[i] = is[i] * os[i - 1];
		}
	}

	};
};
}

#endif /* TENSOR_RESHAPE_H_ */