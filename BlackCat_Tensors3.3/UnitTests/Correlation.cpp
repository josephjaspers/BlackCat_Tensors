#include <cmath>
namespace BC {

template<class V, class T, class U>
	static void x_correlation(V& cor, const T& krnl, const U& img, int krnl_rank, const int* krnl_shape, int img_size, int img_rows) {

		int krnl_col_count = 1;
		for (int i = 1; i < krnl_rank; ++i) {
			krnl_col_count *= krnl_shape[i];
		}

		const int krnl_rows = krnl_shape[0];
		const int cor_rows = krnl_rows + img_rows - 1;
		int base_ = krnl_rows - 1;

		//handles the initial edge case
		for (int i = 0; i < base_; ++i){
			for (int j = 0; j < krnl_col_count; ++j)
				for (int k = 0; k > -1; --k) {
					cor[i + (krnl_col_count - 1 - j) * cor_rows] += krnl[krnl_rows - k - 1 + j * krnl_rows] * img[i + k];
			}
		}


		//handles the rest of the kernel
		for (int i = 0; i < img_size; ++i) {
				bool incremented = false;
					for (int j = 0; j < krnl_col_count; ++j) {

						if (incremented) {
							incremented = false;
							base_--;
						}

						for (int k = 0; k < krnl_rows; ++k) {

						if (k != 0 && !incremented)
							if (std::floor(i / img_rows) < std::floor((i + k)/ img_rows)) {
								incremented = true;
								base_++;
							}

						cor[i+ j * cor_rows + base_] += krnl[k + (krnl_col_count - j - 1)  * krnl_rows] * img[i + k];
				}
			}
		}
	}
}
