/*
 * Tensor_Slice.cu
 *
 *  Created on: Feb 25, 2018
 *      Author: joseph
 */

#ifndef TENSOR_SLICE_CU_
#define TENSOR_SLICE_CU_

#include "Core_Base.h"

namespace BC {

//Floored decrement just returns the max(param - 1, 0)

template<class PARENT>
	struct Tensor_Slice : Core_Base<Tensor_Slice<PARENT>, max(PARENT::DIMS() - 1, 0)> {

	using scalar_type = _scalar<PARENT>;

	__BCinline__ static constexpr int CONTINUOUS() { return max(PARENT::CONTINUOUS() - 1, 0); }
	__BCinline__ static constexpr int DIMS() { return max(PARENT::PARENT_DIMS() - 1, 0); }

	operator const PARENT() const	{ return parent; }

	const PARENT parent;
	scalar_type* array_slice;

	__BCinline__ Tensor_Slice(const scalar_type* array, PARENT parent_) : array_slice(const_cast<scalar_type*>(array)), parent(parent_) {}
	__BCinline__ const auto innerShape() const 			{ return parent.innerShape(); }
	__BCinline__ const auto outerShape() const 			{ return parent.outerShape(); }
	__BCinline__ const scalar_type* getIterator() const { return array_slice; }
	__BCinline__	   scalar_type* getIterator()   	{ return array_slice; }

	};
}

#endif /* TENSOR_SLICE_CU_ */