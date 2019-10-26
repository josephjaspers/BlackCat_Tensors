/*
 	 * Layer_Manager.h
 *
 *  Created on: Jul 23, 2019
 *      Author: joseph
 */

#ifndef LAYER_MANAGER_H_
#define LAYER_MANAGER_H_

#include "Layer_Cache.h"

namespace BC {
namespace nn {

template<
	class Derived, //The LayerChain base
	class Layer,
	class Neural_Network_Is_Recurrent=std::false_type, //must be std::true_type or false_type or have "value"
	class Allocator=BC::Allocator<
		typename layer_traits<Layer>::system_tag,
		typename layer_traits<Layer>::value_type>>
struct Layer_Manager: Layer {

	template<class... Args>
	Layer_Manager(Args... args):
		Layer(args...) {}

	template<class D, class L, class R, class A>
	friend class Layer_Manager;

	using input_tensor_dimension = typename layer_traits<Layer>::input_tensor_dimension;
	using output_tensor_dimension = typename layer_traits<Layer>::output_tensor_dimension;
	using is_recurrent = Neural_Network_Is_Recurrent;

	static_assert(input_tensor_dimension::value == decltype(std::declval<Layer>().get_input_shape())::tensor_dimension,
			"Input tensor_dimension must be equal to Layer::get_input_shape() dimension"
			"\n, did you forget to override get_input_shape()?");

	static_assert(output_tensor_dimension::value == decltype(std::declval<Layer>().get_output_shape())::tensor_dimension,
			"output tensor_dimension must be equal to Layer::get_input_shape() dimension"
			"\n, did you forget to override get_output_shape()?");

	static_assert(input_tensor_dimension::value+1 == decltype(std::declval<Layer>().get_batched_input_shape())::tensor_dimension,
			"Input tensor_dimension must be equal to Layer::get_input_shape() dimension"
			"\n, did you forget to override get_batched_input_shape()?");

	static_assert(output_tensor_dimension::value+1 == decltype(std::declval<Layer>().get_batched_output_shape())::tensor_dimension,
			"output tensor_dimension must be equal to Layer::get_input_shape() dimension"
			"\n, did you forget to override get_batched_output_shape()?");

	using value_type = typename layer_traits<Layer>::value_type;

	using output_tensor_type = BC::Tensor<output_tensor_dimension::value, value_type, Allocator>;
	using batched_output_tensor_type = BC::Tensor<output_tensor_dimension::value+1, value_type, Allocator>;

	using input_tensor_type = BC::Tensor<input_tensor_dimension::value, value_type, Allocator>;
	using batched_input_tensor_type = BC::Tensor<input_tensor_dimension::value+1, value_type, Allocator>;

	using requires_extra_cache = typename layer_traits<Layer>::requires_extra_cache;

	template<char C, class Tensor, class isRecurrent=std::true_type>
	using key_type = cache_key<BC::utility::Name<C>, Tensor, isRecurrent>;

	using batched_input_key = key_type<'X', batched_input_tensor_type>;
	using batched_delta_key = key_type<'D', batched_input_tensor_type, std::false_type>;
	using input_key = key_type<'X', input_tensor_type>;
	using delta_key = key_type<'D', input_tensor_type, std::false_type>;

	Cache m_cache;

	Cache& get_cache() { return m_cache; }
	const Cache& get_cache() const { return m_cache; }

	void zero_time_index() {
		m_cache.zero_time_index();
	}

	void increment_time_index() {
		m_cache.increment_time_index();
	}

private:

	auto default_batched_input_tensor_factory() const {
		return [&]() {
			batched_input_tensor_type in(this->get_batched_input_shape());
			in.zero();
			return in;
		};
	}

	auto default_batched_output_tensor_factory() const {
		return [&]() {
			batched_output_tensor_type out(this->get_batched_output_shape());
			out.zero();
			return out;
		};
	}

public:

	template<class T>
	auto forward_propagation(const T& expression) {
		BC_ASSERT(expression.get_shape() == this->get_batched_input_shape(),
				"forward_propagation input must have the same shape as "
					"get_batched_input_shape() of the current layer "
					"(Invalid input dimensions)");

		return forward_supply_outputs(
				typename layer_traits<Layer>::forward_requires_outputs(),
				store_batched_inputs(expression));
	}

	template<class T>
	auto back_propagation(const T& dy) {
		BC_ASSERT(dy.get_shape() == this->get_batched_output_shape(),
				"back_propagation input must have the same shape as "
					"get_batched_output_shape() of the current layer "
					"(Invalid input dimensions)");


		return backward_supply_outputs(
				typename layer_traits<Layer>::backward_requires_outputs(),
				get_batched_inputs(),
				maybe_cache_delta(dy));
	}

	//TODO batched_predict_input_key
	template<class T>
	auto predict(const T& expression) {
		BC_ASSERT(expression.get_shape() == this->get_batched_input_shape(),
				"predict<T> input must have the same shape as "
					"get_batched_input_shape() of the current layer "
					"(Invalid input dimensions)");


		return predict_supply_outputs(
				typename layer_traits<Layer>::forward_requires_outputs(),
				store_batched_inputs(expression));
	}

	//TODO input_key -> predict_input_key
	template<class T>
	auto single_predict(const T& expression) {
		BC_ASSERT(expression.get_shape() == this->get_input_shape(),
				"single_predict<T> input must have the same shape as "
					"get_input_shape() of the current layer "
					"(Invalid input dimensions)");

		static_assert(T::tensor_dimension ==
						layer_traits<Layer>::input_tensor_dimension::value,
						"assert same dimension as layer");

		return single_predict_supply_outputs(
				typename layer_traits<Layer>::forward_requires_outputs(),
				this->m_cache.store(input_key(), expression));
	}

	void update_weights() {
		Layer::update_weights();
		m_cache.clear_bp_storage(batched_input_key());
		Layer::clear_bp_storage(m_cache);
	}

	void save(Layer_Loader& loader) {
		int t_index = m_cache.get_time_index();
		m_cache.zero_time_index();
		loader.save_variable(get_batched_inputs(), "x");

		Layer::save(loader);
		Layer::save_from_cache(loader, m_cache);

		m_cache.set_time_index(t_index);
	}

	void load (Layer_Loader& loader) {
		int t_index = m_cache.get_time_index();
		m_cache.zero_time_index();
		loader.load_variable(get_batched_inputs(), "x");

		Layer::load(loader);
		Layer::load_to_cache(loader, m_cache);

		m_cache.set_time_index(t_index);
	}

private:

	auto& get_batched_inputs() {
		return m_cache.load(
				batched_input_key(),
				default_batched_input_tensor_factory());
	}

	template<class X>
	auto& store_batched_inputs(const X& x) {
		return this->m_cache.store(batched_input_key(), x);
	}

	auto& next_layer() {
		return BC::traits::derived_cast(*this).next().layer();
	}



	// Handle Forward Args ------------------------------------------------

	template<class X>
	auto forward_supply_outputs(std::false_type, const X& inputs) {
		return forward_supply_cache(requires_extra_cache(), inputs);
	}

	template<class Input>
	auto forward_supply_outputs(std::true_type, const Input& inputs) {
		auto& outputs = next_layer().get_batched_inputs();
		return forward_supply_cache(requires_extra_cache(), inputs, outputs);
	}

	template<class... Args>
	auto forward_supply_cache(std::false_type, const Args&... args) {
		return Layer::forward_propagation(args...);
	}

	template<class... Args>
	auto forward_supply_cache(std::true_type, const Args&... args) {
		return Layer::forward_propagation(args..., m_cache);
	}

	//Handle Predict Args  ------------------------------------------------

	template<class X>
	auto predict_supply_outputs(std::false_type, const X& inputs) {
		return predict_supply_cache(requires_extra_cache(), inputs);
	}

	template<class Input>
	auto predict_supply_outputs(std::true_type, const Input& inputs) {
		auto& outputs = next_layer().get_batched_inputs();
		return predict_supply_cache(requires_extra_cache(), inputs, outputs);
	}

	template<class... Args>
	auto predict_supply_cache(std::false_type, const Args&... args) {
		return layer_traits<Layer>::select_on_predict(*this, args...);
	}

	template<class... Args>
	auto predict_supply_cache(std::true_type, const Args&... args) {
		return layer_traits<Layer>::select_on_predict(*this, args..., m_cache);
	}

	//Handle Single Predict Args  -------------------------------------------

	template<class X>
	auto single_predict_supply_outputs(std::false_type, const X& inputs) {
		static_assert(X::tensor_dimension == input_tensor_dimension::value,
				"Assert single-batch dim for Neural_Network.predict()");
		return single_predict_supply_cache(requires_extra_cache(), inputs);
	}

	template<class Input>
	auto single_predict_supply_outputs(std::true_type, const Input& inputs) {
		auto default_factory = [&]() {
			return input_tensor_type(this->get_output_shape()).zero();
		};

		using key_type = typename std::decay_t<decltype(next_layer())>::input_key;
		auto& outputs = next_layer().m_cache.load(key_type(), default_factory);
		return single_predict_supply_cache(requires_extra_cache(), inputs, outputs);
	}

	template<class... Args>
	auto single_predict_supply_cache(std::false_type, const Args&... args) {
		return layer_traits<Layer>::select_on_single_predict(*this, args...);
	}

	template<class... Args>
	auto single_predict_supply_cache(std::true_type, const Args&... args) {
		return layer_traits<Layer>::select_on_single_predict(*this, args..., m_cache);
	}

	//Handel backward args  ------------------------------------------------

	template<class X, class... T>
	auto backward_supply_outputs(std::false_type,  const X& x, const T&... args) {
		return backward_supply_cache(requires_extra_cache(), x, args...);
	}

	template<class Input, class Dy>
	auto backward_supply_outputs(std::true_type,  const Input& inputs, const Dy& delta) {
		auto& outputs = next_layer().get_batched_inputs();
		return backward_supply_cache(requires_extra_cache(), inputs, outputs, delta);
	}

	template<class... Args>
	auto backward_supply_cache(std::false_type, const Args&... args) {
		return Layer::back_propagation(args...);
	}

	template<class... Args>
	auto backward_supply_cache(std::true_type, Args&... args) {
		return Layer::back_propagation(args..., m_cache);
	}

	template<class T>
	auto&& maybe_cache_delta(const T& dy) {
		return maybe_cache_delta_impl(
				typename layer_traits<Layer>::greedy_evaluate_delta(), dy);
	}

	template<class T>
	auto&& maybe_cache_delta_impl(std::true_type cache_delta, const T& dy) {
		return m_cache.store(batched_delta_key(), dy);
	}

	template<class T>
	const T& maybe_cache_delta_impl(std::false_type cache_delta, const T& dy) {
		return dy;
	}
};


}  // namespace nn
}  // namespace BC



#endif /* LAYER_MANAGER_H_ */
