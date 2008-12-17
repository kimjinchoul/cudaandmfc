//#include "StdAfx.h"
#include "CUDAKernel.h"

__global__ void DoVertexkernel( float4* pos, unsigned int width, unsigned int height, float time)
{
	unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

	// calculate uv coordinates
	float u = x / (float) width;
	float v = y / (float) height;
	u = u*2.0f - 1.0f;
	v = v*2.0f - 1.0f;

	// calculate simple sine wave pattern
	float freq = 4.0f;
	float w = sinf(u*freq + time) * cosf(v*freq + time) * 0.5f;

	// write output vertex
	pos[y*width+x] = make_float4(u, w, v, __int_as_float(0xff00ff00));
}

CCUDAKernel::CCUDAKernel(void)
{
}

CCUDAKernel::~CCUDAKernel(void)
{
}

void CCUDAKernel::DoVertex(float4* pos, unsigned int width, unsigned int height, float time)
{
	dim3 block(8, 8, 1);
	dim3 grid(width / block.x, height / block.y, 1);

	DoVertexkernel<<<grid, block>>>(pos, width, height, time);
	return ;
}
