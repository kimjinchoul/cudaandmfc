#include "stdafx.h"
#include "CUDAUtility.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <cuda_runtime.h>
// #include <cutil.h>


CCUDACLASS::CCUDACLASS(void):m_pVB(NULL), m_pD3D(NULL), m_pD3D9DirectXDevice(NULL), m_pTexVB(NULL), \
	m_pTB(NULL), m_iMeshWidth(0), m_iMeshHeight(0)/*, m_fTime(0.0)*/
{
}

CCUDACLASS::~CCUDACLASS(void)
{
	m_pVB = NULL;
	m_pD3D = NULL;
	m_pTB = NULL;
	m_pTexVB = NULL;
	m_pD3D9DirectXDevice = NULL;
}

// bool CCUDACLASS::InitCUDAD3D(LPDIRECT3DVERTEXBUFFER9 pVB)
// {
// 	if(pVB == NULL)
// 	{
// 		AfxMessageBox(_T("D3D9VertexBuffer is Empty! Can not Register D3D9Vertex Buffer!"));
// 
// 		return false;
// 	}
// 	m_pVB = pVB;
// 	// Initialize interoperability between CUDA and Direct3D
// 	// Register vertex buffer with CUDA
// 	cudaD3D9RegisterResource(m_pVB, cudaD3D9RegisterFlagsNone);
// 	CUDAErrCheck(_T("cudaD3D9RegisterResource (pVB) failed"));
// 	cudaD3D9ResourceSetMapFlags(m_pVB, cudaD3D9MapFlagsWriteDiscard);
// 	CUDAErrCheck(_T("cudaD3D9ResourceSetMapFlags (pVB) failed"));
// 
// 	return true;
// }

bool CCUDACLASS::SetCUDACapableD3D9Device(LPDIRECT3D9 pD3D)
{
	if(pD3D == NULL)
	{
		AfxMessageBox(_T("IDIRECT3D9 is Empty! Can not Set D3D9Device!"));

		return false;
	}

	m_pD3D = pD3D;

	// Get primary display identifier
	D3DADAPTER_IDENTIFIER9 struAdapterID;

	// Find the first CUDA capable device
	int i;
	int iCUDADeviceNum;
	int dwAdapterSum = m_pD3D->GetAdapterCount();

	for(i=0; i<dwAdapterSum; i++)
	{
		m_pD3D->GetAdapterIdentifier(i, 0, &struAdapterID);
		cudaD3D9GetDevice(&iCUDADeviceNum, struAdapterID.DeviceName);
		if(cudaSuccess == cudaGetLastError())
		{
			break;
		}
	}
	if(i == dwAdapterSum)
	{
		AfxMessageBox(_T("No Available Device for CUDA!"));

		return false;
	}

	return true;
}

bool CCUDACLASS::LoadD3D9DirectXDevice(LPDIRECT3DDEVICE9 pD3D9DirectXDevice)
{
	if(pD3D9DirectXDevice == NULL)
	{
		AfxMessageBox(_T("D3D9DirectXDevice is Empty! Can not Load D3D9DirectXDevice!"));

		return false;
	}

	m_pD3D9DirectXDevice = pD3D9DirectXDevice;

	cudaD3D9SetDirect3DDevice(m_pD3D9DirectXDevice);
	CUDAErrCheck(_T("cudaD3D9SetDirect3DDevice failed"));

	return true;	
}

bool CCUDACLASS::RegisterD3D9VertexBuffer(LPDIRECT3DVERTEXBUFFER9 pVB)
{
	if(pVB == NULL)
	{
		AfxMessageBox(_T("D3D9VertexBuffer is Empty! Can not Register D3D9Vertex Buffer!"));

		return false;
	}

	m_pVB = pVB;

	cudaD3D9RegisterResource(m_pVB, cudaD3D9RegisterFlagsNone);
	CUDAErrCheck(_T("cudaD3D9RegisterResource (D3D9VertexBuffer) failed"));
	cudaD3D9ResourceSetMapFlags(m_pVB, cudaD3D9MapFlagsWriteDiscard);
	CUDAErrCheck(_T("cudaD3D9ResourceSetMapFlags (D3D9VertexBuffer) failed"));

	return true;
}

bool CCUDACLASS::UnregisterD3D9VertexBuffer(void)
{
	if(m_pVB == NULL)
	{
		AfxMessageBox(_T("D3D9VertexBuffer is Empty! Can not Unregister D3D9Vertex Buffer!"));

		return false;
	}

	cudaD3D9UnregisterResource(m_pVB);
	CUDAErrCheck(_T("cudaD3D9UnregisterResource (D3D9VertexBuffer) failed"));

	return true;
}

bool CCUDACLASS::RegisterD3D9TextureBuffer(LPDIRECT3DTEXTURE9 pD3D9TextureBuffer)
{
	if(pD3D9TextureBuffer == NULL)
	{
		AfxMessageBox(_T("D3D9TextureBuffer is Empty! Can not Cal D3D9Texture!"));

		return false;
	}

	m_pTB = pD3D9TextureBuffer;

	cudaD3D9RegisterResource(pD3D9TextureBuffer, cudaD3D9RegisterFlagsNone);
	CUDAErrCheck(_T("cudaD3D9RegisterResource (D3D9TextureBuffer) failed"));
	cudaD3D9ResourceSetMapFlags(pD3D9TextureBuffer, cudaD3D9MapFlagsWriteDiscard);
	CUDAErrCheck(_T("cudaD3D9ResourceSetMapFlags (D3D9TextureBuffer) failed"));

	return true;
}

bool CCUDACLASS::UnregisterD3D9TextureBuffer(void)
{
	if(m_pTB == NULL)
	{
		AfxMessageBox(_T("D3D9TextureBuffer is Empty! Can not Unregister D3D9Texture Buffer!"));

		return false;
	}

	cudaD3D9UnregisterResource(m_pTB);
	CUDAErrCheck(_T("cudaD3D9UnregisterResource (D3D9VertexBuffer) failed"));

	return true;
}

bool CCUDACLASS::InitD3D9TextureBuffer(void)
{
	float4 *lpD3D9TextureBuffer = NULL;
	size_t dwTextureBufferSize;

	if(m_pTB == NULL)
	{
		AfxMessageBox(_T("D3D9TextureBuffer is Empty! Can not Cal D3D9Texture!"));

		return false;
	}

	// CUDA Map call to the Vertex Buffer and return a pointer
	cudaD3D9MapResources(1, (IDirect3DResource9 **)&m_pTB);
	CUDAErrCheck(_T("cudaD3D9MapResources failed"));
	cudaD3D9ResourceGetMappedPointer((void **)&lpD3D9TextureBuffer, m_pTB, 0, 0);
	cudaD3D9ResourceGetMappedSize(&dwTextureBufferSize, m_pTB, 0, 0);

	// ZeroMemory
	cudaMemset(lpD3D9TextureBuffer, 0, dwTextureBufferSize);

	// CUDA Map Unmap vertex buffer
	cudaD3D9UnmapResources(1, (IDirect3DResource9 **)&m_pTB);
	CUDAErrCheck(_T("cudaD3D9UnmapResources failed"));

	return true;
}

void CCUDACLASS::CUDAErrCheck(LPCTSTR szErrMessage)
{
	bool bResSign = true;
	cudaError_t ceErr;
	
	ceErr = cudaGetLastError();
	if(ceErr != cudaSuccess)
	{
		CString csErrStr;

		if(szErrMessage == NULL)
		{
			csErrStr = _T("CUDA Running Error");
		}
		else
		{
			csErrStr.Format(_T("CUDA Running Error, %s"), szErrMessage);
		}
		bResSign = false;
		AfxMessageBox(csErrStr);
	}

	ceErr = cudaThreadSynchronize();
	if(ceErr != cudaSuccess)
	{
		CString csErrStr;

		if(szErrMessage == NULL)
		{
			csErrStr = _T("ThreadSynchronize Error");
		}
		else
		{
			csErrStr.Format(_T("ThreadSynchronize Error, %s"), szErrMessage);
		}
		bResSign = false;
		AfxMessageBox(csErrStr);
	}

	if(!bResSign)
	{
		exit(EXIT_FAILURE);
	}

	return;
}

// int CCUDACLASS::MallocMemA(int len)
// {
// 	m_ret_len = len;
// 	CUDA_SAFE_CALL( cudaMalloc((void**) &device_result, sizeof(float) * m_ret_len));
// 	return 1;
// }

bool CCUDACLASS::CalVertexKernel( int iMeshWidth, int iMeshHeight, float fTime )
{
	//HelloCUDA<<<1, 1, 0>>>(device_result, m_ret_len);
	//CUT_CHECK_ERROR("Kernel execution failed\n");
	//CUDAErrCheck(_T("cudaD3D9ResourceSetMapFlags (pVB) failed"));
	float4* dptr = NULL;

	if(m_pVB == NULL)
	{
		AfxMessageBox(_T("D3D9VertexBuffer is Empty! Can not Cal D3D9Vertex!"));

		return false;
	}
	// CUDA Map call to the Vertex Buffer and return a pointer
	cudaD3D9MapResources(1, (IDirect3DResource9 **)&m_pVB);
	CUDAErrCheck(_T("cudaD3D9MapResource failed"));
	cudaD3D9ResourceGetMappedPointer( (void **)&dptr, m_pVB, 0, 0);
	CUDAErrCheck(_T("cudaD3D9ResourceGetMappedPointer failed"));

    // Execute kernel
    //simpleD3DKernel(dptr, iMeshWidth, iMeshHeight, fTime);
	m_ccKernel.DoVertex(dptr, iMeshWidth, iMeshHeight, fTime);

    // CUDA Map Unmap vertex buffer
	cudaD3D9UnmapResources(1, (IDirect3DResource9 **)&m_pVB);
	CUDAErrCheck(_T("cudaD3D9UnmapResources failed"));

	return true;
}

bool CCUDACLASS::CalTextureKernel(int iTextureWidth, int iTextureHeight, float fTimeStamp)
{
	float4 *lpD3D9TextureBuffer = NULL;
	size_t dwTextureBufferPitch;

	if(m_pTB == NULL)
	{
		AfxMessageBox(_T("D3D9TextureBuffer is Empty! Can not Cal D3D9Texture!"));

		return false;
	}

	// CUDA Map call to the Vertex Buffer and return a pointer
	cudaD3D9MapResources(1, (IDirect3DResource9 **)&m_pTB);
	CUDAErrCheck(_T("cudaD3D9MapResources failed"));
	cudaD3D9ResourceGetMappedPointer((void **)&lpD3D9TextureBuffer, m_pTB, 0, 0);
	cudaD3D9ResourceGetMappedPitch(&dwTextureBufferPitch, NULL, m_pTB, 0, 0);

	// Execute kernel
	m_ccKernel.DoTexture(lpD3D9TextureBuffer, iTextureWidth, iTextureHeight, dwTextureBufferPitch, fTimeStamp);

	// CUDA Map Unmap vertex buffer
	cudaD3D9UnmapResources(1, (IDirect3DResource9 **)&m_pTB);
	CUDAErrCheck(_T("cudaD3D9UnmapResources failed"));

	return true;
}

// int CCUDACLASS::TranslateResult(float * out_data)
// {
// 	CUDA_SAFE_CALL( cudaMemcpy(out_data, device_result, sizeof(float) * m_ret_len, cudaMemcpyDeviceToHost));
// 	return 1;
// }
// 
// int CCUDACLASS::ReleaseMem()
// {
// 	CUDA_SAFE_CALL( cudaFree(device_result));
// 	CUDA_SAFE_CALL( cudaFreeArray(device_tex));
// 	//CUDA_SAFE_CALL( cudaUnbindTexture(tex));
// 	return 1;
// }
// 
// int CCUDACLASS::InitTexture(float* init_data, int w, int h)
// {
// 	cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();
// 
// 	CUDA_SAFE_CALL( cudaMallocArray(&device_tex, &channelDesc, w, h));
// 	CUDA_SAFE_CALL( cudaMemcpyToArray(device_tex, 0, 0, init_data, sizeof(float)* w*h , cudaMemcpyHostToDevice));
// //	CUDA_SAFE_CALL( cudaBindTextureToArray(tex, device_tex, channelDesc));
// 	return 1;
// }

