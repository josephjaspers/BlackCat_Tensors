/*
 * Tensor_Lv3_Base.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef TENSOR_LV3_BASE_H_
#define TENSOR_LV3_BASE_H_

#include "Tensor_Lv2_Core.h"

namespace BC {

template<class T, class mid_deriv, class Mathlib, bool Utility_Function_Supported = false>
struct Tensor_Base_impl {
};


template<class scalar_type, class deriv, class MATHLIB>
struct Tensor_Base_impl<scalar_type, deriv, MATHLIB, true> {
/*
 *  Tensor_Base specialization (for primary tensors -- we enable these utility methods)
 */

	deriv& asDerived() {
		return static_cast<deriv&>(*this);
	}
	const deriv& asDerived() const {
		return static_cast<const deriv&>(*this);
	}

	void randomize(scalar_type lb, scalar_type ub) {
		MATHLIB::randomize(asDerived().data(), lb, ub, asDerived().size());
	}
	void fill(scalar_type value) {
		MATHLIB::fill(asDerived().data(), value, asDerived().size());
	}
	void zero() {
		MATHLIB::zero(asDerived().data(), asDerived().size());
	}
	void print() const {

		asDerived();
		MATHLIB::print(asDerived().array, asDerived().ranks , asDerived().order(), 5);
	}

	Tensor_Base_impl() {
		if (asDerived().size() != 0)
		MATHLIB::initialize(asDerived().data(), asDerived().size());
	}

	~Tensor_Base_impl() {
		MATHLIB::destroy(static_cast<deriv&>(const_cast<Tensor_Base_impl&>(*this)).data());
	}
};



template<
	class scalar_type,

	template<class>
	class IDENTITY,
	class DERIVED,
	class MATHLIB>

struct Tensor_Base :

	public Tensor_Core<scalar_type, IDENTITY ,DERIVED, MATHLIB>,
	public Tensor_Base_impl<scalar_type, DERIVED, MATHLIB, MTF::isPrimitive<scalar_type>::conditional>
{
private:
	using primary_parent = Tensor_Core<scalar_type, IDENTITY ,DERIVED, MATHLIB>;
	using functor_type = typename primary_parent::functor_type;
	typedef std::initializer_list<int> _shape;
public:

	using primary_parent::primary_parent;
	using primary_parent::operator=;

	functor_type array;
		static constexpr int degree = IDENTITY<DERIVED>::RANK;
		int sz = 0;
		int* ranks;
		int* ld;

public:



		int size() const {
			return sz;
		}
		int order() const {
			 return degree;
		}




};

}


#endif /* TENSOR_LV3_BASE_H_ */
