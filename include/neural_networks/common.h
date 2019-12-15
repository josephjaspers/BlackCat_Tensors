/*
 * common.h
 *
 *  Created on: Jun 8, 2019
 *      Author: joseph
 */

#ifndef BLACKCAT_NEURALNETWORKS_COMMON_H_
#define BLACKCAT_NEURALNETWORKS_COMMON_H_


namespace BC {
namespace nn {

struct Layer_Loader;
struct Momentum;

using nn_default_system_tag = BC::host_tag;

template<class SystemTag, class ValueType, class... AltAllocator>
using nn_default_allocator_type =
		BC::allocators::Recycle_Allocator<SystemTag, ValueType, AltAllocator...>;

using nn_default_optimizer_type = Momentum;


static constexpr double default_learning_rate = 0.003;

#ifndef BLACKCAT_DEFAULT_SYSTEM
#define BLACKCAT_DEFAULT_SYSTEM_T BC::host_tag
#else
#define BLACKCAT_DEFAULT_SYSTEM_T BC::BLACKCAT_DEFAULT_SYSTEM##_tag
#endif

}
}

//required to be below
#include "layers/Layer_Traits.h"

#endif /* COMMON_H_ */
