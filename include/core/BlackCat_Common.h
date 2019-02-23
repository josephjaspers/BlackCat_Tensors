/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLACKCAT_BLACKCAT_COMMON_H_
#define BLACKCAT_BLACKCAT_COMMON_H_


#include <type_traits>
#include "evaluator/Evaluator.h"
#include "allocator/Allocator.h"
#include "random/Random.h"
#include "utility/Utility.h"
#include "blas/BLAS.h"
#include "context/Context.h"

namespace BC {

template<class> class Tensor_Base;

template<class internal_t>
auto make_tensor(internal_t internal) {
    return Tensor_Base<internal_t>(internal);
}

}

#endif /* BC_INTERNALS_BC_TENSOR_TENSOR_COMMON_H_ */