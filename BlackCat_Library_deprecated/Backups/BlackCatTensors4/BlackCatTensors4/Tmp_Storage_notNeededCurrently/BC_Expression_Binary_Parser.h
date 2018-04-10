///*
// * BC_Math_ParseBinaryOperations.h
// *
// *  Created on: Nov 23, 2017
// *      Author: joseph
// */
//
//#ifndef BC_EXPRESSION_BINARY_PARSER_H_
//#define BC_EXPRESSION_BINARY_PARSER_H_
//
//#include "BC_Expression_Binary.h"
//#include <string>
//#include <functional>
////
////class BinaryExpressionPermutationGenerator {
////
////
////	template<class... params, int... shape>
////	class binary_expression;
////
//////	typedef binary_expression be<params, shape> be;
////
////	constexpr std::string operations[] = {
////		"add",
////		"sub",
////		"mul",
////		"div"
////	};
////	constexpr std::string operators[] = {
////			"+",
////			"-",
////			"*",
////			"/"
////	};
////	constexpr std::string template_decleration[] = {
////			"template<class ",
////			", class ml, class, lv, class rv, int... dimensions>"
////	};
////
////	constexpr std::string class_definition[]  = {
////			"class binary_expression<",
////			", ml, lv, rv, dimensions...> : public",
////	};
////
////
////	void gen();
////
////
////	BinaryExpressionPermutationGenerator();
////};
//
////Forward Declarations
//template<class, class, class, class, int...>
//class binary_expression;
//
//struct add;
//struct sub;
//struct div;
//struct mul;
//
//template<class >
//struct is {
//	static constexpr bool expr = false;
//};
//template<class o, class ml, class lv, class rv, int ... dims>
//struct is<binary_expression<o, ml, lv, rv, dims...>> {
//	static constexpr bool expr = true;
//};
//
//template<class c>
//struct expand_binExpr {
//	using op = void;
//	using ml = void;
//	using lv = c;
//	using rv = c;
//};
//
//template<class oper, class math_lib, class left, class right, int ... dimensions>
//struct expand_binExpr<binary_expression<oper, math_lib, left, right, dimensions...>> {
//	using op = oper;
//	using ml = math_lib;
//	using lv = left;
//	using rv = right;
//};
//
//template<class output, class op, class left, class right>
//struct evaluator {
////	static void calc(int i, output* o, left l, right r)
////	{	o[i] = l[i] + r[i];}
////	static auto calc(int i, left l, right r)
////	{	return l[i] + r[i];};
//};
//template<class output, class left, class right>
//struct evaluator<output, BC::add, left, right> {
//	inline __attribute__((always_inline)) static void calc(int i, output* o, left l, right r) {
//		o[i] = l.left[i] + r[i];
//	}
//	inline __attribute__((always_inline)) static auto calc(int i, left l, right r) {
//		return l[i] + r[i];
//	}
//};
//template<class output, class left, class right>
//struct evaluator<output, BC::sub, left, right> {
//	inline __attribute__((always_inline)) static void calc(int i, output* o, left l, right r) {
//		o[i] = l[i] - r[i];
//	}
//	inline __attribute__((always_inline)) static auto calc(int i, left l, right r) {
//		return l[i] - r[i];
//	}
//};
//template<class output, class left, class right>
//struct evaluator<output, BC::div, left, right> {
//	inline __attribute__((always_inline)) static void calc(int i, output* o, left l, right r) {
//		o[i] = l[i] / r[i];
//	}
//	inline __attribute__((always_inline)) static auto calc(int i, left l, right r) {
//		return l[i] / r[i];
//	}
//};
//template<class output, class left, class right>
//struct evaluator<output, BC::mul, left, right> {
//	inline __attribute__((always_inline)) static void calc(int i, output* o, left l, right r) {
//		o[i] = l[i] * r[i];
//	}
//	inline __attribute__((always_inline)) static auto calc(int i, left l, right r) {
//		return l[i] * r[i];
//	}
//};
//
//template<class output, class left, class right>
//struct evaluator<output, BC::add, left, right> {
//	static void calc(int i, output* o, left l, right r) {
//		o[i] = l[i] + r[i];
//	}
//	static auto calc(int i, left l, right r) {
//		return l[i] + r[i];
//	}
//	;
//};
//
//template<class output, class opMain, class op, class ml, class left, class right, class outer_right>
//struct evaluator<output, opMain, binary_expression<op, ml, left, right>, outer_right> {
//
//	static void calc(int i, output* o, left l, right r) {
//		o[i] = l[i] + r[i];
//	}
//	static auto calc(int i, left l, right r) {
//		return l[i] + r[i];
//	}
//	;
//
//};
//
////
////template<class oper, class left, class right>
////struct evaluator {
////
////	/*
////	 * When you call this functor it checks if l or r are binary_expression
////	 * if either are it typedefs lv/rv to the function of evaluator OR to the natural type (meaning it is a unary functor or an array)
////	 *
////	 * if type is functor the return function of functor (functor = this method) is a nested function that recursively expands...
////	 * the usage of templates means this expansion of this should be done at COMPILE TIME
////	 *
////	 *
////	 *
////	 */
////
////	typedef typename std::conditional<is<left>::expr,
////			typename evaluator<typename expand_binExpr<left>::op, typename expand_binExpr<left>::lv, typename expand_binExpr<left>::rv>::functor, right>::type lv;
////
////	typedef typename std::conditional<is<right>::expr,
////			typename evaluator<typename expand_binExpr<right>::op, typename expand_binExpr<right>::lv, typename expand_binExpr<right>::rv>::functor, right>::type rv;
////
//////	constexpr static std::function<int(int)> functor = [](int i, left l, right r) {
//////		return is<lv>::expr && is<rv>::expr ? [=](int i, left l, right r) {return lv(l.left, l.right)(i) + rv(r.left, r.right)(i);}:
//////		//if left is function
//////			is<lv>::expr ? [=](int i, left l, right r) {return lv(l.left, l.right)(i) + r[i];}:
//////			//if right is a function
//////			is<rv>::expr ? [=](int i, left l, right r) {return l[i] + rv(r.left, r.right)(i);}:
//////			//if neither are functions
//////			[=](int i, left l, right r) {return l[i] + r[i];};
//////		};
////};
////
//////	static constexpr auto functor() {
//////
//////		//If left is expression, typedef lv to functor of type, else typedef lv to left.
//////	//	typedef std::conditional<is<left>::expr, evaluator<expand_binExpr<left>::op, expand_binExpr<left>::lv, expand_binExpr<left>::rv>::functor>, left>::type lv;
//////		//typedef std::conditional<is<right>::expr, evaluator<expand_binExpr<right>::op, expand_binExpr<right>::lv, expand_binExpr<right>::rv>::functor>, right>::type rv;
//////
//////
//////				//if both sides are functions
//////				return [=](int i, left l, right r) {return lv(l.left, l.right)(i) + rv(r.left, r.right)(i);};
//////	}
////
//////		//return 1;
////////		//if both sides are functions
//////		return is<lv>::expr && is<rv>::expr ? [=](int i, eval_to to, left l, right r) {to[i] = lv(l.left, l.right)(i) + rv(r.left, r.right)(i);} :
//////		//if left is function
//////				is<lv>::expr ? [=](int i, eval_to to, left l, right r) {to[i] = lv(l.left, l.right)(i) + r[i];} :
//////				//if right is a function
//////				is<rv>::expr ? [=](int i, eval_to to, left l, right r) {to[i] = l[i] + rv(r.left, r.right)(i);} :
//////				//if neither are functions
//////				[=](int i, eval_to to, left l, right r) {to[i] = l[i] + r[i];};
//////	}
//////
//////	static constexpr auto functor() {
//////
//////		//If left is expression, typedef lv to functor of type, else typedef lv to left.
//////	//	typedef std::conditional<is<left>::expr, evaluator<expand_binExpr<left>::op, expand_binExpr<left>::lv, expand_binExpr<left>::rv>::functor>, left>::type lv;
//////		//typedef std::conditional<is<right>::expr, evaluator<expand_binExpr<right>::op, expand_binExpr<right>::lv, expand_binExpr<right>::rv>::functor>, right>::type rv;
//////
//////		typedef typename std::conditional<is<left>::expr, typename  evaluator<typename expand_binExpr<left>::op,
//////																			  typename expand_binExpr<left>::lv,
//////																			  typename expand_binExpr<left>::rv>::functor(), right>::type lv;
//////
//////		typedef typename std::conditional<is<right>::expr, typename  evaluator<typename expand_binExpr<right>::op,
//////																			  typename expand_binExpr<right>::lv,
//////																			  typename expand_binExpr<right>::rv>::functor(), right>::type rv;
//////		//return 1;
////////		//if both sides are functions
//////		return is<lv>::expr && is<rv>::expr ? [=](int i, left l, right r) {return lv(l.left, l.right)(i) + rv(r.left, r.right)(i);} :
//////		//if left is function
//////				is<lv>::expr ? [=](int i, left l, right r) {return lv(l.left, l.right)(i) + r[i];} :
//////				//if right is a function
//////				is<rv>::expr ? [=](int i, left l, right r) {return l[i] + rv(r.left, r.right)(i);} :
//////				//if neither are functions
//////				[=](int i, left l, right r) {return l[i] + r[i];};
//////	}
//////};
//
//#endif /* BC_EXPRESSION_BINARY_PARSER_H_ */
