author: Joseph Jaspers
project: BlackCat_Tensors
folder: BC_Internals


BC_MathLibraries:

	Stores the files relating to any calls that differ between CPU/GPU backends.
	Any call that would differ accross CUDA and standard CPU will belong to these files.

	An example of this would be 
		'template<class T>void initialize(T*& memptr, int size)'

	The CPU implementation is simply 'memptr = new T[size];'
        while CUDA's implementation is 'cudaMalloc((void**) &t, sizeof(T) * sz);'
	

	The CPU/GPU backends have an identical methods, which enables them to be 'dropped in/out' 
	of the Tensor classes without changing any code. They are essentially the interface to the 		mathematical functions.

BC_Tensor:
	
	This is the core of BlackCat_Tensors, and contains all CPU/GPU 'common' code. 
	The classes inside generally store a 'type' that ties them to a CPU/GPU backend.