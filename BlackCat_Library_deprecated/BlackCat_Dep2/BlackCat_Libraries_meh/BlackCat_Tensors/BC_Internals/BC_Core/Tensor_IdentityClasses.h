/*
 * Tensor_IdentityClasses.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef TENSOR_IDENTITYCLASSES_H_
#define TENSOR_IDENTITYCLASSES_H_

namespace BC {

	template<class derived>
	struct IDENTITY_BASE {
			  derived& convert() 	   { return static_cast<derived&>(*this); }
		const derived& convert() const { return static_cast<derived&>(*this); }
	};

template<class core> struct SCALAR : IDENTITY_BASE<core> { static constexpr int RANK = 0; };
template<class core> struct VECTOR : IDENTITY_BASE<core> { static constexpr int RANK = 1; };
template<class core> struct MATRIX : IDENTITY_BASE<core> { static constexpr int RANK = 2; };
template<class core> struct CUBE   : IDENTITY_BASE<core> { static constexpr int RANK = 3; };
template<class core> struct TENSOR : IDENTITY_BASE<core> { static constexpr int RANK = -1; };

	template<class scalar, class identity_, class functor, class evaluation, class math_lib, class parent = MTF::BOOL<true>>
	struct TRAITS {
			using scalar_type 		= scalar;
			using identity	 		= identity_;
			using functor_type 		= functor;
			using evaluation_type 	= evaluation;
			using math_library 		= math_lib;

			static constexpr bool isParent = parent::conditional;
	};
}



#endif /* TENSOR_IDENTITYCLASSES_H_ */
