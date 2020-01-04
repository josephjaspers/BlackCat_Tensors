	#define BC_ASSERT_ARRAY_ONLY(literal)\
	static_assert(\
		exprs::expression_traits<ExpressionTemplate>::is_array::value,\
		"BC Method: '" literal "' IS NOT SUPPORTED FOR EXPRESSIONS")

public:

	template<class Xpr>
	void copy(const Tensor_Base<Xpr>& rv) {
		static_assert(exprs::expression_traits<ExpressionTemplate>::is_copy_assignable::value, "copy lv must be array");
		static_assert(exprs::expression_traits<Xpr>::is_copy_assignable::value, "copy rv most be array");
		static_assert(ExpressionTemplate::tensor_iterator_dim <= 1, "copy only accepts continuous");
		static_assert(Xpr::tensor_iterator_dim <= 1, "copy only accepts continuous");

		if (this->size() != rv.size()) {
			bc::printerr(
					"Attempting to copy two different size tensors (ERROR)");
			throw 1;
		}

#ifdef __CUDACC__
		using copy_impl = bc::Utility<device_tag>;
		using same_system = std::is_same<system_tag, typename Xpr::system_tag>;
		if (std::is_same<system_tag, typename Xpr::system_tag>::value) {
			//Ensures it only compiles when true
			bc::traits::constexpr_if<same_system::value>(
					bc::traits::bind([](auto& self, const auto& rv){
						self = rv;
			}, *this, rv));
		} else if (std::is_same<system_tag, device_tag>::value) {
			copy_impl::HostToDevice(this->data(),
					rv.data(),
					this->size());
		} else {
			copy_impl::DeviceToHost(this->data(),
					rv.data(),
					this->size());
		}
#else
		this->*this = rv.*this;
#endif
	}

	std::string to_string(
			int precision=8,
			bool pretty=true,
			bool sparse=false) const
	{
		using traits = exprs::expression_traits<ExpressionTemplate>;
		using is_host = std::is_same<bc::host_tag, system_tag>;

#ifdef __CUDACC__
		using allocator_type = std::conditional_t<
				is_host::value,
				bc::Allocator<system_tag, value_type>,
				bc::Cuda_Managed<value_type>>;
#else
		using allocator_type = bc::Allocator<system_tag, value_type>;
#endif
		using tensor_type = Tensor_Base<exprs::Array<
				bc::Shape<tensor_dim>,
				value_type,
				allocator_type>>;

		using host_tensor_type = Tensor_Base<exprs::Array<
				bc::Shape<tensor_dim>,
				value_type,
				bc::Allocator<host_tag, value_type>>>;

		auto fs = bc::tensors::io::features(precision, pretty, sparse);
		auto dim = bc::traits::Integer<tensor_dim>();

		constexpr bool is_array = traits::is_array::value;
		constexpr bool no_copy_required = /*(is_managed::value || */
				is_host::value /*)*/ && is_array;

		static constexpr bool is_continuous = traits::is_continuous::value;

		return bc::traits::constexpr_if<no_copy_required>(
			bc::traits::bind([&](const auto& der)
				{
					return bc::tensors::io::to_string(der, fs, dim);
				}, *this),
			bc::traits::constexpr_else_if<is_continuous && is_array>(
				bc::traits::bind([&](const auto& der)
				{
					host_tensor_type tensor(der.get_shape());
					tensor.copy(der);
					return bc::tensors::io::to_string(tensor, fs, dim);
				}, *this),
			bc::traits::constexpr_else(
				bc::traits::bind([&](const auto& der)
				{
					tensor_type copy(der);
					bc::streams::device_sync();
					return bc::tensors::io::to_string(copy, fs, dim);
				}, *this))
			));
	}

	std::string to_raw_string(int precision=8) const {
		return this->to_string(precision, false, false);
	}

	void print(int precision=8, bool pretty=true, bool sparse=false) const {
		std::cout << this->to_string(precision, pretty, sparse) << std::endl;
	}

	void print_sparse(int precision=8, bool pretty=true) const {
		std::cout << this->to_string(precision, pretty, true) << std::endl;
	}

	void raw_print(int precision=0, bool sparse=false) const {
		std::cout << this->to_string(precision, false, sparse) << std::endl;
	}

	void print_dims() const {
		for (int i = 0; i < tensor_dim; ++i) {
			std::cout << "[" << this->dim(i) << "]";
		}
		std::cout << std::endl;
	}

	void print_leading_dims() const {
		for (int i = 0; i < tensor_dim; ++i) {
			std::cout << "[" << this->leading_dim(i) << "]";
		}
		std::cout << std::endl;
	}

	friend std::ostream& operator << (
			std::ostream& os,
			const Tensor_Base& self) {
		return os << self.to_string();
	}