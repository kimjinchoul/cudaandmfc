#pragma once

#pragma warning(disable:4311)
#pragma warning(disable:4819)

#pragma comment(lib, "cudart.lib ")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "cutil32d.lib")
#else
#pragma comment(lib, "cutil32.lib")
#endif

// CUDA Interface Declare
#include "CUDAKernel.h"
// includes, d3d9
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
// includes, cuda
#include <cuda.h>
#include <builtin_types.h>
#include <cuda_runtime_api.h>
#include <cuda_d3d9_interop.h>
// includes, project
#include <cutil.h>

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
	bool RegisterD3D9TextureBuffer(LPDIRECT3DTEXTURE9 pD3D9TextureBuffer);
	bool UnregisterD3D9TextureBuffer(void);
	bool InitD3D9TextureBuffer(void);
	bool CalVertexKernel(int iMeshWidth, int iMeshHeight, float fTime);
	bool CalTextureKernel(int iTextureWidth, int iTextureHeight, float fTime);
	

// 	int MallocMemA(int len);
// 	int InitTexture(float* init_data,int w, int h);
// 	void RunCUDA(LPDIRECT3DVERTEXBUFFER9 pVB);
// 	int TranslateResult(float* out_data);
// 	int ReleaseMem();
// 	int ReleaseTex();

private:
	CCUDACLASS(const CCUDACLASS &ccdD3DDisplayFramework);
	CCUDACLASS& operator=(const CCUDACLASS &ccdRhs);
	void CUDAErrCheck(LPCTSTR szErrMessage = NULL);
	//float	*device_result;
	//cudaArray* device_tex;
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DVERTEXBUFFER9 m_pTexVB;
	LPDIRECT3DTEXTURE9 m_pTB;
 	LPDIRECT3DDEVICE9 m_pD3D9DirectXDevice;

	//int m_ret_len;
	//float m_fTime;
	int m_iMeshWidth;
	int m_iMeshHeight;

	CCUDAKernel m_ccKernel;
};
