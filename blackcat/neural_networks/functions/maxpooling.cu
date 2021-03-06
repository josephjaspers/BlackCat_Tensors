/*
 * MaxPooling.h
 *
 *	Created on: Nov 10, 2019
 *			Author: joseph
 */

#ifdef __CUDACC__
#ifndef BLACKCAT_TENSORS_CAFFE_MAXPOOLING_CU_
#define BLACKCAT_TENSORS_CAFFE_MAXPOOLING_CU_


#include "common.h"
#include "../../common.h"
/*
 * THIS IS NOT AN ORIGINAL CAFFE FILE.
 * MAXPOOLING IMPLEMENTATION WAS ORIGINALLY CREATED BY THE CAFFE AUTHOR(S)
 */

namespace bc {
namespace caffe {

using bc::traits::min;
using bc::traits::max;

template <typename Dtype> __global__
void MaxPoolForward_gpu_kernel(const int nthreads,
		const Dtype* const bottom_data, const int num, const int channels,
		const int height, const int width, const int pooled_height,
		const int pooled_width, const int kernel_h, const int kernel_w,
		const int stride_h, const int stride_w, const int pad_h, const int pad_w,
		Dtype* const top_data, int* mask)
{
	BC_CUDA_KERNEL_LOOP_X(index, nthreads) {
		const int pw = index % pooled_width;
		const int ph = (index / pooled_width) % pooled_height;
		const int c = (index / pooled_width / pooled_height) % channels;
		const int n = index / pooled_width / pooled_height / channels;
		int hstart = ph * stride_h - pad_h;
		int wstart = pw * stride_w - pad_w;
		const int hend = bc::traits::min(hstart + kernel_h, height);
		const int wend = bc::traits::min(wstart + kernel_w, width);
		hstart = bc::traits::max(hstart, 0);
		wstart = bc::traits::max(wstart, 0);
		Dtype maxval = -FLT_MAX;
		int maxidx = -1;
		const Dtype* const bottom_slice =
				bottom_data + (n * channels + c) * height * width;
		for (int h = hstart; h < hend; ++h) {
			for (int w = wstart; w < wend; ++w) {
				if (bottom_slice[h * width + w] > maxval) {
					maxidx = h * width + w;
					maxval = bottom_slice[maxidx];
				}
			}
		}
		top_data[index] = maxval;
		mask[index] = maxidx;
	}
}


template <typename Dtype>
__global__ void MaxPoolBackward_gpu_kernel(
		const int nthreads, //size
		const Dtype* const top_diff,
		const int* const mask, const int num,
		const int channels, const int height, const int width,
		const int pooled_height, const int pooled_width, const int kernel_h,
		const int kernel_w, const int stride_h, const int stride_w, const int pad_h,
		const int pad_w, Dtype* const bottom_diff) {
	BC_CUDA_KERNEL_LOOP_X(index, nthreads) {
		// find out the local index
		// find out the local offset
		const int w = index % width;
		const int h = (index / width) % height;
		const int c = (index / width / height) % channels;
		const int n = index / width / height / channels;
		const int phstart =
				 (h + pad_h < kernel_h) ? 0 : (h + pad_h - kernel_h) / stride_h + 1;
		const int phend = bc::traits::min((h + pad_h) / stride_h + 1, pooled_height);
		const int pwstart =
				 (w + pad_w < kernel_w) ? 0 : (w + pad_w - kernel_w) / stride_w + 1;
		const int pwend = bc::traits::min((w + pad_w) / stride_w + 1, pooled_width);
		Dtype gradient = 0;
		const int offset = (n * channels + c) * pooled_height * pooled_width;
		const Dtype* const top_diff_slice = top_diff + offset;
			const int* const mask_slice = mask + offset;
			for (int ph = phstart; ph < phend; ++ph) {
				for (int pw = pwstart; pw < pwend; ++pw) {
					if (mask_slice[ph * pooled_width + pw] == h * width + w) {
						gradient += top_diff_slice[ph * pooled_width + pw];
					}
				}
			}
		bottom_diff[index] = gradient;
	}
}



template <typename Dtype>
void MaxPoolForward(
		bc::device_tag,
		const Dtype* const bottom_data,
		const int num, const int channels,
		const int height, const int width,
		const int pooled_height, const int pooled_width,
		const int kernel_h, const int kernel_w,
		const int stride_h, const int stride_w,
		const int pad_h, const int pad_w,
		Dtype* const top_data, int* mask)
{
	int size = pooled_height * pooled_width * channels * num;
	MaxPoolForward_gpu_kernel<<<
			bc::calculate_block_dim(size),
			bc::calculate_threads(size)>>>(
					size, bottom_data,
					num, channels,
					height, width,
					pooled_height, pooled_width,
					kernel_h, kernel_w,
					stride_h, stride_w,
					pad_h, pad_w,
					top_data, mask);
}

template <typename Dtype>
void MaxPoolBackward(
		bc::device_tag,
		const Dtype* const top_diff, const int* const mask,
		const int num,      const int channels,
		const int height,   const int width,
		const int pooled_height, const int pooled_width,
		const int kernel_h, const int kernel_w,
		const int stride_h, const int stride_w,
		const int pad_h,    const int pad_w,
		Dtype* bottom_diff)
{
	int size = pooled_height * pooled_width * channels * num;
	MaxPoolBackward_gpu_kernel<Dtype><<<
			bc::calculate_block_dim(size),
			bc::calculate_threads(size)>>>(
					size, top_diff, mask,
					num, channels,
					height, width,
					pooled_height, pooled_width,
					kernel_h, kernel_w,
					stride_h, stride_w,
					pad_h, pad_w,
					bottom_diff);
}

}
}

#endif /* MAXPOOLING_H_ */
#endif // __CUDACC__
