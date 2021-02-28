extern "C" {
#include "bitmap.h"
}
#include"helper_cuda.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>


#define OUTFILE "output.bmp"

__global__ void bw_kernel(RgbTriple* img, int n, int x_stride, int y_stride)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	float L[3] = {4,4,-1}, C[3]={0,12,0}, I[3] = {0,0,0};
	float N[3], S[3];
	float R = 4.4, W_max = 10, delta_x = 2 * W_max / (float) n;
	float delta_z = delta_x;
	int b;


	for (int i = x; i < n; i += x_stride) {
		for (int j = y; j < n; j += y_stride) {
			float W_x = i * delta_x - W_max;
			float W_z = j * delta_z - W_max;
			float D = R * R - (W_x - C[0]) * (W_x - C[0]) - 
			(W_z - C[2]) * (W_z - C[2]);
			if(D < 0){
				b = 0;
			}
			else{
				I[1] = C[1] - sqrtf(D);
				I[0] = W_x; I[2] = W_z;
				float I_min_C_norm = sqrtf(powf(I[0]-C[0],2) + powf(I[1]-C[1],2) + powf(I[2]-C[2],2));
				float L_min_I_norm = sqrtf(powf(L[0]-I[0],2) + powf(L[1]-I[1],2) + powf(L[2]-I[2],2));
				N[0] = (I[0] - C[0]) / I_min_C_norm; N[1] = (I[1] - C[1]) / I_min_C_norm;
				N[2] = (I[2] - C[2]) / I_min_C_norm;
				S[0] = (L[0] - I[0]) / L_min_I_norm; S[1] = (L[1] - I[1]) / L_min_I_norm;
				S[2] = (L[2] - I[2]) / L_min_I_norm;
				float dot_prod = N[0] * S[0] + N[1] * S[1] + N[2] * S[2];
				if (dot_prod > 0){
					b = (int) floorf(dot_prod * 255); 
					//b = 170;
				}
				else{
					b = 0;
				}
			}

			RgbTriple* p = &img[i * n + j];
			p->red = b;
			p->green = b;
			p->blue = b;
		}
	}

	return;
}

int main(int argc, char* argv[])
{
	int image_size, grid_dim_x, grid_dim_y, block_dim_x, block_dim_y, n;
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, 0);
		
	if (argc < 6) {
		fprintf(stderr, "5 arguments in this order:  image size,  grid dim x,"
		" grid dim y, block dim x, and block dim y\n");
		exit(EXIT_FAILURE);
	}

	n = atoi(argv[1]);
	image_size = n * n;
	grid_dim_x = atoi(argv[2]); grid_dim_y = atoi(argv[3]);
	block_dim_x = atoi(argv[4]); block_dim_y = atoi(argv[5]);

	if(prop.maxThreadsPerBlock < block_dim_x * block_dim_y){
		fprintf(stderr, "illegal block dimension!\n");
		return -1;
	}

	cudaEvent_t tick, tock;
	int x_stride = grid_dim_x * block_dim_x;
	int y_stride = grid_dim_y * block_dim_y;


	/* allocate memory for bmp on host */
	RgbTriple* img = (RgbTriple*) malloc(image_size * sizeof(RgbTriple));

	/* allocates GPU memory for bmp*/
	RgbTriple* d_img;
	checkCudaErrors(cudaMalloc((void**)&d_img, sizeof(RgbTriple) * image_size));
	checkCudaErrors(cudaMemcpy(d_img, img, sizeof(RgbTriple) * image_size, cudaMemcpyHostToDevice));

	/* Set gridDim and blockDim */
	dim3 blockDim(block_dim_x, block_dim_y, 1);
	dim3 gridDim(grid_dim_x, grid_dim_y, 1);


	cudaEventCreate(&tick);
	cudaEventCreate(&tock);

	cudaEventRecord(tick, 0);

	/* how to check kernel errors
	https://developer.nvidia.com/blog/how-query-device-properties-and-handle-errors-cuda-cc/
	*/
	bw_kernel<<<gridDim, blockDim>>>(d_img, n, x_stride, y_stride);

	/* stop timing */
	cudaEventRecord(tock, 0);
	cudaEventSynchronize(tock);

	cudaError_t errSync  = cudaGetLastError(); //gets last error off stack
cudaError_t errAsync = cudaDeviceSynchronize(); //This is because of asynchornicity off kernel 

if (errSync != cudaSuccess){
  printf("Sync kernel error: %s\n", cudaGetErrorString(errSync));
  return -1;
}
if (errAsync != cudaSuccess){
  printf("Async kernel error: %s\n", cudaGetErrorString(errAsync));
}


	float time;
	cudaEventElapsedTime(&time, tick, tock);
	cudaEventDestroy(tick); cudaEventDestroy(tock);
	fprintf(stderr, "runtime: %f seconds \t%f ms\n", time / 1000000, time);
	printf("%f\n", time);

	// Get img from device and write it to file
	checkCudaErrors(cudaMemcpy(img, d_img, sizeof(RgbTriple) * image_size, cudaMemcpyDeviceToHost));
	
	save_bitmap(img, n, n, OUTFILE);

	free(img);
	checkCudaErrors(cudaFree(d_img));
	
	
}
