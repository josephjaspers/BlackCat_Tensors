/*
 * BC_Mathematics_CPU.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef BC_MATHEMATICS_CPU_H_
#define BC_MATHEMATICS_CPU_H_

#include <cmath>
#include <iostream>
#include <string>
#include "BC_Expression_Binary_Functors.h"
#include "BC_Expression_Binary_Pointwise_Same.h"

namespace BC_CPU_CopySpecializations {

	template<int sz, typename T, typename J>
	struct opt_copyer {

		inline __attribute__((always_inline))
		static void copy(T& t, const J& j, int id) {

//			t[id + sz - 1] = j.left.left[id + sz - 1] + j.left.right[id + sz - 1] + j.right[id + sz - 1];
//			t[id + sz - 2] = j.left.left[id + sz - 2] + j.left.right[id + sz - 2] + j.right[id + sz - 2];
//			t[id + sz - 3] = j.left.left[id + sz - 3] + j.left.right[id + sz - 3] + j.right[id + sz - 3];
//			t[id + sz - 4] = j.left.left[id + sz - 4] + j.left.right[id + sz - 4] + j.right[id + sz - 4];
//			t[id + sz - 5] = j.left.left[id + sz - 5] + j.left.right[id + sz - 5] + j.right[id + sz - 5];
//			t[id + sz - 6] = j.left.left[id + sz - 6] + j.left.right[id + sz - 6] + j.right[id + sz - 6];
//			t[id + sz - 7] = j.left.left[id + sz - 7] + j.left.right[id + sz - 7] + j.right[id + sz - 7];
//			t[id + sz - 8] = j.left.left[id + sz - 8] + j.left.right[id + sz - 8] + j.right[id + sz - 8];

//			t[id + sz - 1] = j.left.left[id + sz - 1]+j.left.right[id + sz - 1] + j.right[id + sz - 1];
//			t[id + sz - 2] = j[id + sz - 2];
//			t[id + sz - 3] = j[id + sz - 3];
//			t[id + sz - 4] = j[id + sz - 4];
//			t[id + sz - 5] = j[id + sz - 5];
//			t[id + sz - 6] = j[id + sz - 6];
//			t[id + sz - 7] = j[id + sz - 7];
//			t[id + sz - 8] = j[id + sz - 8];

			opt_copyer<sz - 8, T, J>::copy(t, j, id);
		}

	};
	template<typename T, typename J>
	struct opt_copyer<0, T, J> {

		static void copy(T& t, const J& j, int id) {

		}

	};

	template<int ary_sz>
	struct o {
		template<class T, class U>
		static void i_copy(T t, U u) {
#pragma omp parallel sections
			{
#pragma omp section
				{
					o<ary_sz / 10>::o_copy(&t[0], u);
					o<ary_sz / 10>::o_copy(&t[ary_sz * 1 / 10], u);
				}
#pragma omp section
				{
					o<ary_sz / 10>::o_copy(&t[ary_sz * 3 / 10], u);
					o<ary_sz / 10>::o_copy(&t[ary_sz * 4 / 10], u);
				}
#pragma omp section
				{
					o<ary_sz / 10>::o_copy(&t[ary_sz * 6 / 10], u);
					o<ary_sz / 10>::o_copy(&t[ary_sz * 7 / 10], u);

				}
#pragma omp section
				{
					o<ary_sz / 10>::o_copy(&t[ary_sz * 8 / 10], u);
					o<ary_sz / 10>::o_copy(&t[ary_sz * 9 / 10], u);
				}
#pragma omp section
				{
					o<ary_sz / 10>::o_copy(&t[ary_sz * 5 / 10], u);
					o<ary_sz / 10>::o_copy(&t[ary_sz * 2 / 10], u);
				}
				/* ... */
			}

		}

		template<class T, class U>
static void o_copy(T t, U u) {
			o<ary_sz / 10>::o_copy(&t[0], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 1 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 2 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 3 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 4 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 5 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 6 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 7 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 8 / 10], u);
			o<ary_sz / 10>::o_copy(&t[ary_sz * 9 / 10], u);
		}
	};

	template<>
	struct o<10> {
		template<class T, class U>
		static void o_copy(T t, U u) {
			t[0] = u[0];
			t[1] = u[1];
			t[2] = u[2];
			t[3] = u[3];
			t[4] = u[4];
			t[5] = u[5];
			t[6] = u[6];
			t[7] = u[7];
			t[8] = u[8];
			t[9] = u[9];
		}
	};

	template<typename T, typename J>
	struct copyer {
		static void copy(T& t, const J& j, int sz) {
			const int eval_dist = 64;
#pragma omp parallel for schedule(static)
			for (int i = 0; i < sz - eval_dist; i += eval_dist) {
				BC_CPU_CopySpecializations::opt_copyer<eval_dist, T, J>::copy(t, j, i);
			}
			for (int i = sz - eval_dist; i < sz; ++i) {
				t[i] = j[i];
			}

#pragma omp barrier
		}
	};
}

class CPU {
public:
	/*
	 * T -- An object (Generally an array) with a functional [] operator
	 * J -- Either a value on the stack to be assigned or another array similar to above.
	 *
	 * J -- may also be a single value passed by pointer, certains methods are overloaded to handle these instance
	 *
	 */

	//destructor - no destructor are for controlling destruction of the pointer
	template<typename T>
	static void initialize(T*& t, int sz) {
		t = new T[sz];
	}

	template<typename T>
	static void initialize(T t, int sz) {
		std::cout << "invalid overload called " << std::endl;
	}
	template<typename T>
	static void unified_initialize(T*& t, int sz) {
		t = new T[sz];
	}
	template<typename T>
	static void destroy(T* t) {
		delete[] t;
	}
	template<typename T, typename J>
	static void fill(T& t, const J j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j;
		}
#pragma omp barrier
	}
	template<typename T, typename J>
	static void fill(T& t, const J* j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = *j;
		}
#pragma omp barrier

	}
	template<typename T>
	static void zero(T& t, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = 0;
		}
#pragma omp barrier
	}

	template<typename T, typename J>
	static void copy(T& t, J j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier

		//BC_CPU_CopySpecializations::copyer<T, J>::copy(t, j, sz);

	}

	template<typename T, typename J>
	static void copy_stack(T& t, J j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}
	template<typename T, typename J>
	static void evaluate(T& t, const J& j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}
	template<typename T, typename J>
	static void randomize(T t, J lower_bound, J upper_bound, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = ((double) (rand() / ((double) RAND_MAX + 1)) * (upper_bound - lower_bound)) + lower_bound;

		}
#pragma omp barrier
	}
	template<typename T, typename J>
	static void sum(T* sum, J ary, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			*sum += ary[i];
		}
#pragma omp barrier
	}

private:
	template<typename T>
	static void printHelper(const T ary, const int* ranks, int order, std::string indent, int printSpace) {
		--order;

		if (order > 1) {
			std::cout << indent << "--- --- --- --- --- " << order << " --- --- --- --- ---" << std::endl;
			indent += "    "; //add a tab to the index

			for (int i = 0; i < ranks[order]; ++i) {
				printHelper(&ary[i * ranks[order - 1]], ranks, order, indent, printSpace);
			}

			auto gap = std::to_string(order);
			auto str = std::string(" ", gap.length());
			std::cout << indent << "--- --- --- --- --- " << str << " --- --- --- --- ---" << std::endl;

		} else if (order == 1) {

//			std::string indent2 = "  ";
//			for (int i = 0; i < ranks[order]; ++i) {
//
//				printHelper(&ary[i * ranks[order - 1]], ranks, order, indent2, printSpace);
//				std::cout << std::endl;
//			}

			for (int j = 0; j < ranks[order - 1]; ++j) {
				std::cout << "[ ";

				for (int i = 0; i < ranks[order]; ++i) {
					//convert to string --- seems to not be working with long/ints?
					auto str = std::to_string(ary[i * ranks[order - 1] + j]);

					//if the string is longer than the printspace truncate it
					str = str.substr(0, str.length() < printSpace ? str.length() : printSpace);

					//if its less we add blanks (so all numbers are uniform in size)
					if (str.length() < printSpace)
						str += std::string(" ", printSpace - str.length());

					//print the string
					std::cout << str;

					//add some formatting fluff
					if (i < ranks[order] - 1)
						std::cout << " | ";

				}
				std::cout << " ]";
				std::cout << std::endl;
			}
		} else {
			std::cout << "[ ";
			for (int i = 0; i < ranks[order]; ++i) {
				//convert to string --- seems to not be working with long/ints?
				auto str = std::to_string(ary[i]);

				//if the string is longer than the printspace truncate it
				str = str.substr(0, str.length() < printSpace ? str.length() : printSpace);

				//if its less we add blanks (so all numbers are uniform in size)
				if (str.length() < printSpace)
					str += std::string(" ", printSpace - str.length());

				//print the string
				std::cout << str;

				//add some formatting fluff
				if (i < ranks[order] - 1)
					std::cout << " | ";
			}
			std::cout << " ]";
		}
	}
public:
	template<typename T>
	static void print(const T ary, const int* ranks, int order, int print_length) {
		std::string indent = "";
		printHelper(ary, ranks, order, indent, print_length);

		std::cout << std::endl;
	}
	template<typename T>
	static void print(const T ary, int size, int printSpace) {
		std::cout << "[ ";
		for (int i = 0; i < size; ++i) {
			auto str = std::to_string(ary[i]);
			str = str.substr(0, str.length() < printSpace ? str.length() : printSpace);
			std::cout << str;

			if (i < size - 1) {
				std::cout << " | ";
			}

		}
		std::cout << " ]" << std::endl;

	}
	template<typename T>
	static void print(T ary, int size) {
		print(ary, size, 5);
	}
	template<typename T>
	static void sigmoid(T ary, int sz) {
		for (int i = 0; i < sz; ++i) {
			ary[i] = 1 / (1 + std::pow(2.71828, -ary[i]));
		}
	}
	/*
	 * a = M x N
	 * b = N x K
	 * c = M x K
	 */

	template<class A, class B, class C>
	static void matmul(C c, A a, int m, int n, B b, int k) {
#pragma omp parallel for
		for (int z = 0; z < k; ++z) {
			for (int x = 0; x < n; ++x) {
				for (int y = 0; y < m; ++y) {
					c[z * m + y] += a[x * m + y] * b[z * n + x];

				}
			}
		}
#pragma omp barrier
	}

	template<class T>
	static void matmulBLAS(T* c, const T* a, int m, int n, const T* b, int k) {

#pragma omp parallel for
		for (int z = 0; z < k; ++z) {
			for (int x = 0; x < n; ++x) {
				for (int y = 0; y < m; ++y) {
					c[z * m + y] += a[x * m + y] * b[z * n + x];

				}
			}
		}
#pragma omp barrier
	}
	static void matmulBLAS(double* c, const double* a, int m, int n, const double* b, int k);
	static void matmulBLAS(float* c, const float* a, int m, int n, const float* b, int k);
	static void matmulBLAS(double* c, const double* a, bool a_transposed, int m, int n, const double* b, bool b_transposed, int k);
	static void matmulBLAS(float* c, const float* a, bool a_transposed, int m, int n, const float* b, bool b_transposed, int k);
}
;
#endif /* BC_MATHEMATICS_CPU_H_ */
