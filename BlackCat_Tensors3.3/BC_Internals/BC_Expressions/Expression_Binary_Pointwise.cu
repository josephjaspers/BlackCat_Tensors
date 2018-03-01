/*
 * BC_Expression_Binary.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */
#ifdef  __CUDACC__
#ifndef EXPRESSION_BINARY_POINTWISE_SAME_H_
#define EXPRESSION_BINARY_POINTWISE_SAME_H_


#include "Expression_Base.cu"
namespace BC {


template<class T, class op, class lv, class rv>
class binary_expression;

template<class T, class operation, class lv, class rv>
struct binary_expression : public expression<T, binary_expression<T, operation, lv, rv>> {

	operation oper;

	const lv& left;
	const rv& right;

	template<class L, class R>
	inline __attribute__((always_inline)) binary_expression(const L& l, const R& r) :
			left(l), right(r) {
	}

	inline __attribute__((always_inline)) __BC_gcpu__ auto operator [](int index) const -> decltype(oper(left[index], right[index])) {
		return oper(left[index], right[index]);
	}

	int rank() const { return right.rank(); }
	int rows() const { return right.rows(); };
	int cols() const { return right.cols(); };
	int size() const { return right.size(); };
	int LD_rows() const { return right.LD_rows(); }
	int LD_cols() const { return right.LD_cols(); }
	int dimension(int i)		const { return right.dimension(i); }
	void printDimensions() 		const { right.printDimensions();   }
	void printLDDimensions()	const { right.printLDDimensions(); }
	auto accessor_packet(int index) const { return right.accessor_packet(index); }
	const auto innerShape() const { return right.innerShape(); }
	const auto outerShape() const { return right.outerShape(); }

};


}

#endif /* EXPRESSION_BINARY_POINTWISE_SAME_H_ */
#endif
