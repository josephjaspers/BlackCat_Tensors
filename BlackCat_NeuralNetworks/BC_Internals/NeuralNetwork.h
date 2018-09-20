/*
 * NeuralNetwork.h
 *
 *  Created on: Mar 5, 2018
 *      Author: joseph
 */

#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_

#include <forward_list>

#include "../BC_Internals/LayerChain.h"
#include "../BC_Internals/Layers/InputLayer.h"
#include "../BC_Internals/Layers/OutputLayer.h"
//#include "LayerLinker.h"
namespace BC {
namespace NN {

template<class... layers>
struct NeuralNetwork {

	Chain<InputLayer, layers..., OutputLayer> network;

	template<class... integers>
	NeuralNetwork(integers... architecture) : network(architecture...) {}

	template<class tensor> auto forward_propagation	(const tensor& x) { return network.fp(x); }
	template<class tensor> auto back_propagation	(const tensor& y) { return network.back_propagation(y); }


	void set_batch_size(int size) { network.set_batch_size(size); }
	void write(std::ofstream& os) { network.write(os); }
	void read(std::ifstream& is)  { network.read(is);  }

	void update_weights()  			 { network.update_weights(); }
//	void clear_stored_gradients() 	 { network.clear_stored_gradients(); }
	void setLearningRate(fp_type lr) { network.setLearningRate(lr); }
	void initialize_variables()      { network.initialize_variables(); }
};

}
}

#endif /* NEURALNETWORK_H_ */