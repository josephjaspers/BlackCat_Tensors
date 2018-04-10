#include "LinearAlgebraRoutines.h"
#include "BLACKCAT_EXPLICIT_INSTANTIATION.h"

//    static void power_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
//    static void divide_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
//    static void add_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
//    static void subtract_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);
//    static void multiply_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned sz);

template<typename number_type>
void Tensor_Operations<number_type>::power_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned row, unsigned col) {
    unsigned index = 0;
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            s[index] = pow(m1[c * row + r], m2[index]);
            ++index;
        }
    }
}

template<typename number_type>
void Tensor_Operations<number_type>::multiply_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned row, unsigned col) {
    unsigned index = 0;
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            s[index] = m1[c * row + r] * m2[index];
            ++index;
        }
    }
}
template<typename number_type>
void Tensor_Operations<number_type>::divide_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned row, unsigned col) {
    unsigned index = 0;
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            s[index] = m1[c * row + r] / m2[index];
            ++index;
        }
    }
}

template<typename number_type>
void Tensor_Operations<number_type>::add_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned row, unsigned col) {
    unsigned index = 0;
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            s[index] = m1[c * row + r] + m2[index];
            ++index;
        }
    }
}

template<typename number_type>
void Tensor_Operations<number_type>::subtract_TranposeA(number_type* s, const number_type* m1, const number_type* m2, unsigned row, unsigned col) {
    unsigned index = 0;
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            s[index] = m1[c * row + r] - m2[index];
            ++index;
        }
    }
}
