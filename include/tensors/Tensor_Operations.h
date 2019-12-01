private:

	#define BC_ASSERT_ASSIGNABLE(literal)           \
	static_assert(                                  \
			traits_type::is_copy_assignable::value, \
			"ASSERT COPY ASSIGNABLE: " literal)

	template<class ScalarType>
	using enable_if_scalar = std::enable_if_t<
			std::is_convertible<ScalarType, value_type>::value>;

	template<class self_type_t>
	void evaluate(const Tensor_Base<self_type_t>& tensor) {
		BC_ASSERT(this->get_stream().get_allocator().allocated_bytes() == 0,
				"Evaluation expects streams allocate_bytes to be 0 pre-evaluation");

		exprs::evaluate(tensor.internal(), this->get_stream());

		BC_ASSERT(this->get_stream().get_allocator().allocated_bytes() == 0,
				"Evaluation expects streams allocate_bytes to be 0 post-evaluation");
	}

public:

	// ---- assignment operators ---- //

	template<class Xpr> BCHOT
	self_type& operator = (const Tensor_Base<Xpr>& param)
	{
		static_assert(tensor_dimension >= Xpr::tensor_dimension,
				"BlackCat_Tensors: Operator= is not "
					"a valid operation for (reduction) broadcasting");
		BC_ASSERT_ASSIGNABLE(
				"self_type& operator = (const Tensor_Base<Xpr>& param)");
		assert_valid(param);
		evaluate(bi_expr< BC::oper::Assign >(param));
		return *this;
	}

#define BC_OPER_BASIC_ASSIGNMENT_DEF(op, op_functor)                       \
                                                                           \
    template<class Xpr> BCHOT                                              \
    self_type& operator op (const Tensor_Base<Xpr>& param) {               \
        BC_ASSERT_ASSIGNABLE(                                              \
                "operator " #op "(const Tensor_Base<Xpr>& param)");        \
        assert_valid(param);                                               \
        using operation = std::conditional_t<                              \
                (tensor_dimension >= Xpr::tensor_dimension),               \
                    oper::op_functor##_Assign,                             \
                    oper::Atomic_##op_functor<system_tag>>;                \
        evaluate(bi_expr< operation >(param));                             \
        return *this;                                                      \
    }                                                                      \

#define BC_OPER_SCALAR_ASSIGNMENT_DEF(op, op_functor)                      \
    template<class ScalarType, class=enable_if_scalar<ScalarType>>         \
    self_type& operator  op (const ScalarType& param) {                    \
        BC_ASSERT_ASSIGNABLE(                                              \
                "operator " #op " (const Tensor_Base<Xpr>& param)");       \
        value_type value = param;                                          \
        evaluate(                                                          \
                bi_expr_scalar<oper:: op_functor##_Assign >(               \
                        exprs::make_scalar_constant<system_tag>(value)));  \
        return *this;                                                      \
    }

#define BC_OPER_ASSIGNMENT_DEF(op, op_functor)\
        BC_OPER_SCALAR_ASSIGNMENT_DEF(op, op_functor)\
        BC_OPER_BASIC_ASSIGNMENT_DEF(op, op_functor)


	template<class ScalarType, class=enable_if_scalar<ScalarType>>
	self_type& operator = (const ScalarType& param)
	{
		BC_ASSERT_ASSIGNABLE(
				"self_type& operator =(const Tensor_Base<Xpr>& param)");

		evaluate(bi_expr_scalar<oper::Assign>(
				exprs::make_scalar_constant<system_tag>((value_type)param)));
		return *this;
	}

	BC_OPER_ASSIGNMENT_DEF(+=, Add)
	BC_OPER_ASSIGNMENT_DEF(-=, Sub)
	BC_OPER_ASSIGNMENT_DEF(%=, Mul)
	BC_OPER_ASSIGNMENT_DEF(/=, Div)

#undef BC_OPER_ASSIGNMENT_DEF
#undef BC_OPER_SCALAR_ASSIGNMENT_DEF
#undef BC_OPER_BASIC_ASSIGNMENT_DEF

// ---- elementwise operations ---- //

#define BC_BASIC_COEFFICIENTWISE_DEF(op, op_functor)       \
    template<class Xpr>                                    \
    auto op (const Tensor_Base<Xpr>& param) const    \
    {                                                      \
        assert_valid(param);                               \
        return bi_expr< oper:: op_functor >(param);        \
    }

#define BC_SCALAR_COEFFICIENTWISE_DEF(op, op_functor)                         \
    template<class ScalarType, class=enable_if_scalar<ScalarType>>            \
    auto op (const ScalarType& param) const                                   \
	{                                                                         \
        return bi_expr_scalar<oper::op_functor>(                              \
                exprs::make_scalar_constant<system_tag>((value_type)param));  \
    }                                                                         \
                                                                              \
    template<class ScalarType, class =enable_if_scalar<ScalarType>>           \
    friend auto op (const ScalarType& param, const Tensor_Base& tensor)       \
	{                                                                         \
		value_type value = param;                                             \
        auto scalar_obj = exprs::make_scalar_constant<system_tag>(value);     \
        return make_tensor(scalar_obj).bi_expr(oper:: op_functor (), tensor); \
    }

#define BC_COEFFICIENTWISE_DEF(op, op_functor)\
    BC_BASIC_COEFFICIENTWISE_DEF(op, op_functor)\
    BC_SCALAR_COEFFICIENTWISE_DEF(op, op_functor)\


#define BC_OPER_COEFFICIENTWISE_DEF(op, op_functor)\
    BC_BASIC_COEFFICIENTWISE_DEF(operator op, op_functor)\
    BC_SCALAR_COEFFICIENTWISE_DEF(operator op, op_functor)\

	BC_OPER_COEFFICIENTWISE_DEF(+, Add)
	BC_OPER_COEFFICIENTWISE_DEF(-, Sub)
	BC_OPER_COEFFICIENTWISE_DEF(%, Mul)
	BC_OPER_COEFFICIENTWISE_DEF(/, Div)
	BC_OPER_COEFFICIENTWISE_DEF( == , Equal )
	BC_OPER_COEFFICIENTWISE_DEF( >  , Greater)
	BC_OPER_COEFFICIENTWISE_DEF( <  , Lesser)
	BC_OPER_COEFFICIENTWISE_DEF( >= , Greater_Equal)
	BC_OPER_COEFFICIENTWISE_DEF( <= , Lesser_Equal )
	BC_OPER_COEFFICIENTWISE_DEF( && , And )
	BC_OPER_COEFFICIENTWISE_DEF( || , Or )

	BC_COEFFICIENTWISE_DEF(approx_equal, Approx_Equal)
	BC_COEFFICIENTWISE_DEF(max_value, Max)
	BC_COEFFICIENTWISE_DEF(min_value, Min)
	BC_SCALAR_COEFFICIENTWISE_DEF(operator *, Scalar_Mul)

#undef BC_BASIC_COEFFICIENTWISE_DEF
#undef BC_SCALAR_COEFFICIENTWISE_DEF
#undef BC_OPER_COEFFICIENTWISE_DEF
#undef BC_COEFFICIENTWISE_DEF

	template<class Xpr>
	auto operator *(const Tensor_Base<Xpr>& param) const {

		using blas_traits = exprs::blas_expression_traits<expression_type>;
		using rv_blas_traits = exprs::blas_expression_traits<Xpr>;

		constexpr bool lv_trans = blas_traits::is_transposed::value;
		constexpr bool rv_trans = rv_blas_traits::is_transposed::value;

		constexpr bool scalmul =
				tensor_dimension == 0 || Xpr::tensor_dimension == 0;
		constexpr bool gemm =
				tensor_dimension == 2 && Xpr::tensor_dimension == 2;
		constexpr bool gemv =
				tensor_dimension == 2 && Xpr::tensor_dimension == 1;
		constexpr bool ger =
				tensor_dimension == 1 && Xpr::tensor_dimension == 1 &&
				!lv_trans && rv_trans;
		constexpr bool dot =
				tensor_dimension == 1 && Xpr::tensor_dimension == 1 &&
				!lv_trans && !rv_trans;

		using matmul_t =
					 std::conditional_t<scalmul, oper::Scalar_Mul,
					 std::conditional_t<gemm, oper::gemm<system_tag>,
					 std::conditional_t<gemv, oper::gemv<system_tag>,
					 std::conditional_t<ger,  oper::ger<system_tag>,
					 std::conditional_t<dot,  oper::dot<system_tag>, void>>>>>;

		static_assert(!std::is_void<matmul_t>::value,
				"INVALID USE OF OPERATOR *");
		return bi_expr<matmul_t>(param);
	}

	// ---- Unary Expressions ---- //

	const auto transpose() const {
		return make_tensor(make_transpose(this->internal()));
	}

	      auto transpose()       {
		return make_tensor(make_transpose(this->internal()));
    }

	const auto t() const { return this->transpose(); }
	      auto t()       { return this->transpose(); }

	auto operator - () const {
		return un_expr<oper::negation>();
	}


	// ---- Negation Specializations ---- //

private:
	 template<class Xpr>
	 using negated_t = Tensor_Base<exprs::Unary_Expression<oper::negation, Xpr>>;
public:

	template<class Xpr>
	self_type& operator +=(const negated_t<Xpr>& param) {
		BC_ASSERT_ASSIGNABLE("operator +=(const Tensor_Base<Xpr>& param)");
		assert_valid(param);
		evaluate(bi_expr<oper::Sub_Assign>(make_tensor(param.array)));
		return *this;
	}

	template<class Xpr>
	self_type& operator -=(const negated_t<Xpr>& param) {
		BC_ASSERT_ASSIGNABLE("operator -=(const Tensor_Base<Xpr>& param)");
		assert_valid(param);
		evaluate(bi_expr<oper::Add_Assign>(make_tensor(param.array)));
		return *this;
	}

	template<class Xpr>
	auto operator +(const negated_t<Xpr>& param) const {
		assert_valid(param);
		return bi_expr<oper::Sub>(make_tensor(param.array));
	}

	// ---- expression_factory ---- //

	template<class functor>
	auto un_expr(functor f) const {
		return make_tensor(exprs::make_un_expr(this->internal(), f));
	}

	template<class functor>
	auto un_expr() const {
		return make_tensor(exprs::make_un_expr<functor>(this->internal()));
	}

	template<class functor, class Xpr>
	auto bi_expr(functor f, const Tensor_Base<Xpr>& rv) const {
		return make_tensor(
				exprs::make_bin_expr(this->internal(), rv.internal(), f));
	}

	template<class functor, class Xpr>
	auto bi_expr(const Tensor_Base<Xpr>& rv) const {
		return make_tensor(
				exprs::make_bin_expr<functor>(this->internal(), rv.internal()));
	}

	template<class functor, class Xpr>
	auto bi_expr(functor f, const Tensor_Base<Xpr>& rv) {
		return make_tensor(
				exprs::make_bin_expr(this->internal(), rv.internal(), f));
	}

	template<class functor, class Xpr>
	auto bi_expr(const Tensor_Base<Xpr>& rv) {
		return make_tensor(
				exprs::make_bin_expr<functor>(this->internal(), rv.internal()));
	}

private:

	template<class functor, class Scalar>
	auto bi_expr_scalar(functor f, const Scalar& rv) const {
		return make_tensor(exprs::make_bin_expr<functor>(this->internal(), rv, f));
	}

	template<class functor, class Scalar>
	auto bi_expr_scalar(const Scalar& rv) const {
		return make_tensor(exprs::make_bin_expr<functor>(this->internal(), rv));
	}

	template<class Xpr>
	bool valid_slice(const Tensor_Base<Xpr>& tensor) const {
		constexpr BC::size_t min_dim =
			traits::min(tensor_dimension, Xpr::tensor_dimension);

		for (int i = 0; i < min_dim; ++i)
			if (tensor.dimension(i) != this->dimension(i))
				return false;
		return true;
	}

	template<class deriv>
	bool error_message(const Tensor_Base<deriv>& tensor) const {
		throw std::invalid_argument(
				"Tensor by Tensor operation - size mismatch - "
				"\nthis->tensor_dimension =" +
						std::to_string(tensor_dimension) +
				"\nthis->size() =" + std::to_string(this->size()) +
				"\nthis_dims:" + this->inner_shape().to_string() +
				"\nparam->tensor_dimension =" +
						std::to_string(deriv::tensor_dimension) +
				"\nparam.size() =" + std::to_string(tensor.size()) +
				"\nparam_dims:" + tensor.inner_shape().to_string()
		);
	}

	template<class Xpr>
	void assert_valid(const Tensor_Base<Xpr>& tensor) const {
		static_assert(std::is_same<system_tag, typename Xpr::system_tag>::value,
				"Tensor arguments must have compatible (same) system_tags");

		bool scalar_op = tensor_dimension == 0 || Xpr::tensor_dimension == 0;
		bool same_dimension = tensor_dimension == Xpr::tensor_dimension;
		bool same_size = this->size() == tensor.size();

		if (!scalar_op) {
			if (same_dimension) {
				if (!same_size)
					error_message(tensor);
			} else if (!valid_slice(tensor)) {
				error_message(tensor);
			}
		}
	}


public:

	struct Alias{

		self_type& tensor;

		Alias(self_type& tensor_) : tensor(tensor_) {}

		template<class self_type_t>
		void evaluate(const Tensor_Base<self_type_t>& param) {
			evaluate_aliased(param.internal(), tensor.get_stream());
		}

		template<class self_type_t>
		auto& operator = (const Tensor_Base<self_type_t>& param) {
			tensor.assert_valid(param);
			evaluate(tensor.bi_expr(oper::Assign(), param));
			return tensor;
		}

		template<class self_type_t>
		auto& operator += (const Tensor_Base<self_type_t>& param) {
			tensor.assert_valid(param);
			evaluate(tensor.bi_expr(oper::Add_Assign(), param));
			return tensor;
		}

		template<class self_type_t>
		auto& operator -= (const Tensor_Base<self_type_t>& param) {
			tensor.assert_valid(param);
			evaluate(tensor.bi_expr(oper::Sub_Assign(), param));
			return tensor;
		}
	};

	friend class Alias;

	Alias alias() {
		return Alias (*this);
	}
