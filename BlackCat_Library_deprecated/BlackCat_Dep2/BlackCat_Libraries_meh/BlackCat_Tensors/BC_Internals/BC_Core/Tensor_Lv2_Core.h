/*
 * Tensor_Core.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef TENSOR_CORE_H_
#define TENSOR_CORE_H_

#include "Tensor_Lv1_Head.h"

namespace BC {

	/*
	 * Tensor Core is a syntactic sugar class
	 * All the ugly inheritance code goes here.
	 */

	template <class T, template<class> class IDENTITY, class derived, class lib>
	struct Tensor_Core
			: public Tensor_Head<
			  derived,
			  TRAITS<
			  	  typename MTF::determine_scalar<T>::type,
			  	  IDENTITY<derived>,
			  	  typename MTF::determine_functor<T>::type,
			  	  typename MTF::determine_evaluation<IDENTITY<derived>>::type, lib>> {


			using parent_class = Tensor_Head<
					  derived,
					  TRAITS<
					  	  typename MTF::determine_scalar<T>::type,
					  	  IDENTITY<derived>,
					  	  typename MTF::determine_functor<T>::type,
					  	  typename MTF::determine_evaluation<IDENTITY<derived>>::type, lib>>;

			using functor_type = typename parent_class::functor_type;
	};

}



#endif /* TENSOR_CORE_H_ */
