#include <iostream>
#include <omp.h>
#include "math.h"
#include "time.h"
#include <stdio.h>
#include "BC_Tensor_Primary_Vector.h"
#include "BC_Tensor_Primary_Matrix.h"

#include "BC_Tensor_Super_Queen.h"

template<int sz, class T>
inline __attribute__((always_inline)) void add(T* eval_to, T* a, T* b) {
	eval_to[sz] = a[sz] + b[sz];
	eval_to[sz - 1] = a[sz - 1] + b[sz - 1];
	eval_to[sz - 2] = a[sz - 2] + b[sz - 2];
	eval_to[sz - 3] = a[sz - 3] + b[sz - 3];

	add<sz - 4, T>(eval_to, a, b);
}
template<>
inline __attribute__((always_inline)) void add<0, double>(double* eval_to, double* a, double* b) {
	eval_to[0] = a[0] + b[0];
}

void add(double* a, double* b, double* c, int sz) {
	const int eval_dist = 64;

#pragma omp parallel for schedule(static)

	for (int i = 0; i < sz - eval_dist; i += eval_dist) {
		add<eval_dist, double>(&a[i], &b[i], &c[i]);
	}
	for (int i = sz - eval_dist; i < sz; i++) {
		a[i] = b[i] + c[i];
	}
#pragma omp barrier
}
//Generic add two arrays together
void dumb_add(double* a, double* b, double* c, int sz) {
#pragma omp parallel for schedule(static)

	for (int i = 0; i < sz; ++i) {
		a[i] = b[i] + c[i];
	}

#pragma omp barrier
}

//Generic add two arrays together
void dumb_add_unrolled(double* a, double* b, double* c, int sz) {
#pragma omp parallel for schedule(static)

	for (int i = 0; i < sz; i += 2) {
		a[i] = b[i] + c[i];
		a[i + 1] = b[i + 1] + c[i + 1];
//		a[i + 2] = b[i + 2] + c[i + 2];
//		a[i + 3] = b[i + 3] + c[i + 3];
//		a[i + 4] = b[i + 4] + c[i + 4];
//		a[i + 5] = b[i + 5] + c[i + 5];
//		a[i + 6] = b[i + 6] + c[i + 6];
//		a[i + 7] = b[i + 7] + c[i + 7];

	}

#pragma omp barrier
}
//Generic add two arrays together
void dumb_add_unrolled(double* a, double* b, double* c, double* d, int sz) {
#pragma omp parallel for schedule(static)

	for (int i = 0; i < sz; i += 2) {
		a[i] = b[i] + c[i] + d[i];
		a[i + 1] = b[i + 1] + c[i + 1] + d[i + 1];
//		a[i + 2] = b[i + 2] + c[i + 2];
//		a[i + 3] = b[i + 3] + c[i + 3];
//		a[i + 4] = b[i + 4] + c[i + 4];
//		a[i + 5] = b[i + 5] + c[i + 5];
//		a[i + 6] = b[i + 6] + c[i + 6];
//		a[i + 7] = b[i + 7] + c[i + 7];

	}

#pragma omp barrier
}
#include <cstdlib>
#include "BC_Tensor_Super_Jack.h"
//
//int main() {
//
//	Tensor_Queen<double, CPU, 10> a;
//	Tensor_Queen<double, CPU, 10> b;
//	Tensor_Queen<double, CPU, 10> c;
//
//	a.randomize(-3, 3);
//	b.randomize(-3, 3);
//	a.print();
//	b.print();
//
//	c = a + b;
//	c.print();
//
//}

int speedTests() {

	omp_set_num_threads(3);

	const int sz = 100000;
	const int reps = 10000;

	double* a = new double[sz];
	double* b = new double[sz];

	double* c = new double[sz];
	double* d = new double[sz];

	Vector<double, CPU, sz> at;
	Vector<double, CPU, sz> bt;
	Vector<double, CPU, sz> ct;

	at.randomize(0, 1000);
	bt.randomize(0, 1000);

	for (int i = 0; i < sz; ++i) {
		a[i] = 0;
		b[i] = 0;
		c[i] = rand() % 1000;
		d[i] = rand() % 1000;

	}

	float t;
	t = omp_get_wtime();
	printf("Calculating... Optimized through templates \n");
	for (int i = 0; i < reps; ++i)
		add(a, c, d, sz);

	t = omp_get_wtime() - t;
	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
	t = omp_get_wtime();
	printf("Calculating...dumb generic version\n");

	for (int i = 0; i < reps; ++i) {
		dumb_add(b, c, d, sz);
	}

	t = omp_get_wtime() - t;
	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));

	t = omp_get_wtime();
	printf("Calculating...dumb generic 0 unrolled version\n");

	for (int i = 0; i < reps; ++i)
		dumb_add_unrolled(b, c, d, b, sz);

	t = omp_get_wtime() - t;
	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));

	//attemptt o cahe
//	ct.data();
//	at.data();
//	bt.data();

	t = omp_get_wtime();
	printf("Calculating... Lazy expression \n");
	for (int i = 0; i < reps; ++i) {
		ct = at + bt + at;
	}

	t = omp_get_wtime() - t;
	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));

	std::cout << "success " << std::endl;

	for (int i = 0; i < sz; ++i) {
//		if (a[i] != b[i]) {
//			std::cout << " discrepency at " << i << std::endl;
//		}
	}
	delete[] a;
	delete[] b;
	delete[] c;

	return 0;
}

int transTest() {

	Vector<double, CPU, 10> vec;

	vec.print();
	vec.t().print();
	vec.randomize(-4, 5);

	std::cout << vec.t().rows() << " " << vec.t().cols() << std::endl;
	Matrix<double, CPU, 3, 5> matrix;
	Matrix<double, CPU, 5, 3> matrixA;
	Matrix<double, CPU, 5, 3> matrixT;

	std::cout << " generating transpose " << std::endl;
	matrix.t();
	std::cout << " generating transpose  end" << std::endl;
	matrix.randomize(-5, 5);

	matrixT = matrix.t();
	matrix.print();
	matrixT.print();

	matrixA = matrixA + matrixT + matrix.t();
	matrixA.print();

	Vector<double, CPU, 1> vec_as_scalar;
	vec_as_scalar = 4;

	vec_as_scalar.print();

	matrixA[1].print();

	vec.print();

	matrixA[1][1].print();
	matrixA[1][2].print();

	vec = vec + matrixA[1][1] + matrixA[1][2] + vec_as_scalar;

	Vector<double, CPU, 5> three;
	vec.print();
	return 0;
}

int main() {
	speedTests();
}
