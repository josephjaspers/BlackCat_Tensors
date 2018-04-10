/*
 * Dynamic_Shape.h
 *
 *  Created on: Dec 20, 2017
 *      Author: joseph
 */

#ifndef DYNAMIC_SHAPE_H_
#define DYNAMIC_SHAPE_H_

/*
 * BC_Tensor_Super_Ace.h
 *
 *  Created on: Nov 18, 2017
 *      Author: joseph
 */
#include "../BlackCat_Internal_GlobalUnifier.h"
#include "../BlackCat_Printer.h"
#include <initializer_list>

#include "../BC_Shape/Dynamic_Shape_impl.h"

namespace BC {

	/*
	 * What's Happening?
	 *
	 * initDynamic and its sibling Specialization fills a pre-allocated array with the appropriate
	 * inner or outer dimensions.
	 */

template<int order>
struct Dynamic_Inner_Shape {

	static constexpr int RANK = order;
	Dynamic_Inner_Shape() = default;

	constexpr bool inner_shape_fast_copy () const  { return false; }

	int sz = 1;
	int Inner_Shape[order];

	int size()  		const { return Inner_Shape[order]; }
	int rows()  		const { return Inner_Shape[0];   }
	int cols()  		const { return order > 1 ? Inner_Shape[1] : 1; }
	int depth()			const { return order > 2 ? Inner_Shape[2] : 1; }
	int pages() 		const { return order > 3 ? Inner_Shape[3] : 1; }
	int books() 		const { return order > 4 ? Inner_Shape[4] : 1; }
	int libraries() 	const { return order > 5 ? Inner_Shape[5] : 1; }
	auto getShape() const { return Inner_Shape; }

	template<class... params>
	void initInner(const params... p) {
		sz = init_inner<params...>::foo((int *)Inner_Shape, p...);
	}
	void printDimensions() const {
		for (int i = 0; i < order; ++i) {
			IO::print- "[", Inner_Shape[i], "]";
		}
		IO::print - IO::NewLine();
	}
};



template<int order>
struct Dynamic_Outer_Shape {

	static constexpr int RANK = order;
	Dynamic_Outer_Shape() = default;

	constexpr bool outer_shape_fast_copy () const  { return false; }


	int Outer_Shape[order];
	int outer_sz = 0;

	int LD_size()  		const { return Outer_Shape[order]; }
	int LD_rows()  		const { return Outer_Shape[0];   }
	int LD_cols()  		const { return order > 1 ? Outer_Shape[1] : 1;  }
	int LD_depth()		const { return order > 2 ? Outer_Shape[2] : 1;  }
	int LD_pages() 		const { return order > 3 ? Outer_Shape[3] : 1;  }
	int LD_books() 		const { return order > 4 ? Outer_Shape[4] : 1;  }
	int LD_libraries() 	const { return order > 5 ? Outer_Shape[5] : 1;  }

	template<class... params>
	void initOuter(const params... p) {
		outer_sz = init_outer<params...>::foo(Outer_Shape, p...);
	}

	void printLeadingDimensions() const {
		for (int i = 0; i < order; ++i) {
			IO::print- "[", Outer_Shape[i], "]";
		}
		IO::print - IO::NewLine();
	}
};
}
#endif /* DYNAMIC_SHAPE_H_ */
