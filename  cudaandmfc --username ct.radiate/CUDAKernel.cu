//#include "StdAfx.h"
#include "CUDAKernel.h"

__global__ void DoVertexKernel( float4* pos, unsigned int width, unsigned int height, float time)
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

__global__ void DoTextureKernel(unsigned char* lpCUDATexture, int iTextureWidth, int iTextureHeight, size_t dwTextureBufferPitch, float fTimeStamp)
{
    int x = blockIdx.x*blockDim.x + threadIdx.x;
    int y = blockIdx.y*blockDim.y + threadIdx.y;
    float *pixel;
       
    // in the case where, due to quantization into grids, we have
    // more threads than pixels, skip the threads which don't 
    // correspond to valid pixels
	if (x >= iTextureWidth || y >= iTextureHeight)
	{
		return;
	}
	
    // get a pointer to the pixel at (x,y)
    pixel = (float *)(lpCUDATexture+y*dwTextureBufferPitch) + 4*x;

	// populate it
	float value_x = 0.5f + 0.5f*cos(fTimeStamp+10.0f*((2.0f*x)/iTextureWidth-1.0f));
	float value_y = 0.5f + 0.5f*cos(fTimeStamp+10.0f*((2.0f*y)/iTextureHeight-1.0f));

	pixel[0] = 0.5*pixel[0] + 0.5*pow(value_x, 3.0f); // red
	pixel[1] = 0.5*pixel[1] + 0.5*pow(value_y, 3.0f); // green
	pixel[2] = 0.5f + 0.5f*cos(fTimeStamp); // blue
	pixel[3] = 1; // alpha

	return;
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

	DoVertexKernel<<<grid, block>>>(pos, width, height, time);
	return ;
}

void CCUDAKernel::DoTexture(float4 *lpCUDATexture, int iTextureWidth, int iTextureHeight, size_t dwTextureBufferPitch, float fTimeStamp)
{
	dim3 dimBlock(16, 16);
	dim3 dimGrid(iTextureWidth/dimBlock.x, iTextureHeight/dimBlock.y);
    //dim3 dimGrid((iTextureWidth+dimBlock.x-1)/dimBlock.x, (iTextureHeight+dimBlock.y-1)/dimBlock.y);

    DoTextureKernel<<<dimGrid, dimBlock>>>((unsigned char*)lpCUDATexture, iTextureWidth, iTextureHeight, dwTextureBufferPitch, fTimeStamp);

	return;
}