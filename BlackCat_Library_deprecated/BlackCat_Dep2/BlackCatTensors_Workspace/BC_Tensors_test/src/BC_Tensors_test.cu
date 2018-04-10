#include "Tensor.h"
#include <iostream>
using namespace std;
#include <vector>

Tensor<double> test_tensor(std::initializer_list<unsigned> dim) {
    auto a = Tensor<double>({dim});

    for (int i = 0; i < a.size(); ++i) {
        a(i) = i + 1;
    }
    return a;
}

void Constructor_Tests() {
    std::cout << "constructor-data accessors test BEGIN-----------" << std::endl;



    Tensor<double> a = test_tensor({2, 3, 4});
    a[0][1][1];

    std::cout << "tensor a " << std::endl;

    a.print();
    std::cout << "a[0]" << std::endl;
    a[0].print();
    std::cout << "a[0] - filling with 2's" << std::endl;

    a[0].fill(2);
    std::cout << "tensor a "<< std::endl;
    a.print();
    std::cout << "a[1][1]" << std::endl;
    ((a[1])[1]).print();
    std::cout <<"setting a[1][1] to 999" << std::endl;
    a[1][1] = 999;
    std::cout << "tensor a" << std::endl;
    a.print();

    Tensor<double> b = Tensor<double>(1, 2, 3);
    b.print();
    std::cout << "constructor-data accessors test END-----------" << std::endl;
}

void movementTests() {
	std::cout <<"Movement Tests Begin-----------" << std::endl;
	Tensor<double> a = {3, 4};

    std::cout << " a tensor " << std::endl;
    a = 0;
    a.print();
    std::cout << " b tensor " << std::endl;
    auto b = test_tensor({3, 4});
    b.print();


    std::cout << " b tensor reshaped (2,2,3) " << std::endl;
   b.reshape({2, 2, 3});


    std::cout << " b reshaped (as reference 12)" << std::endl;
    b.reshape({12});
    b.print();

	std::cout <<"Movement Tests END-----------"<< std::endl;
}

void p(Tensor<double>& a, Tensor<double>& b) {
	std::cout << "dot product " << std::endl;
    (a * b).print();
}

void pt(Tensor<double>& a, Tensor<double>& b) {
	std::cout <<" transpsoe dot mult" << std::endl;
     (a.T() * b).print();
}
//void p(Tensor<double>& a, double& d);

void e(Tensor<double>& a, Tensor<double>& b) {
	std::cout << " poinstwise power" << std::endl;
    (a ^ b).print();
}

void m(Tensor<double>& a, Tensor<double>& b) {
	std::cout << " pointwise mult" << std::endl;
    (a & b).print();
}

void d(Tensor<double>& a, Tensor<double>& b) {
	std::cout << " divide " << std::endl;
    (a / b).print();
}

void a(Tensor<double>& a, Tensor<double>& b) {
    (a + b).print();
}

void s(Tensor<double>& a, Tensor<double>& b) {
    (a - b).print();
}
//matrix by scalar tests

void e(Tensor<double>& a, double& d) {
    (a ^ d).print();
}

void m(Tensor<double>& a, double& d) {
    (a & d).print();
}

void d(Tensor<double>& a, double& d) {
    (a / d).print();
}

void a(Tensor<double>& a, double& d) {
    (a + d).print();
}

void s(Tensor<double>& a, double& d) {
    (a - d).print();
}

void GeneralTests() {
    Tensor<double> alpha = Tensor<double>(4, 4);
    Tensor<double> beta = Tensor<double>(4, 4);
    for (int i = 0; i < alpha.size(); ++i) {
        alpha(i) = i;
    }
    for (int i = 0; i < beta.size(); ++i) {
        beta(i) = i;
    }

    alpha.print();
    beta.print();

    p(alpha, beta);
    e(alpha, beta);
    m(alpha, beta);
    d(alpha, beta);
    a(alpha, beta);
    s(alpha, beta);

    double vbeta = 2;

    e(alpha, vbeta);
    m(alpha, vbeta);
    d(alpha, vbeta);
    a(alpha, vbeta);
    s(alpha, vbeta);
}
//#include "cblas.h"
//#include "cblas_f77.h"

void dot_products() {
    auto a = test_tensor({2, 3});
    auto b = test_tensor({3, 2});

    std::cout << " a tensor " << std::endl;
    a.print();
    std::cout << " b tensor " << std::endl;
    b += 6;

    b.print();
    std::cout << " a*b tensor " << std::endl;

    (a * b).print();

    //b.transpose().print();
    std::cout << "transpose dot products "<< std::endl;
    auto c = test_tensor({2, 3});

    c(0) = 7; c(1) = 9; c(2) = 11;
    c(3) = 8; c(4) = 10; c(5) =12;

    c.print();
	b.transpose().print();

    std::cout << " a * c.t tensor " << std::endl;
   (a * c.transpose()).print();
   (a * c.T()).print();

std::cout << "a.t * c tensor  " << std::endl;
   (a.transpose() * c).print();
   (a.T() * c).print();

   a.transpose().print();
   b.transpose().print();

   c.print();
   c.transpose().print();
}
int main3() {

    std::cout << "%SUITE_STARTING% Method_Tests" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% " << std::endl;

   // std::cout << "constructor tests" << std::endl;
   // Constructor_Tests();
   // std::cout << "movement tests" << std::endl;
   // movementTests();
    std::cout << "operator tests" << std::endl;
    GeneralTests();
   // std::cout << "dotproduct tests" << std::endl;
   // dot_products();

    std::cout << "%TEST_FINISHED% time=0 test1 (Method_Tests)" << std::endl;


    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
    std::cout << " testing complete " << std::endl;
    return (EXIT_SUCCESS);
}


struct alpha {
	int a = 1;
	alpha* typeA;
};


int main() {
//	Constructor_Tests();
	Tensor<float> a = {3, 3};
	Tensor<float> b = {2,2};

for (unsigned i = 0; i < b.size(); ++i) {
	b(i) = i % 2;
}
	b.print();
	a({1,1},{2,2}).print();

	b.corr(2, a).print();


erro.print();


std::cout << " success " << std::endl;
	return 0;
}


