/*
 * FeedForward.cu
 *
 *  Created on: Jan 28, 2018
 *      Author: joseph
 */

#ifndef OUTPUTas_CU
#define OUTPUTas_CU

#include "Layer_Base.h"

namespace BC {
namespace nn {

template<class SystemTag, class ValueType>
struct OutputLayer : Layer_Base {


	using system_tag = SystemTag;
	using value_type = ValueType;

	using mat = BC::Matrix<ValueType, BC::Allocator<SystemTag, ValueType>>;
    using vec = BC::Vector<ValueType, BC::Allocator<SystemTag, ValueType>>;

    using mat_view = BC::Matrix_View<ValueType, BC::Allocator<SystemTag, ValueType>>;



    mat_view x;

public:

    OutputLayer(int inputs) : Layer_Base(inputs, inputs) {}

    template <class Matrix>
    const auto& forward_propagation(const Matrix& x_) {
        return x = mat_view(x_);
    }

    template <class Matrix>
    auto back_propagation(const Matrix& exp) {
        return x - exp;
    }


    void update_weights() {}
    void clear_stored_gradients() {}
    void write(std::ofstream& is) {}
    void read(std::ifstream& os) {}

};

}
}



#endif /* FEEDFORWARD_CU_ */
