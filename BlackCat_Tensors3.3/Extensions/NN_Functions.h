#ifndef BLACKCAT_TENSOR_FUNCTIONS
#define BLACKCAT_TENSOR_FUNCTIONS
namespace BC {

	namespace NN_Functions {

		struct Sigmoid {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T t) const {
				static constexpr T e = 2.71828;

				return 1 / (1 + std::pow(e, - t));
			}
		};
		struct SigmoidAssign {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T& t) const {
				static constexpr T e = 2.71828;

				return t = 1 / (1 + std::pow(e, - t));
			}
		};
		struct CachedSigmoidDeriv {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T t) const {
				return t * (1 - t);
			}
		};
		struct CachedSigmoidDerivAssign {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T& t) const {
				return t *= (1 - t);
			}
		};

		struct Tanh {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T t) const {
				static constexpr double e = 2.71828;

				return (powf(e, t) - powf(e, -t)) /
					   (powf(e, t) + powf(e, -t));
			}
		};
		struct TanhAssign {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T& t) const {
				static constexpr T e = 2.71828;

				return t = (powf(e, t) - powf(e, -t)) /
					       (powf(e, t) + powf(e, -t));
			}
		};
		struct CachedTanhDeriv {

			template<class T>
			__BCinline__ inline __attribute__((always_inline))  T operator () (T t) const {
				return 1 - powf(t, 2);
			}
		};
		struct CachedTanhDerivAssign {

			template<class T>

			__BCinline__ inline __attribute__((always_inline))  T operator () (T& t) const {
				static constexpr T e = 2.71828;

				return t = 1 - powf(t, 2);
			}
		};

		template<template<class,class> class tensor, class T, class ml>
		auto sigmoid(tensor<T, ml>& x) {
			return x.unExpr(SigmoidAssign());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto sigmoid(tensor<T, ml>&& x) {
			return x.unExpr(Sigmoid());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto sigmoidDeriv(tensor<T, ml>& x) {
			return x.unExpr(CachedSigmoidDerivAssign());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto sigmoidDeriv(tensor<T, ml>&& x) {
			return x.unExpr(CachedSigmoidDeriv());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto tanh(tensor<T, ml>& x) {
			return x.unExpr(TanhAssign());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto tanh(tensor<T, ml>&& x) {
			return x.unExpr(Tanh());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto tanhDeriv(tensor<T, ml>& x) {
			return x.unExpr(CachedTanhDerivAssign());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto tanhDeriv(tensor<T, ml>&& x) {
			return x.unExpr(CachedTanhDeriv());
		}

	}

	namespace NN_Abreviated_Functions {
		using namespace NN_Functions;
		template<template<class,class> class tensor, class T, class ml>
		auto g(const tensor<T, ml>& x) {
			return x.unExpr(Sigmoid());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto g(const tensor<T, ml>&& x) {
			return x.unExpr(Sigmoid());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto gd(const tensor<float, ml>& x) {
			return x.unExpr(CachedSigmoidDeriv());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto gd(const tensor<T, ml> x) {
			return x.unExpr(CachedSigmoidDeriv());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto h(const tensor<T, ml>& x) {
			return x.unExpr(Tanh());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto h(const tensor<T, ml>&& x) {
			return x.unExpr(Tanh());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto hd(const tensor<T, ml>& x) {
			return x.unExpr(CachedTanhDeriv());
		}
		template<template<class,class> class tensor, class T, class ml>
		auto hd(const tensor<T, ml>&& x) {
			return x.unExpr(CachedTanhDeriv());
		}

	}

}
#endif
