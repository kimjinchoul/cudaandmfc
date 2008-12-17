/********************************************************************
*  cuda_class.h
*  This is a example of the CUDA program.
*  aurthor: zhao.kaiyong(at)gmail.com
*  http://www.comp.hkbu.edu.hk/~kyzhao/
*********************************************************************/
#ifndef __CUDA_CLASS_H__
#define __CUDA_CLASS_H__

#pragma warning(disable:4311)
#pragma warning(disable:4819)


#include <d3dx9.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_d3d9_interop.h>
#include <builtin_types.h>

#include <cutil.h>
#include <driver_types.h>

#include "CUDAKernel.h"


class CCUDACLASS
{
public:
	CCUDACLASS(void);
	~CCUDACLASS(void);

	//bool InitCUDAD3D(LPDIRECT3DVERTEXBUFFER9 pVB);
	bool SetCUDACapableD3D9Device(LPDIRECT3D9 pD3D);
	bool LoadD3D9DirectXDevice(LPDIRECT3DDEVICE9 pD3D9DirectXDevice);
	bool RegisterD3D9VertexBuffer(LPDIRECT3DVERTEXBUFFER9 pVB);
	bool UnregisterD3D9VertexBuffer(void);
	bool CalVertexKernel(float fTime, int iMeshWidth, int iMeshHeight);
	

// 	int MallocMemA(int len);
// 	int InitTexture(float* init_data,int w, int h);
// 	void RunCUDA(LPDIRECT3DVERTEXBUFFER9 pVB);
// 	int TranslateResult(float* out_data);
// 	int ReleaseMem();
// 	int ReleaseTex();

private:
	void CUDAErrCheck(LPCTSTR szErrMessage = NULL);
	float	*device_result;
	cudaArray* device_tex;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
 	LPDIRECT3D9 m_pD3D;
 	LPDIRECT3DDEVICE9 m_pD3D9DirectXDevice;

	int m_ret_len;
	float m_fTime;
	int m_iMeshWidth;
	int m_iMeshHeight;

	CCUDAKernel m_ccKernel;
};

#endif // __CUDA_CLASS_H__