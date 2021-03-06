/*
 * Host.h
 *
 *  Created on: Dec 12, 2018
 *      Author: joseph
 */

#ifndef BC_UTILITY_HOST_H_
#define BC_UTILITY_HOST_H_

namespace bc {
namespace utility {

template<class SystemTag>
struct Utility;

template<>
struct Utility<host_tag> {

	template<class T, class U, class V>
	static void copy(T* to, U* from, V size) {
		BC_omp_for__
		for (int i = 0; i < size; ++i) {
			to[i] = from[i];
		}
	}

	template<class T, class U>
	static void HostToDevice(T* device_ptr, U* host_ptr, bc::size_t size) {
		copy(device_ptr, host_ptr, size);
	}

	template<class T, class U>
	static void DeviceToHost(T* host_ptr, U* device_ptr, bc::size_t size) {
		copy(host_ptr, device_ptr, size);
	}
};

}
}


#endif /* HOST_H_ */
