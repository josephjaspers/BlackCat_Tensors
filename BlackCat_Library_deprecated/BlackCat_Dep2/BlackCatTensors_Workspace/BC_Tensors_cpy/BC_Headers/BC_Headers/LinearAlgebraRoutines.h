/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LinearAlgebraRoutines.h
 * Author: joseph
 *
 * Created on July 23, 2017, 9:11 PM
 */

#ifndef LINEARALGEBRAROUTINES_H
#define LINEARALGEBRAROUTINES_H
#include <iostream>
#include <math.h>
#include "cblas.h"

template <typename number_type>
class Tensor_Operations {
public:
    //Advanced
    static void correlation_autoPadding(number_type * s, const number_type* filter, unsigned length, const number_type* img, unsigned img_r, unsigned img_c);
    static void convolution_autoPadding(number_type * s, const number_type* filter, unsigned length, const number_type* img, unsigned img_r, unsigned img_c);
    static void correlation(number_type* s, const number_type* filter, unsigned filter_dim, const number_type* img, unsigned img_dim);
    static void convolution(number_type* s, const number_type* filter, unsigned filter_dim, const number_type* img, unsigned img_dim);
    //Dotproducts
    static void dot(number_type* s, const number_type* m1, unsigned m1_rows, unsigned m1_cols, const number_type* m2, unsigned m2_rows, unsigned m2_cols);
    static void dot_outerproduct(number_type* s, const number_type* m1, unsigned m1_size, const number_type* m2, unsigned m2_size);
    //General
    static void initialize(number_type*& d, unsigned sz);
    static void destruction(number_type* d);
    static void copy(number_type* store, const number_type* v, unsigned sz);
    static void equal(number_type* m1, const number_type* m2, unsigned sz, bool* EQUAL);
    static void fill(number_type* m, number_type f, unsigned sz);
    static void transpose(number_type* s, const number_type* m, unsigned x_dim, unsigned y_dim);
    static void randomize(number_type* m, int lower_bound, int upper_bound, unsigned sz);
    //Pointwise
    static void power(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
    static void divide(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
    static void add(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
    static void subtract(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
    static void multiply(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
    //Pointwise Scalar
    static void power(number_type *s, const number_type *mat, number_type v, unsigned sz);
    static void divide(number_type *s, const number_type *m, number_type v, unsigned sz);
    static void add(number_type *s, const number_type *m, number_type v, unsigned sz);
    static void subtract(number_type *s, const number_type *m, number_type v, unsigned sz);
    static void multiply(number_type *s, const number_type *m, number_type v, unsigned sz);
    //Indexing
    static void max_val(const number_type* m, number_type* max_val, unsigned sz);
    static void min_val(const number_type* m, number_type* min_val, unsigned sz);
    static void max_index(const number_type * m, number_type * max_val, unsigned * index, unsigned sz);
    static void min_index(const number_type * m, number_type * min_val, unsigned * index, unsigned sz);
    //Pointwise_ increment variants - - - - - - - - -COLMAJOR- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void power(number_type* store, unsigned s_row, unsigned s_col, unsigned s_inc, number_type* m1, unsigned m1_inc,  number_type* m2, unsigned m2_inc);
    static void multiply(number_type* store, unsigned s_row, unsigned s_col, unsigned s_inc, number_type* m1, unsigned m1_inc,  number_type* m2, unsigned m2_inc);
    static void divide(number_type* store, unsigned s_row, unsigned s_col, unsigned s_inc, number_type* m1, unsigned m1_inc,  number_type* m2, unsigned m2_inc);
    static void add(number_type* store, unsigned s_row, unsigned s_col, unsigned s_inc, number_type* m1, unsigned m1_inc,  number_type* m2, unsigned m2_inc);
    static void subtract(number_type* store, unsigned s_row, unsigned s_col, unsigned s_inc, number_type* m1, unsigned m1_inc,  number_type* m2, unsigned m2_inc);

#endif /* LINEARALGEBRAROUTINES_H */

