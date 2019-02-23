/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MATHEMATICS_CPU_H_
#define MATHEMATICS_CPU_H_

namespace BC {
namespace evaluator {

template<int Dimension>
struct evaluator_impl {
    template<class Expression, class... indexes>
    static void impl(Expression expression, indexes... indicies) {
        __BC_omp_for__
        for (int i = 0; i < expression.dimension(Dimension-1); ++i) {
        	evaluator_impl<Dimension-1>::impl(expression, i, indicies...);
        }
    }
};
template<>
struct evaluator_impl<1> {
    template<class Expression, class... indexes>
    static void impl(Expression expression, indexes... indicies) {
        __BC_omp_for__
        for (int i = 0; i < expression.dimension(0); ++i) {
            expression(i, indicies...);
        }
    }
    template<class Expression>
    static void impl(Expression expression) {
        __BC_omp_for__
        for (int i = 0; i < expression.size(); ++i) {
            expression[i];
        }
    }
};
template<>
struct evaluator_impl<0> {
    template<class Expression>
    static void impl(Expression expression) {
        __BC_omp_for__
        for (int i = 0; i < expression.size(); ++i) {
            expression[i];
        }
    }
};


struct Host {

//	template<int Dimension, class Expression>
//	static void nd_evaluator(Expression expression) {
//		evaluator_impl<Dimension>::impl(expression);
//		 __BC_omp_bar__
//	}

	template<int Dimension, class Expression, class Context>
	static void nd_evaluator(Expression expression, Context context) {
		auto job = [=]() {
			evaluator_impl<Dimension>::impl(expression);
		};
		context.push_job(job);

		 __BC_omp_bar__
	}

};
}
}
#endif
 /* MATHEMATICS_CPU_H_ */