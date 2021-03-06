
#include "CGPUDecoder.h"

CGPUDecoder::CGPUDecoder(size_t _nb_frames, size_t n, size_t k, size_t m)
{
    cudaError_t Status;

	sz_nodes	= n * _nb_frames;
	sz_checks	= k * _nb_frames;
	sz_msgs		= m * _nb_frames;

	nb_frames = _nb_frames;

	size_t s = (2 * n + m) * sizeof(float);
	size_t o = ((2 * sz_nodes + m + sz_msgs)/1024)*4;
	printf("(GPU) Memory per decoder : %d ko (%d Mo)\n", (s+o)/1024, (s+o)/1024/1024);

    CUDA_MALLOC_DEVICE(&d_transpose, m, __FILE__, __LINE__);
    Status = cudaMemcpy(d_transpose, PosNoeudsVariable, m * sizeof(unsigned int), cudaMemcpyHostToDevice);
    ERROR_CHECK(Status, (char*)__FILE__, __LINE__);

    CUDA_MALLOC_DEVICE(&d_MSG_C_2_V, sz_msgs, __FILE__, __LINE__);
    CUDA_MALLOC_DEVICE(&device_V, sz_nodes, __FILE__, __LINE__);
}


CGPUDecoder::~CGPUDecoder()
{
	cudaError_t Status;
	Status = cudaFree(device_V);
    ERROR_CHECK(Status, (char*)__FILE__, __LINE__);

	Status = cudaFree(d_transpose);
    ERROR_CHECK(Status, (char*)__FILE__, __LINE__);

    Status = cudaFree(d_MSG_C_2_V);
    ERROR_CHECK(Status, (char*)__FILE__, __LINE__);

    ERROR_CHECK(Status, (char*)__FILE__, __LINE__);
}

