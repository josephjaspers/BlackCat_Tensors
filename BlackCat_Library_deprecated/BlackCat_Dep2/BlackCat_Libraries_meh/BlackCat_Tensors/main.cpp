#include <iostream>
#include "BlackCat_Tensors.h"
#include "BC_Internals/BC_Core/Vector.h"

using BC::Vector;

int main() {

	Vector<double> a(5);

	Vector<double> b(5);
	Vector<double> c(5);

//	b.randomize(-3, 5);
//	c.randomize(0, 5);

//	std::cout << " bad " << std::endl;
//
//	a.print();
//	b.print();
//	c.print();
//
//	a = b;
//	a.print();
//
//	a = b + c;

	a.print();

	std::cout << " out" << std::endl;
}
