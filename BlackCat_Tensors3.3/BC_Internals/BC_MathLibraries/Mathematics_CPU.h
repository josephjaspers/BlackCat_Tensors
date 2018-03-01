/*
 * BC_Mathematics_CPU.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef MATHEMATICS_CPU_H_
#define MATHEMATICS_CPU_H_

#include <cmath>
#include <iostream>
#include <string>

#include "BC_PrintFunctions.h"
#include "cblas.h"

namespace BC {

static constexpr int MAX_UNROLLED = 256;
static constexpr int DEFAULT_ROLL = 8;

class CPU {
public:
	template<typename T>
	static void initialize(T*& t, int sz) {
		t = new T[sz];
	}
	template<typename T>
	static void unified_initialize(T*& t, int sz) {
		t = new T[sz];
	}
	//The tensor library doesn't differentiate method calls between gpu and cpu libraries, ergo you must override certain methods.
	template<class T, class U>
	static void HostToDevice(T& t, U& u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void DeviceToHost(T& t, U& u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void HostToDevice(T* t, U* u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void DeviceToHost(T* t, U* u, int size) {
		copy(t, u, size);
	}
	template<typename T>
	static void destroy(T* t) {
		delete[] t;
	}
	template<typename T, typename J, int roll = DEFAULT_ROLL>
	static void fill(T& t, const J j, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i] = j;
		}
	}

	template<typename T>
	static void eval(T& t, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i];
		}
	}
	template<typename T, typename J>
	static void set_heap(T *t, J *j) {
		*t = *j;
	}
	template<typename T, typename J>
	static void set_stack(T *t, J j) {
		*t = j;
	}

	template<typename T>
	static void zero(T& t, int sz) {
		fill(t, 0, sz);
	}





	template<typename T, typename J>
	static void copy(T& t, const J& j, int sz) {
		if (sz < 8192) {
			copy_single_thread(t, j, sz);
			return;
		}
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}


	template<typename T, typename J, int roll = DEFAULT_ROLL>
	static void copy_single_thread(T& t, const J& j, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
	}
	template<typename T, typename J>
	static void copyHostToDevice(T t, J j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}

	template<typename T, typename J>
	static void randomize(T& t, J lower_bound, J upper_bound, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = ((double) (rand() / ((double) RAND_MAX + 1)) * (upper_bound - lower_bound)) + lower_bound;
		}
#pragma omp barrier
	}
	template<class T, class RANKS>
	static void print(const T ary, const RANKS ranks, int order, int print_length) {
		BC::print(ary, ranks, order, print_length);
	}

	template<class T, class RANKS>
	static void printSparse(const T ary, const RANKS ranks, int order, int print_length) {
		BC::printSparse(ary, ranks, order, print_length);
	}



	/*
	 * a = M x K
	 * b = K x N
	 * c = M x N
	 */

	//
public:
	template<class U, class T, class V>
	static void scalarMul(U eval, T a, V b) {
		*eval = a[0] * b[0];
	}

	static void MatrixMul(bool transA, bool transB, const float* A, const float* B, float* C, int m, int n, int k,
			const float* scalarA = nullptr, const float* scalarB = nullptr,  int lda = 0, int ldb =0, int ldc =0) {

		 auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
		 auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

		  if (lda == 0 ) lda = m;
		  if (ldb == 0 ) ldb = n;
		  if (ldc == 0 ) ldc = m;

	      const float beta   =  scalarB ? *scalarB : 0;
		  const float alpha  =  scalarA ? *scalarA : 1;

		cblas_sgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}
	static void MatrixMul(bool transA, bool transB, const double* A, const double* B, double* C, int m, int n, int k,
			const double* scalarA = nullptr, const double* scalarB = nullptr, int lda = 0, int ldb = 0, int ldc = 0) {

		 auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
		 auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

		  if (lda == 0 ) lda = m;
		  if (ldb == 0 ) ldb = n;
		  if (ldc == 0 ) ldc = m;


		const double beta   =  scalarB ? *scalarB : 0.0;
		const double alpha  =  scalarA ? *scalarA : 1.0;
		cblas_dgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	//padded correlation
	template<class V, class T, class U>
	static void x_correlation(V& cor, const T& krnl, const U& img, int krnl_rank, const int* krnl_shape, int img_size, int img_rows) {

		int krnl_col_count = 1;
		for (int i = 1; i < krnl_rank; ++i) {
			krnl_col_count *= krnl_shape[i];
		}

		const int krnl_rows = krnl_shape[0];
		const int cor_rows = krnl_rows + img_rows - 1;
		int base_ = krnl_rows - 1;

		//handles the initial edge case
		for (int i = 0; i < base_; ++i){
			for (int j = 0; j < krnl_col_count; ++j)
				for (int k = 0; k > -1; --k) {
					cor[i + (krnl_col_count - 1 - j) * cor_rows] += krnl[krnl_rows - k - 1 + j * krnl_rows] * img[i + k];
			}
		}


		//handles the rest of the kernel
		for (int i = 0; i < img_size; ++i) {
				bool incremented = false;
					for (int j = 0; j < krnl_col_count; ++j) {

						if (incremented) {
							incremented = false;
							base_--;
						}

						for (int k = 0; k < krnl_rows; ++k) {

						if (k != 0 && !incremented)
							if (std::floor(i / img_rows) < std::floor((i + k)/ img_rows)) {
								incremented = true;
								base_++;
							}

						cor[i+ j * cor_rows + base_] += krnl[k + (krnl_col_count - j - 1)  * krnl_rows] * img[i + k];
				}
			}
		}
	}
	//non - padded
	template<class V, class T, class U>
		static void x_correlation_in(V& cor, const T& krnl, const U& img, int krnl_rank, const int* krnl_shape, int img_size, int img_rows) {

			int krnl_col_count = 1;
			for (int i = 1; i < krnl_rank; ++i) {
				krnl_col_count *= krnl_shape[i];
			}

		const int krnl_rows = krnl_shape[0];
		const int cor_rows =  img_rows - krnl_rows + 1;
		const int positions = img_rows - krnl_rows + 1;
		int cor_base = 0;
		//handles the rest of the kernel
		for (int i = 0; i < img_size; ++i) {
			for (int j = 0; j < krnl_col_count; ++j)
			for (int x = i; x < positions + i; ++x) {
					for (int k = 0; k < krnl_rows; ++k) {
						cor[cor_base + x + j * cor_rows] += krnl[k + j * krnl_rows] * img[x + k + j * img_rows];
					}
			}
//			i += krnl_rows;
//			cor_base -= krnl_rows;
		}
	}

};
}
#endif /* MATHEMATICS_CPU_H_ */
//};
//
//
//	//This doesn't actually give a shit if transposed --- just using this till I integrate blas
//	template<class A, class B, class C, class D, class E>
//	static void MatrixMul(bool transA, bool transB, const A a, const B b,  C c, int m, int n, int k, const D scalarA = nullptr, const E scalarB = nullptr,  int lda = 0, int ldb = 0, int ldc = 0) {
//		if (lda == 0 and ldb == 0 and ldc ==0) {
//			lda = m;
//			ldb = n;
//			ldc = m;
//		}
//
//		typename MTF::remove_mods<A>::type alpha =  scalarA == nullptr  ? 1 : *scalarA;
//		typename MTF::remove_mods<B>::type beta  =  scalarB == nullptr  ? 1 : *scalarB;
//
//#pragma omp parallel for
//		for (int z = 0; z < k; ++z) {
//			for (int x = 0; x < n; ++x) {
//				for (int y = 0; y < m; ++y) {
//					c[z * ldc + y] += (a[x * lda + y]* alpha) * (b[z * ldb + x] * beta);
//
//				}
//			}
//		}
//#pragma omp barrier
//	}
//
//	template<class T>
//	static void MatrixMul(bool transA, bool transB, const T a, const T b,  T c, int m, int n, int k, const T scalarA = nullptr, const T scalarB = nullptr,  int lda = 0, int ldb = 0, int ldc = 0) {
//		if (lda == 0 and ldb == 0 and ldc ==0) {
//			lda = m;
//			ldb = n;
//			ldc = m;
//		}
//
//		typename MTF::remove_mods<T>::type alpha =  scalarA == nullptr  ? 1 : *scalarA;
//		typename MTF::remove_mods<T>::type beta  =  scalarB == nullptr  ? 1 : *scalarB;
//
//#pragma omp parallel for
//		for (int z = 0; z < k; ++z) {
//			for (int x = 0; x < n; ++x) {
//				for (int y = 0; y < m; ++y) {
//					c[z * ldc + y] += (a[x * lda + y] * scalarA) * (b[z * ldb + x] * scalarB);
//
//				}
//			}
//		}
//#pragma omp barrier
//	}
