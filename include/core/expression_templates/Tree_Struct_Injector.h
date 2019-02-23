/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef INJECTION_INFO_H_
#define INJECTION_INFO_H_


namespace BC {
namespace expression_templates {
namespace tree {


template<class tensor_core, BC::size_t  alpha_modifier_ = 1, BC::size_t  beta_modifier_= 0>
struct injector {

	static constexpr BC::size_t  ALPHA = alpha_modifier_;
	static constexpr BC::size_t  BETA = beta_modifier_;
    injector(tensor_core& array_) : array(array_) {}

    tensor_core& array;

    operator const tensor_core& () const { return array; }
    operator       tensor_core& ()       { return array; }

    const tensor_core& data() const { return array; }
          tensor_core& data()       { return array; }
};


template<int a, int b, class core>
auto make_injection(core& c) {
	return injector<core, a, b>(c);
}


}
}
}


#endif /* INJECTION_INFO_H_ */