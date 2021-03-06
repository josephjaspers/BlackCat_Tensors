/*
 * Tensor_CMath.h
 *
 *  Created on: Oct 30, 2018
 *      Author: joseph
 */

#ifndef BLACKCAT_TENSOR_CMATH_H_
#define BLACKCAT_TENSOR_CMATH_H_

#include <functional>
#include <cmath>

namespace bc {
namespace tensors {
template<class> class Expression_Base;
template<class> class Tensor_Base;
}

namespace oper {
namespace cmath_functions {

#define BLACKCAT_FUNCTOR_DEF(funcName, instance_name, math_function, ...)  \
                                                                           \
    struct funcName {                                                      \
    template<class value_type> BCINLINE                                    \
    value_type operator () (const value_type& x) const {                   \
        return math_function;                                              \
    }                                                                      \
    template<class value_type> BCINLINE                                    \
    static auto apply(const value_type& x) {                               \
        return math_function;                                              \
    }                                                                      \
    template<class Xpr>                                                    \
    auto operator() (const bc::tensors::Expression_Base<Xpr>& tensor) {    \
          return tensor.un_expr(funcName());                               \
    }                                                                      \
    template<class Xpr>                                                    \
    auto operator() (const bc::tensors::Tensor_Base<Xpr>& tensor) {        \
          return tensor.un_expr(funcName());                               \
    }                                                                      \
    __VA_ARGS__                                                            \
} instance_name;                                                           \

#define DERIVATIVE_DEF(...)\
BLACKCAT_FUNCTOR_DEF(Derivative, dx, __VA_ARGS__)

#define DERIVATIVE_CACHED_DEF(...)\
BLACKCAT_FUNCTOR_DEF(Cached_Derivative, cached_dx, __VA_ARGS__)

#define BLACKCAT_MATH_DEF(funcName, instanceName, ...) \
BLACKCAT_FUNCTOR_DEF(funcName, instanceName, std::instanceName(x), __VA_ARGS__)

//UTILITY 'just returns x'
BLACKCAT_FUNCTOR_DEF( Pass , pass, x, DERIVATIVE_DEF(1))

//COMMON
BLACKCAT_MATH_DEF( Exp , exp , DERIVATIVE_DEF(std::exp(x)))
BLACKCAT_MATH_DEF( Exp2 , exp2 )
BLACKCAT_MATH_DEF( Sqrt , sqrt, DERIVATIVE_DEF((std::pow(x, -1/2)/2)))

//Trig
BLACKCAT_MATH_DEF( Sin , sin, DERIVATIVE_DEF(std::cos(x)));
BLACKCAT_MATH_DEF( Cos , cos, DERIVATIVE_DEF(-std::sin(x)))
BLACKCAT_MATH_DEF( Tan , tan, DERIVATIVE_DEF(std::pow(1/std::cos(x), 2)))
BLACKCAT_FUNCTOR_DEF( Sec, sec, 1/std::cos(x) )

//Hyperbolic
BLACKCAT_MATH_DEF( Sinh , sinh, DERIVATIVE_DEF(std::cosh(x)))
BLACKCAT_MATH_DEF( Cosh , cosh, DERIVATIVE_DEF(std::sinh(x)))
BLACKCAT_MATH_DEF( Tanh , tanh,
		DERIVATIVE_DEF(1 - std::pow(std::tanh(x), 2))
		DERIVATIVE_CACHED_DEF(1 - std::pow(x,2)))
//Arc
BLACKCAT_MATH_DEF( Asin , asin, DERIVATIVE_DEF(1/std::sqrt(1-std::pow(x,2))))
BLACKCAT_MATH_DEF( Acos , acos, DERIVATIVE_DEF(-1/std::sqrt(1-std::pow(x,2))))
BLACKCAT_MATH_DEF( Atan , atan, DERIVATIVE_DEF(1/(1+std::pow(x,2))))
BLACKCAT_MATH_DEF( Atan2 , atan2 )

//Arc Hyperbolic
BLACKCAT_MATH_DEF( Asinh , asinh )
BLACKCAT_MATH_DEF( Acosh , acosh )
BLACKCAT_MATH_DEF( Atanh , atanh )

BLACKCAT_MATH_DEF( Abs , abs )
BLACKCAT_MATH_DEF( Cbrt , cbrt )
BLACKCAT_MATH_DEF( Ceil , ceil )
BLACKCAT_MATH_DEF( Copysign , copysign )
BLACKCAT_MATH_DEF( Expm1 , expm1 )
BLACKCAT_MATH_DEF( Fabs , fabs )
BLACKCAT_MATH_DEF( Fdim , fdim )
BLACKCAT_MATH_DEF( Floor , floor )
BLACKCAT_MATH_DEF( Fma , fma )
BLACKCAT_MATH_DEF( Fmax , fmax )
BLACKCAT_MATH_DEF( Fmin , fmin )
BLACKCAT_MATH_DEF( Fmod , fmod )
BLACKCAT_MATH_DEF( Frexp , frexp )
BLACKCAT_MATH_DEF( Hypot , hypot )
BLACKCAT_MATH_DEF( Ilogb , ilogb )
BLACKCAT_MATH_DEF( Ldexp , ldexp )
BLACKCAT_MATH_DEF( Llrint , llrint )
BLACKCAT_MATH_DEF( Llround , llround )
BLACKCAT_MATH_DEF( Log , log, DERIVATIVE_DEF(1/x))
BLACKCAT_MATH_DEF( Log10 , log10 )
BLACKCAT_MATH_DEF( Log1P , log1p )
BLACKCAT_MATH_DEF( Log2 , log2 )
BLACKCAT_MATH_DEF( Logb , logb )
BLACKCAT_MATH_DEF( Lrint , lrint )
BLACKCAT_MATH_DEF( Lround , lround )
BLACKCAT_MATH_DEF( Modf , modf )
BLACKCAT_MATH_DEF( Nan , nan )
BLACKCAT_MATH_DEF( Nearbyint , nearbyint )
BLACKCAT_MATH_DEF( Nextafter , nextafter )
BLACKCAT_MATH_DEF( Nexttoward , nexttoward )

struct Pow {

	template<class ValueType, class Exp> BCINLINE
	ValueType operator () (const ValueType& x, Exp exp) const {
		return std::pow(x, exp);
	}

	template<class ValueType, class Exp> BCINLINE
	static auto apply(const ValueType& x, Exp exp) {
		return std::pow(x, exp);
	}

	template<class Xpr, class Exp>
	auto operator() (const bc::tensors::Expression_Base<Xpr>& tensor, Exp exp)
	{
		struct FunctorPow
		{
			typename Xpr::value_type exp;
			 auto operator() (const typename Xpr::value_type value) const {
				return std::pow(value, exp);
			}
		};

		return tensor.un_expr(FunctorPow {exp});
	}

} pow;

BLACKCAT_MATH_DEF( Remainder , remainder )
BLACKCAT_MATH_DEF( Remquo , remquo )
BLACKCAT_MATH_DEF( Rint , rint )
BLACKCAT_MATH_DEF( Round , round )
BLACKCAT_MATH_DEF( Scalbln , scalbln )
BLACKCAT_MATH_DEF( Scalbn , scalbn )
BLACKCAT_MATH_DEF( Trunc , trunc )
BLACKCAT_MATH_DEF( Isinf , isinf )
BLACKCAT_MATH_DEF( Isnan , isnan )

BLACKCAT_FUNCTOR_DEF(Pow2, pow2, (std::pow(x, 2)), DERIVATIVE_DEF(2));
BLACKCAT_FUNCTOR_DEF(Pow3, pow3, (std::pow(x, 3)), DERIVATIVE_DEF(3));

BLACKCAT_FUNCTOR_DEF(Logistic, logistic, (1 / (1 + std::exp(-x))),
		DERIVATIVE_DEF(Logistic::apply(x) * (1 - Logistic::apply(x)))
		DERIVATIVE_CACHED_DEF(x * (1 - x)));

BLACKCAT_FUNCTOR_DEF(Relu, relu, bc::traits::max(0, x),
		DERIVATIVE_DEF(x > 0 ? 1 : 0)
		DERIVATIVE_CACHED_DEF(x))

BLACKCAT_FUNCTOR_DEF(Logical, logical, x != 0 ? 1 : 0);

BLACKCAT_FUNCTOR_DEF(SoftPlus, softplus, std::log(1 + std::exp(x)),
		DERIVATIVE_DEF(Logistic::apply(x)))

BLACKCAT_FUNCTOR_DEF(Mish, mish,
		x * std::tanh(SoftPlus::apply(x)),
		DERIVATIVE_DEF(
				std::exp(x)
					* (4*(x+1)
					+ 4*(std::exp(2*x))
					+ std::exp(3*x)
					+ std::exp(x)*(4*x+6))
						/ std::pow((2*std::exp(x) + std::exp(2*x) + 2),2)))

#undef BLACKCAT_FUNCTOR_DEF
#undef BLACKCAT_MATH_DEF
#undef DERIVATIVE_DEF
#undef DERIVATIVE_CACHED_DEF

} //end of ns cmath_functions
} //end of ns oper

using namespace bc::oper::cmath_functions;

} //end of ns BC



#endif /* TENSOR_CMATH_H_ */
