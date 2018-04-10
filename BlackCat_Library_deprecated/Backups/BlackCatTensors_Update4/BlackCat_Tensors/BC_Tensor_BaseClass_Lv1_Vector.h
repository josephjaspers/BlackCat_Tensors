/*
 * BC_Tensor_Vector.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef BC_TENSOR_BASECLASS_LV1_VECTOR_H_
#define BC_TENSOR_BASECLASS_LV1_VECTOR_H_

#include "BC_Tensor_InheritLv4_Queen.h"
#include "BC_Tensor_BaseClass_Lv0_Scalar.h"
#include "BC_Tensor_InheritLv5_Core.h"

template<class T = double, class ml = CPU, int... dimensions>
class Vector;

template<class T, class ml, int row>
class Vector<T, ml, row> : public Tensor_Core<T, ml, row> {
public:

	Vector<T, ml, row>() {};
	Vector<T, ml, row>(T* vector) : Tensor_Core<T, ml, row>(vector) {};

	Scalar<T, ml> operator [] (int index) { return Scalar<T, ml>(&this->data()[index]); }
	const Scalar<T, ml> operator [] (int index) const { return Scalar<T, ml>(this->data()[index]); }

	Vector<T, ml, row>& operator = (const Tensor_Ace<T, ml, row>& tens) {
		ml::copy(this->data(), tens.data(), this->size());
		return * this;
	}

	template<class U>
	Vector<T, ml, row>& operator = (const Tensor_King<U, ml, row>& tens) {
//		std::cout << " this is better " << std::endl;

//#pragma omp parallel sections
//	{
//		BC_CPU_CopySpecializations::o<row>::i_copy(this->data(), tens.data());
//	}
//#pragma omp barrier
		//ml::copy(this->data(), tens.data(), this->size());

#pragma omp parallel for
		for (int i = 0; i < this->size(); ++i) {
			this->data()[i] = tens.data()[i];
		}
#pragma omp barrier
		return * this;
	}

	const Vector<T, ml, 1, row> t() const {
		return Vector<T, ml, 1, row>(const_cast<T*>(this->data()));
	}

};

//Row Vector specialization
template<class T, class ml, int row>
class Vector<T, ml, 1, row> : public Tensor_Core<T, ml, 1, row> {
public:

	Vector<T, ml, 1, row>() {};
		Vector<T, ml, 1, row>(T* vector) : Tensor_Core<T, ml, 1, row>(vector) {}

	Scalar<T, ml> operator [] (int index) { return Scalar<T, ml>(&this->data()[index]); }
	const Scalar<T, ml> operator [] (int index) const { return Scalar<T, ml>(this->data()[index]); }

	const Vector<T, ml, row> t() const {
		return Vector<T, ml, row>(this->data());
	}

	Vector<T, ml, row>& operator = (const Tensor_King<T, ml, row>& tens) {

		ml::copy(this->data(), tens.data(), this->size());
		return * this;
	}

	template<class U>
	Vector<T, ml, row>& operator = (const Tensor_King<U, ml, row>& tens) {

#pragma omp parallel sections
	{
		BC_CPU_CopySpecializations::o<this->size()>::o_copy(this->data(), tens.data());
	}
		//ml::copy(this->data(), tens.data(), this->size());
		return * this;
	}
};


#endif /* BC_TENSOR_BASECLASS_LV1_VECTOR_H_ */
