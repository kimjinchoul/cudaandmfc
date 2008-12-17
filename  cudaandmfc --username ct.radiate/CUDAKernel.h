#pragma once

// includes, cuda
#include <cuda.h>
#include <builtin_types.h>
#include <cuda_runtime_api.h>
#include <cuda_d3d9_interop.h>
// includes, project
#include <cutil.h>

__global__ void DoVertexkernel( float4* pos, unsigned int width, unsigned int height, float time);

class CCUDAKernel
{
public:
	CCUDAKernel(void);
	virtual ~CCUDAKernel(void);
	void DoVertex(float4* pos, unsigned int width, unsigned int height, float time);
};
