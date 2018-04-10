//#include <iostream>
//#include <omp.h>
//#include "math.h"
//#include "time.h"
//#include <stdio.h>
//
//#include "BlackCat_Tensors.h"
//
//using BC::Vector;
//template<int SIZE, class T>
//inline __attribute__((always_inline)) void add(T* eval_to, T* a, T* b) {
//	eval_to[SIZE] = a[SIZE] + b[SIZE];
//	eval_to[SIZE - 1] = a[SIZE - 1] + b[SIZE - 1];
////	eval_to[sz - 2] = a[sz - 2] + b[sz - 3];
////	eval_to[sz - 3] = a[sz - 2] + b[sz - 4];
//
//	add<SIZE - 2, T>(eval_to, a, b);
//}
//template<>
//inline __attribute__((always_inline)) void add<0, double>(double* eval_to, double* a, double* b) {
//	eval_to[0] = a[0] + b[0];
//}
//
//void add(double* a, double* b, double* c, int sz) {
//	const int eval_dist = 64;
//
//#pragma omp parallel for schedule(static)
//
//	for (int i = 0; i < sz - eval_dist; i += eval_dist) {
//		add<eval_dist, double>(&a[i], &b[i], &c[i]);
//	}
//	for (int i = sz - eval_dist; i < sz; i++) {
//		a[i] = b[i] + c[i];
//	}
//#pragma omp barrier
//}
////Generic add two arrays together
//void dumb_add(double* a, double* b, double* c, int sz) {
//#pragma omp parallel for schedule(static)
//
//	for (int i = 0; i < sz; ++i) {
//		a[i] = b[i] + c[i];
//	}
//
//#pragma omp barrier
//}
//
////Generic add two arrays together
//void dumb_add_unrolled(double* a, double* b, double* c, int sz) {
//#pragma omp parallel for schedule(static)
//
//	for (int i = 0; i < sz; i += 2) {
//		a[i] = b[i] + c[i];
//		a[i + 1] = b[i + 1] + c[i + 1];
//	}
//
//#pragma omp barrier
//}
////Generic add two arrays together
//void dumb_add_unrolled(double* a, double* b, double* c, double* d, int sz) {
//#pragma omp parallel for schedule(static)
//
//	for (int i = 0; i < sz; i += 2) {
//		a[i] = b[i] + c[i] + d[i];
//		a[i + 1] = b[i + 1] + c[i + 1] + d[i + 1];
//	}
//
//#pragma omp barrier
//}
//
//void dumb_add_unrolled(double* a, double* b, double* c, double* d, double* e, int sz) {
//#pragma omp parallel for schedule(static)
//
//	for (int i = 0; i < sz; i += 2) {
//		a[i] = b[i] + c[i] + d[i] + e[i];
//		a[i + 1] = b[i + 1] + c[i + 1] + d[i + 1] + e[i + 1];
//	}
//
//#pragma omp barrier
//}
//
//template<int sz, int repetitions>
//int speedTests() {
//	const int SIZE = sz;
//	const int reps = repetitions;
//
//	std::cout << "  ------------------------------------------ speed tests ------------------------------------------" << std::endl;
//	std::cout << " size = " << SIZE << std::endl;
//	std::cout << " Repetitions = " << reps << std::endl;
//
//	double a[SIZE]; // = new double[sz];
//	double b[SIZE]; // = new double[sz];
//
//	double c[SIZE]; /// = new double[sz];
//	double d[SIZE]; // = new double[sz];
//
//	Vector<double, SIZE> at;
//	Vector<double, SIZE> bt;
//	Vector<double, SIZE> ct;
//
//	ct.randomize(0, 1000);
//	bt.randomize(0, 1000);
//
//	for (int i = 0; i < SIZE; ++i) {
//		a[i] = 0;
//		b[i] = 0;
//		c[i] = rand() % 1000;
//		d[i] = rand() % 1000;
//
//	}
//
//	float t;
////	t = omp_get_wtime();
////	printf("\n Calculating... Optimized through templates (a = c + d) \n");
////	for (int i = 0; i < reps; ++i)
////		add(a, c, d, SIZE);
////
////	t = omp_get_wtime() - t;
////	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//	t = omp_get_wtime();
//	printf("\n Calculating...dumb generic version (a =  c + d), (a = a + d) --same as (a = b + c + d)  \n");
//
//	for (int i = 0; i < reps; ++i) {
//		dumb_add(a, c, d, SIZE);
//		dumb_add(a, a, d, SIZE);
//
//	}
//
//	t = omp_get_wtime() - t;
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	t = omp_get_wtime();
//	printf("\n Calculating...dumb generic 0 unrolled version (a = b + c + d) \n");
//
//	for (int i = 0; i < reps; ++i) {
//		dumb_add_unrolled(a, b, b, c, SIZE);
//	}
//
//	t = omp_get_wtime() - t;
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	t = omp_get_wtime();
//	printf("\n Calculating... Lazy expression (Personal Class) (a = b + c + d) \n");
//	for (int i = 0; i < reps; ++i) {
//		at = bt + ct + ct;
//	}
//
//	t = omp_get_wtime() - t;
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//
//	t = omp_get_wtime();
//	//	//Examples of how to not be type safe
//	//	template<typename U> T operator + (const U scalar) { return this->array[0] + scalar; }
//	//	template<typename U> T operator - (const U scalar) { return this->array[0] - scalar; }
//	//	template<typename U> T operator / (const U scalar) { return this->array[0] / scalar; }
//	//	template<typename U> T operator % (const U scalar) { return this->array[0] * scalar; }
//	//	template<typename U> T operator * (const U scalar) { return this->array[0] * scalar; }
//
//
//	t = omp_get_wtime();
//	printf("\n Calculating... BlackCat Manual for loop zeroing (iteration speed)) \n");
//	for (int j = 0; j < reps; ++j)
//		for (int i = 0; i < SIZE; ++i) {
//			at[i] = 0;
//		}
//
//	t = omp_get_wtime() - t;
//
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	t = omp_get_wtime();
//	printf("\n Calculating... Generic Manual for loop zeroing (iteration speed)) \n");
//	for (int j = 0; j < reps; ++j)
//		for (int i = 0; i < SIZE; ++i) {
//			a[i] = 0;
//		}
//
//	t = omp_get_wtime() - t;
//
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	t = omp_get_wtime();
//	printf("\n Calculating... BlackCat Manual for loop adding 1 (iteration speed)) \n");
//	for (int j = 0; j < reps; ++j)
//		for (int i = 0; i < SIZE; ++i) {
//			at[i] = at[i] + 1;
//		}
//
//	t = omp_get_wtime() - t;
//
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	t = omp_get_wtime();
//	printf("\n Calculating... Generic Manual for loop adding 1 (iteration speed)) \n");
//	for (int j = 0; j < reps; ++j)
//		for (int i = 0; i < SIZE; ++i) {
//			a[i] = a[i] + 1;
//		}
//
//	t = omp_get_wtime() - t;
//
//	printf("It took me %d clicks (%f seconds).\n", t, ((float) t));
//
//	std::cout << "success " << std::endl;
//
//#pragma omp barrier
//	return 0;
//}
//
////	//Examples of how to not be type safe
////	template<typename U> T operator + (const U scalar) { return this->array[0] + scalar; }
////	template<typename U> T operator - (const U scalar) { return this->array[0] - scalar; }
////	template<typename U> T operator / (const U scalar) { return this->array[0] / scalar; }
////	template<typename U> T operator % (const U scalar) { return this->array[0] * scalar; }
////	template<typename U> T operator * (const U scalar) { return this->array[0] * scalar; }
//
//int main() {
//
//	speedTests<100000, 1000>();
//	speedTests<10000, 100000>();
//	speedTests<1000, 1000000>();
//	speedTests<100, 1000000>();
//	speedTests<10, 1000000>();
//
//	return 0;
//}
//
