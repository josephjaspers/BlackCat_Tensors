///*
// * BC_Correlation.h
// *
// *  Created on: Feb 25, 2018
// *      Author: joseph
// */
//
//#ifndef BC_CORRELATION_H_
//#define BC_CORRELATION_H_
//
//#include "../BlackCat_Tensors.h"
//#include <vector>
//
//namespace BC {
//template<int> struct cor;
//
//struct corr_mul {
//	template<class T, class U>
//	__BC_gcpu__ inline __attribute__((always_inline))  T operator () (T t, U u) const {
//		return t * u;
//	}
//};
//struct axpy {
//	template<class T, class U>
//	__BC_gcpu__ inline __attribute__((always_inline))  T operator () (T& t, U u) const {
//		return t += u;
//	}
//};
//
//
//
//using _shape = std::vector<int>;
//
//template<int movements = 2, class d1, class d2>
//auto x_corr(const TensorBase<d1>& krnl, const TensorBase<d2>& img) {
//	using output_tensor = typename base<movements>::type<_scalar<d1>, _mathlib<d1>>;
//	//initialize dimensionality of the tensor
//	_shape dimensions(movements);
//	for (int i = 0; i < movements; ++i)
//		dimensions[i] = img.dimension(i) - krnl.dimension(i) + 1;
//
//	//initialize the output tensor
//	output_tensor tensor(dimensions);
//	cor<movements>::x_corr(tensor, krnl, img);
//	return tensor;
//
//}
//
//
//template<>
//struct cor<0> {
//template<class d0, class d1, class d2>
//static auto x_corr(const TensorBase<d0>& out, const TensorBase<d1>& krnl, const TensorBase<d2>& img, int size) {
//
//		auto expr = krnl.binExpr(corr_mul(), img);
//		for (int i = 0; i < size; ++i) {
//			expr[i];//iterate through the krnl to enforce evaluation
//		}
//}
//};
//
//}
//
//#endif /* BC_CORRELATION_H_ */
