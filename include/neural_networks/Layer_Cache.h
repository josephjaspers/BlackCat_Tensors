/*
 * Layer_Cache.h
 *
 *  Created on: Aug 31, 2019
 *      Author: joseph
 */

#ifndef BLACKCATTENSORS_NEURALNETWORKS_LAYER_CACHE_H_
#define BLACKCATTENSORS_NEURALNETWORKS_LAYER_CACHE_H_

#include <vector>
#include <type_traits>

namespace BC {
namespace nn {

template<class K, class V, class IsRecurrent>
struct cache_key : BC::utility::Any_Key<K, V> {
	static_assert(
			std::is_same<IsRecurrent, std::true_type>::value ||
					std::is_same<IsRecurrent, std::false_type>::value,
			"Cache_Key `IsRecurrent` must be std::true_type or std::false_type");

	using is_recurrent = IsRecurrent;
};

struct Cache {

	template<class K, class V, class R>
	using key_type = cache_key<K, V, R>;

	int m_time_index = 0;

	BC::utility::Any_Map cache;


private:

	template<class K, class V>
	auto hash(key_type<K, V, std::true_type> key) {
		return BC::utility::Any_Key<K, std::vector<V>>();
	}

	template<class K, class V>
	auto hash(key_type<K, V, std::false_type> key) {
		return BC::utility::Any_Key<K, V>();
	}

public:

	template<class K, class V>
	auto& load(key_type<K, V, std::true_type> key, int t_modifier=0) {
		std::vector<V>& history = cache[hash(key)];
		int index = history.size()- 1 - m_time_index + t_modifier;

		BC_ASSERT(index < history.size(),
			"Load recurrent_variable index out of bounds"
				"\nHistory size: " + std::to_string(history.size()) +
				"\nIndex:" + std::to_string(index));

		return history[index];
	}

	template<class K, class V>
	auto& load(key_type<K, V, std::false_type> key) {
		return cache[hash(key)];
	}

	template<class K, class V, class U>
	auto& store(key_type<K, V, std::true_type> key, U&& expression) {
		cache[hash(key)].push_back(V(expression));
		return cache[hash(key)].back();
	}

	template<class K, class V, class U>
	auto& store(key_type<K, V, std::false_type> key, U&& expression) {
		if (cache.contains(hash(key))) {
			return cache[hash(key)] = expression;
		} else {
			return cache[hash(key)] = V(expression);
		}
	}

	int get_time_index() const { return m_time_index; }
	void increment_time_index() { m_time_index++; }
	void decrement_time_index() { m_time_index--; }
	void zero_time_index() { m_time_index=0; }

	template<class K, class V>
	void clear_bp_storage(key_type<K, V, std::false_type> key) {}

	template<class K, class V>
	void clear_bp_storage(key_type<K, V, std::true_type> key) {
		auto& storage = cache[hash(key)];

		if (storage.size() > 1) {
			auto last = std::move(storage.back());
			storage.clear();
			storage.push_back(std::move(last));
		}
	}
};


template<class Type, class BatchedType>
struct Recurrent_Tensor_Cache {

	//time index refers to 'the past index' IE time_index==3 means '3 timestamps in the past'
	int time_index = 0;

	std::vector<Type> tensor;
	std::vector<BatchedType> batched_tensor;

	int get_time_index() const { return time_index; }
	void increment_time_index() { time_index++; }
	void decrement_time_index() { time_index--; }
	void zero_time_index() { time_index = 0; }

	void init_tensor(Type init) {
		tensor.push_back(std::move(init));
		tensor.back().zero();
	}

	void init_batched(BatchedType init) {
		batched_tensor.push_back(std::move(init));
		batched_tensor.back().zero();
	}

	template<class... Args>
	void init_tensor(const Args&... init) {
		tensor.push_back(Type(init...));
		tensor.back().zero();
	}

	template<class... Args>
	void init_batched(const Args&... init) {
		batched_tensor.push_back(BatchedType(init...));
		batched_tensor.back().zero();
	}

	Type& load(std::false_type is_batched = std::false_type(), int tmodifier=0) {
		return tensor[tensor.size() - 1 - time_index + tmodifier];
	}

	BatchedType& load(std::true_type is_batched, int tmodifier=0) {
		return batched_tensor[batched_tensor.size() - 1 - time_index + tmodifier];
	}

	const Type& load(std::false_type is_batched = std::false_type(), int tmodifier=0) const {
		return tensor[tensor.size() - 1 - time_index + tmodifier];
	}

	const BatchedType& load(std::true_type is_batched, int tmodifier=0) const {
		return batched_tensor[batched_tensor.size() - 1 - time_index + tmodifier];
	}

	template<class T>
	auto& store(const T& expression) {
		using is_batched = BC::traits::truth_type<(T::tensor_dimension == BatchedType::tensor_dimension)>;
		return store(expression, is_batched());
	}

	template<class T>
	auto& store(const T& expression, std::true_type is_batched) {
		this->batched_tensor.push_back(expression);
		return this->batched_tensor.back();
	}

	template<class T>
	auto& store(const T& expression, std::false_type is_batched) {
		this->tensor.push_back(expression);
		return this->tensor.back();
	}

	void clear_bp_storage() {
		if (tensor.size() > 1) {
			auto last = std::move(tensor.back());
			tensor.clear();
			tensor.push_back(std::move(last));
		}
		if (batched_tensor.size() > 1) {
			auto last = std::move(batched_tensor.back());
			batched_tensor.clear();
			batched_tensor.push_back(std::move(last));
		}
	}
};

}
}


#endif /* LAYER_CACHE_H_ */
