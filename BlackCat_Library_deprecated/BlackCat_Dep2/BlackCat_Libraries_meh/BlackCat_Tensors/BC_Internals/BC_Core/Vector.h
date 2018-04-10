/*
 * Vector.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Tensor_Lv3_Base.h"
#include "Tensor_IdentityClasses.h"
#include "../BC_Shape/Static_Shape.h"

namespace BC {
template<class T, class Mathlib = CPU>
class Vector: public Tensor_Base<T, VECTOR, Vector<T, Mathlib>, Mathlib>
{

	using parent_class = Tensor_Base<T, VECTOR, Vector<T, Mathlib>, Mathlib>;

public:


	template<class U>
	Vector<T, Mathlib>& operator =(const Vector<U, Mathlib>& vec) {
		Mathlib::copy(this->data(), vec.data(), this->size());
		return *this;
	}

	Vector<T, Mathlib>& operator =(const Vector<T, Mathlib>& vec) {
		Mathlib::copy(this->data(), vec.data(), this->size());
		return *this;
	}
	template<class... params> Vector(const params&... p) : parent_class(p...) {}
	Vector(int rows) {}

};

} //End Namespace BC

#endif /* VECTOR_H_ */
