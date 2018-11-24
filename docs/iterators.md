# Iterators 

BlackCat_Tensors defines two types of iterators, coefficient-wise and n-dimensional. 

----------------------------------------------------------------------------------------------

##### Coefficient-Wise 
The coefficient-wise iterator returns references of the `scalar_type` of the tensor.

```cpp
int main() {

    BC::Matrix<float> matrix(5, 5); 
    
    for (float& val : matrix) {
      //do work 
    }
    
    //identical
    for (float& val : matrix.iter()) {
      //do work 
    }
}
```
**Warning** Tensor's allocated via non-managed Cuda memory may not normally use the coefficient-wise iterator. (As it causes dereferencing a device pointer from the host). User's may opt to use `BC` [algorithms](https://github.com/josephjaspers/BlackCat_Tensors/blob/master/docs/algorithms.md) to meet their needs.   

----------------------------------------------------------------------------------------------
  
    
   
##### N-Dimensional     
The n-dimensional iterator returns slices of the current tensor.

```cpp

int main() { 

  BC::Cube<float> cube(3,3,3); 

  for (auto matrix : cube.nd_iter()) {       
    for (auto row : cube.nd_iter()) {        
      for (float& scalar : row) {
         //do work 
      }
    }
  }
}
```

**Note** Calling `nd_iter()` on a Vector forwards to `iter()`. 

----------------------------------------------------------------------------------------------
##### Std-Style Iterators

Formal `std` style iterators are supported; using `begin` and `end`. 

```cpp
BC::Matrix<float> mat; 

  mat.begin();        
  mat.end();
  mat.rbegin();
  mat.rend();

  mat.nd_begin();
  mat.nd_end();
  mat.nd_rbegin();
  mat.nd_rend();
```
#### Utility Iterators
The Blackcat_Tensors defines convienant iterator proxies which support start and end ranges.

```cpp
BC::Matrix<float> mat; 

  for (float& scalar : mat.iter(start, finish)) {
    //do work
  }
  
  for (float& scalar : mat.nd_iter(start, finish)) {
    //do work
  }
  
  //reverse iterators are also supported.
  for (float& scalar : mat.reverse_iter(finish, start)) {
    //do work
  }
  
    for (float& scalar : mat.reverse_nd_iter(finish, start)) {
    //do work
  }

```