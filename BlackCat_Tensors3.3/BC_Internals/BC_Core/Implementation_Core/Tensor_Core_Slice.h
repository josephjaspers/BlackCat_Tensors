/*
 * Tensor_Slice.cu
 *
 *  Created on: Feb 25, 2018
 *      Author: joseph
 */

#ifndef TENSOR_SLICE_CU_
#define TENSOR_SLICE_CU_

#include "Tensor_Core_Interface.h"
#include "BC_Expressions/Expression_Base.h"
#include "Determiners.h"
#include <iostream>
#include "Tensor_Core_Scalar.h"
#include "Tensor_Core_RowVector.h"
namespace BC {


template<int i>
static constexpr int oneLess = i - 1 > 0 ? i - 1 : 0;


template<class PARENT>
	struct Tensor_Slice : Tensor_Core_Base<Tensor_Slice<PARENT>, oneLess<PARENT::DIMS()>> {

	using scalar_type = _scalar<PARENT>;

	const PARENT parent;
	scalar_type* array_slice;

	operator const PARENT	   () const	{ return parent;	  }

	__BCinline__ Tensor_Slice(const scalar_type* array, PARENT parent_)
						: array_slice(const_cast<scalar_type*>(array)), parent(parent_) {}
	__BCinline__ const auto innerShape() const 			{ return parent.innerShape(); }
	__BCinline__ const auto outerShape() const 			{ return parent.outerShape(); }

	__BCinline__ const scalar_type* getIterator() const { return array_slice; }
	__BCinline__	   scalar_type* getIterator()   	{ return array_slice; }

	};
}

#endif /* TENSOR_SLICE_CU_ */
