/*
 * BC_Tensor_Inheritv4_Queen.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef BC_TENSOR_INHERITLV4_QUEEN_H_
#define BC_TENSOR_INHERITLV4_QUEEN_H_

#include "BC_Tensor_InheritLv3_BasicOperands.h"

/*
 * Defines Utility Methods, may or may not be extended by depending upon type
 */

template<class T, class ml, int ... dimensions>
class Tensor_Queen : public Tensor_King<T, ml, dimensions...> {

protected:
	template<class... params> inline __attribute__((always_inline)) Tensor_Queen<T, ml, dimensions...>(const params&... p) : Tensor_King<T, ml, dimensions...>(p...) {}
	template<class... params> inline __attribute__((always_inline)) Tensor_Queen<T, ml, dimensions...>(		 params&... p) : Tensor_King<T, ml, dimensions...>(p...) {}
							  inline __attribute__((always_inline)) Tensor_Queen<T, ml, dimensions...>() = default;
public:

	void fill(T value) {
		ml::fill(this->data(), value, this->size());
	}
	void zero() {
		ml::zero(this->data(), this->size());
	}
	void randomize(T lb, T ub) {
		ml::randomize(this->data(), lb, ub, this->size());
	}
	void print() const {
		int* ranks = new int[sizeof...(dimensions)];
		printHelper::f<dimensions...>().fill(ranks);
		ml::print(this->array, ranks, sizeof...(dimensions), 5);
		delete[] ranks;
	}
};

#endif /* BC_TENSOR_INHERITLV4_QUEEN_H_ */
