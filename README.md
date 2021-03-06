# Documentation 
Author: Joseph Jaspers

[![Build Status](https://travis-ci.org/josephjaspers/BlackCat_Tensors.svg?branch=master)](https://travis-ci.org/josephjaspers/BlackCat_Tensors)


BlackCat_Tensors (BCT) is a highly efficient NeuralNetwork and Tensor library. BCT is designed to support GPU computing (CUDA) and CPU multi-threading (OpenMP). BCT focuses on delivering a high-level framework with low-level performance.

#### Doxygen Documentation:
For a better source-tree/full method listing:
	[Source Tree Documentation](https://josephjaspers.github.io/BlackCat_Tensors_Doxygen/html/annotated.html)

#### Neural Networks:
BlackCat_Tensors supports various Neural Network primitives including:

Convolution, LSTM, RNN, FeedForward(Dense),  
Tanh, Logistic, Relu, Softmax, Softplus, Mish

Brief example:
```cpp

#include "BlackCat_NeuralNetworks.h"

int main() {

using namespace bc::nn;
auto system_tag = bc::host_tag();   //for CPU/OpenMP
auto system_tag = bc::device_tag(); //for CUDA 

auto network = neuralnetwork(
	feedforward(system_tag, 784, 256, bc::nn::adam),
	logistic(system_tag, 256),
	feedforward(system_tag, 256, 10, bc::mm::adam),
	softmax(system_tag, 10),
	outputlayer(system_tag, 10)
);

}
```
[MNIST example](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/examples/mnist_test/mnist_test.h)  
[MNIST example output](https://travis-ci.org/josephjaspers/BlackCat_Tensors)  
[Neural Network Documentation](https://josephjaspers.github.io/BlackCat_Tensors_Doxygen/html/namespaceBC_1_1nn.html)

Note* The neural-networks are considered to be in active development/experimental and are subject to
vast interface changes. The tensor/linear_algebra library however is significantly more well tested (for performance and accuracy). 

#### Setup:
BCT is a header only library that supports compilation with the NVCC and G++ BCT does not support any default BLAS routines, and must be linked with an apropriate BLAS library. Setting up simply requires adding the BlackCat_Tensors your path and including "BlackCat_Tensors.h"

#### Tested Compilers:  
NVCC: 9.2, 10, 10.1  
GCC: 7.4, 8.0   
clang: 6.0.0  
MSVC: VS2019 (_MSC_VER=1921) //CPU-only  
Cling: 0.6 //CPU-only

#### FAQ Fast Explanation:

CPU multithreading? Simply link OpenMP  
GPU multithreading? Simply link CUDA

How to choose allocation?

```cpp
bc::Vector<float, bc::Cuda_Allocator<float>> myVec(sz);    //Allocates data on the gpu
bc::Vector<double, bc::Basic_Allocator<double>> myVec(sz); //Allocates data on the cpu
bc::Vector<double>  myVec(sz);                             //defaults to bc::Basic_Allocator
```

#### Important: 
BCT must be linked to an appropriate BLAS implementation, and currently does not support any defaults.  
BLAS support is limited to floats, and doubles on the CPU, and floats on the GPU. (__future__, integer and complex support)

Non-numeric types are supported though they are considered to be a secondary concern. 

#### Documentation Index  

BlackCat Tensors attempts to align itself with the design of the C++ standard library to facilitate predictable syntax, and integration with the standard template library. Its design and documentation is divided amongst module-like components that are frequently concepts of the standard library. 

|Index| Concept | Brief |
| --- | --- | --- 
| 1 | [Tensor Methods](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/methods.md)| Brief documentation of the common tensor methods | 
| 2 | [Core Types](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/aliases.md) | List of primary Tensor-Types |
| 3 | [Allocators](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/allocators.md) | Overview of allocators in BCT and their functionality |
| 4 | [Iterators](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/iterators.md) | Overview of iterators in BCT and their functionality |
| 5 | [Algorithms](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/algorithms.md) | Brief overview of how algorithms are implemented, and a list of supported algorithms |
| 6 | [CMath Functions](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/cmath_functions.md) | A list of supported CMath functions |
| 7 | [Streams](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/streams.md) | Streams refer to streaming features offered by the cuda Library. |
| 8 | [Benchmarks](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/benchmarks.md) | Performance Testing |
| 9 | [PTX Code Generation](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/PTX_Generation.md) | A comparison of PTX code generated from handwritten kernels compared to BlackCat_Tensors (Hint: None) |


#### Dependencies 
	Cuda 9.2 (and up, Only required for GPU-usage)
	GCC 7.4 (and up)

	gomp  (Only required for CPU multithreading)
	cblas (Only required for BLAS functions)

	The cuda libraries are only required if the user wants to utilize GPU operations.
	gomp (or another openmp library) is only required for multithreading certain operations, though it is not required. 

#### Tests/Examples 
[tests and examples output (Travis build)](https://travis-ci.org/josephjaspers/BlackCat_Tensors) 



#### License
```
/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 ```
