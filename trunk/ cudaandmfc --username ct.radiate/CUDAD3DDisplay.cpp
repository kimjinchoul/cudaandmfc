#include "StdAfx.h"
#include "CUDAD3DDisplay.h"


CCUDAD3DDisplay::CCUDAD3DDisplay(void): m_hDisplayWnd(NULL), m_fDisplayZoom(1.0f), /*m_lpResetDataPointer(NULL), */m_pD3D(NULL), m_pD3DDevice(NULL), \
	m_pVertexBuffer(NULL), m_pImageTexture(NULL)
{
	memset(&m_struTextInfo, 0, sizeof(StruTextureInfo));
	memset(&m_struHALCaps, 0, sizeof(D3DCAPS9));
	memset(&m_struCurrentMode, 0, sizeof(D3DDISPLAYMODE));
	memset(&m_struPresentParameters, 0, sizeof(D3DPRESENT_PARAMETERS));
	memset(&m_struCreationParameters, 0, sizeof(D3DDEVICE_CREATION_PARAMETERS));
}

CCUDAD3DDisplay::~CCUDAD3DDisplay(void)
{
	Cleanup();
}

bool CCUDAD3DDisplay::InitCUDAD3DDisplay(HWND hDisplayDevice)
{
	if(hDisplayDevice == NULL)
	{
		AfxMessageBox(_T("DisplayDevice Handle is Empty!"));		

		return false;
	}
	m_hDisplayWnd = hDisplayDevice;

	if(!PreInit())
	{
		return false;
	}
	if(!InitD3D())
	{
		return false;
	}
	if(!PostInit())
	{
		return false;
	}

// 	if(!m_ccclass.InitCUDA())
// 	{
// 		return false;
// 	}

// 	if(!m_ccclass.InitCUDAD3D(m_pVertexBuffer))
// 	{
// 		return false;
// 	}

	return true;
}

bool CCUDAD3DDisplay::DisplayTexture( /*CPgmSlice *lpTexture,*/ D3DFORMAT StruTextFormat, int iPixelBytesNum )
{
//	int iTestAdaptedTextW, iTestAdaptedTextH;

// 	if(lpTexture->m_lpImageBuffer == NULL)
// 	{
// 		AfxMessageBox(_T("FlowTexture is Empty!"));
// 
// 		return false;
// 	}

	// Test
// 	TextureSizeAdapt(lpTexture->m_iImageWidth, lpTexture->m_iImageHeight, &iTestAdaptedTextW, &iTestAdaptedTextH);
// 
// 	if(iTestAdaptedTextW != m_struTextInfo.iTextWidth || iTestAdaptedTextH != m_struTextInfo.iTextHeight || \
// 		StruTextFormat != m_struTextInfo.StruTextFormat || iPixelBytesNum != m_struTextInfo.iPixelBytesNum || m_pImageTexture == NULL)
// 	{
// 		if(FAILED(CreateCustomTextures(lpTexture->m_iImageWidth, lpTexture->m_iImageHeight, StruTextFormat, iPixelBytesNum)))
// 		{
// 			AfxMessageBox(_T("Init Texture Failed!"));
// 
// 			return false;
// 		}
// 
// 		if(FAILED(CreateGeometry()))
// 		{
// 			AfxMessageBox(_T("Init Geometry Failed!"));
// 
// 			return false;
// 		}
// 	}
// 
// 	if(FAILED(LoadTextureData(lpTexture->m_lpImageBuffer, lpTexture->m_iImageWidth, lpTexture->m_iImageHeight)))
// 	{
// 		AfxMessageBox(_T("Load Texture is Empty!"));
// 
// 		return false;
// 	}
// 	m_lpResetDataPointer = lpTexture;

	// Render
	if(FAILED(PreRender()))
	{
		AfxMessageBox(_T("PreRender Failed!"));

		return false;
	}

	if(FAILED(Render()))
	{
		AfxMessageBox(_T("Render Failed!"));

		return false;
	}

	if(FAILED(PostRender()))
	{
		AfxMessageBox(_T("PostRender Failed!"));

		return false;
	}

	return true;
}

bool CCUDAD3DDisplay::DisplayVertex( float fTime )
{
	//Run Kernel
	if(!m_ccclass.CalVertexKernel(fTime, m_iMeshWidth, m_iMeshHeight))
	{
		AfxMessageBox(_T("Kernel Failed!"));

		return false;
	}
	
	// Render
	if(FAILED(PreRender()))
	{
		AfxMessageBox(_T("PreRender Failed!"));

		return false;
	}

	// Setup the world, view, and projection matrices
	SetupMatrices();

	if(FAILED(Render()))
	{
		AfxMessageBox(_T("Render Failed!"));

		return false;
	}

	if(FAILED(PostRender()))
	{
		AfxMessageBox(_T("PostRender Failed!"));

		return false;
	}



	return true;
}

bool CCUDAD3DDisplay::Cleanup(void)
{
	if(!PreTerminate())
	{
		AfxMessageBox(_T("PreTerminate Failed!"));

		return false;
	}

	if(!PostTerminate())
	{
		AfxMessageBox(_T("PostTerminate Failed!"));

		return false;
	}

	return true;
}

bool CCUDAD3DDisplay::SetDisplayZoom(float fDisplayZoom)
{
	if(fDisplayZoom <= EPSILON || fDisplayZoom > 1.5f-EPSILON)
	{
		return false;
	}

	m_fDisplayZoom = fDisplayZoom;

	return true;
}

bool CCUDAD3DDisplay::PreInit(void)
{
		//Set the default device value to NULL
	m_pD3DDevice = NULL;
	m_pVertexBuffer = NULL;
	m_pImageTexture = NULL;

	memset(&m_struTextInfo, 0, sizeof(StruTextureInfo));
	memset(&m_struHALCaps, 0, sizeof(D3DCAPS9));
	memset(&m_struCurrentMode, 0, sizeof(D3DDISPLAYMODE));
	memset(&m_struPresentParameters, 0, sizeof(D3DPRESENT_PARAMETERS));
	memset(&m_struCreationParameters, 0, sizeof(D3DDEVICE_CREATION_PARAMETERS));

	return true;
}

HRESULT CCUDAD3DDisplay::PreRender(void)
{
	HRESULT hr = S_OK;

	hr = m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//Call BeginScene to set up the device
	m_pD3DDevice->BeginScene();

	return hr;
}

bool CCUDAD3DDisplay::PreTerminate(void)
{
	// Unregister D3D9VertexBuffer for CUDA
	if(!m_ccclass.UnregisterD3D9VertexBuffer())
	{
		return false;
	}
// 	if(FAILED(DestroyTextureObject()))
// 	{
// 		AfxMessageBox(_T("DestroyTextureObject Failed!"));
// 
// 		return false;
// 	}

	if(FAILED(DestroyGeometry()))
	{
		AfxMessageBox(_T("DestroyGeometry Failed!"));

		return false;
	}

	if(FAILED(DestroyDevice()))
	{
		AfxMessageBox(_T("DestroyDevice Failed!"));

		return false;
	}

	if(FAILED(DestroyD3D()))
	{
		AfxMessageBox(_T("DestroyD3D Failed!"));

		return false;
	}

	return true;
}

bool CCUDAD3DDisplay::InitD3D(void)
{
	//Create the actual D3D object.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//Return a default value
	return true;
}

HRESULT CCUDAD3DDisplay::Render(void)
{
	HRESULT hr = S_OK;
	//for texture
	//hr = m_pD3DDevice->SetTexture(0, m_pImageTexture);

	//for vertex
	hr |= m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(StruCustomVertex) );
	hr |= m_pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	hr |= m_pD3DDevice->DrawPrimitive( D3DPT_POINTLIST, 0, m_iNumVertices );
	//hr |= m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return hr;
}

bool CCUDAD3DDisplay::PostInit(void)
{
	if(FAILED(SetWindowedDevice()))
	{
		return false;
	}

	if(FAILED(SetupDevice()))
	{
		return false;
	}

	if(FAILED(CreateGeometry()))
	{
		AfxMessageBox(_T("Init Geometry Failed!"));

		return false;
	}

	if (!m_ccclass.SetCUDACapableD3D9Device(m_pD3D))
	{
		return false;
	}

	if (!m_ccclass.LoadD3D9DirectXDevice(m_pD3DDevice))
	{
		return false;
	}

	if(!m_ccclass.RegisterD3D9VertexBuffer(m_pVertexBuffer))
	{
		return false;
	}


	//VerifyModes();


	return true;
}

HRESULT CCUDAD3DDisplay::PostRender(void)
{
	int i;
	int iTestCount = 3;
	HRESULT hr = S_OK;

	// End the scene
	hr = m_pD3DDevice->EndScene();
	for(i=0; i<iTestCount; i++)
	{
		if((hr = m_pD3DDevice->Present(NULL, NULL, NULL, NULL)) != D3D_OK)
		{
			if(!PreReset())
			{
				continue;
			}
			if(FAILED(RestoreDevice()))
			{
				continue;
			}
			if(!PostReset())
			{
				continue;
			}
		}
		else
		{
			break;
		}
	}

	return hr;
}

bool CCUDAD3DDisplay::PostTerminate(void)
{
	//m_lpResetDataPointer = NULL;

	return true;
}

bool CCUDAD3DDisplay::HandleMessage(MSG *pMessage)
{
	if(pMessage->message == WM_KEYDOWN && pMessage->wParam == VK_ESCAPE)
	{
		return false;
	}

	return true;
}

void CCUDAD3DDisplay::SetupMatrices()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 3.0f, -2.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return ;
}

bool CCUDAD3DDisplay::PreReset(void)
{
// 	if(FAILED(DestroyTextureObject()))
// 	{
// 		return false;
// 	}

	if(FAILED(DestroyGeometry()))
	{
		return false;
	}
	// Unregister D3D9VertexBuffer for CUDA
	if(!m_ccclass.UnregisterD3D9VertexBuffer())
	{
		return false;
	}

	return true;
}

bool CCUDAD3DDisplay::PostReset(void)
{
	if(FAILED(SetupDevice()))
	{
		return false;
	}

// 	if(FAILED(CreateCustomTextures(m_lpResetDataPointer->m_iImageWidth, m_lpResetDataPointer->m_iImageHeight, m_struTextInfo.StruTextFormat, m_struTextInfo.iPixelBytesNum)))
// 	{
// 		return false;
// 	}

	if(FAILED(CreateGeometry()))
	{
		AfxMessageBox(_T("Init Geometry Failed!"));

		return false;
	}

	//Init CUDAD3D Interface
	if (!m_ccclass.SetCUDACapableD3D9Device(m_pD3D))
	{
		return false;
	}

	if (!m_ccclass.LoadD3D9DirectXDevice(m_pD3DDevice))
	{
		return false;
	}

	if(!m_ccclass.RegisterD3D9VertexBuffer(m_pVertexBuffer))
	{
		return false;
	}

// 	if(FAILED(LoadTextureData(m_lpResetDataPointer->m_lpImageBuffer, m_lpResetDataPointer->m_iImageWidth, m_lpResetDataPointer->m_iImageHeight)))
// 	{
// 		return false;
// 	}

	return true;
}

void CCUDAD3DDisplay::VerifyModes(void)
{
	//Get the device caps
	m_pD3DDevice->GetDeviceCaps(&m_struHALCaps);

	//This is a fairly sloppy way to do this, but we don't have
	//a better method to do UI (yet). You may want to comment
	//these lines out once you understand the limitations of your device.
	if(!(m_struHALCaps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER))
	{
		AfxMessageBox(_T("The Border Addressing mode is not available."));
	}

	if(!(m_struHALCaps.TextureAddressCaps & D3DPTADDRESSCAPS_CLAMP))
	{
		AfxMessageBox(_T("The Clamp Addressing mode is not available."));
	}

	if(!(m_struHALCaps.TextureAddressCaps & D3DPTADDRESSCAPS_MIRROR))
	{
		AfxMessageBox(_T("The Mirror Addressing mode is not available."));
	}

	if(!(m_struHALCaps.TextureAddressCaps & D3DPTADDRESSCAPS_MIRRORONCE))
	{
		AfxMessageBox(_T("The Mirror Once Addressing mode is not available."));
	}

	if(!(m_struHALCaps.TextureAddressCaps & D3DPTADDRESSCAPS_WRAP))
	{
		AfxMessageBox(_T("The Wrap Addressing mode is not available."));
	}

	//We are going to assume that if the device supports a given filter
	//type for the magnification filter that it will support it for the 
	//minification and mip map filters as well. THIS IS NOT NECESSARILY
	//GUARANTEED TO BE TRUE, but it should give you a general feel for
	//what your device is capable of. If you need to check, replicate these
	//lines replacing MAG with MIN and/or MIP.
	if(!(m_struHALCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFPOINT))
	{
		AfxMessageBox(_T("The Point Filtering mode is not available."));
	}

	if(!(m_struHALCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
	{
		AfxMessageBox(_T("The Linear Filtering mode is not available."));
	}

	if(!(m_struHALCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC))
	{
		AfxMessageBox(_T("The Anisotropic Filtering mode is not available."));
	}

	if(!(m_struHALCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFGAUSSIANQUAD))
	{
		AfxMessageBox(_T("The Gaussian Quad Filtering mode is not available."));
	}

	return;
}

HRESULT CCUDAD3DDisplay::SetWindowedDevice(void)
{
	memset(&m_struCurrentMode, 0, sizeof(D3DDISPLAYMODE));
	memset(&m_struPresentParameters, 0, sizeof(D3DPRESENT_PARAMETERS));
	memset(&m_struCreationParameters, 0, sizeof(D3DDEVICE_CREATION_PARAMETERS));

	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_struCurrentMode)))
	{
		return E_FAIL;
	}

	//Since this is a convenience function for the developer,
	//make sure your settings make sense for your hardware.
	//These settings work well for a geForce3.
	m_struPresentParameters.Windowed = TRUE;
	m_struPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_struPresentParameters.BackBufferFormat = m_struCurrentMode.Format;

	m_struCreationParameters.AdapterOrdinal = D3DADAPTER_DEFAULT;
	m_struCreationParameters.DeviceType     = D3DDEVTYPE_HAL;
	m_struCreationParameters.hFocusWindow   = m_hDisplayWnd;
	m_struCreationParameters.BehaviorFlags  = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	return CreateDevice();
}

HRESULT CCUDAD3DDisplay::CreateDevice(void)
{
	return 	m_pD3D->CreateDevice(m_struCreationParameters.AdapterOrdinal, \
		m_struCreationParameters.DeviceType, \
		m_struCreationParameters.hFocusWindow, \
		m_struCreationParameters.BehaviorFlags, \
		&m_struPresentParameters, \
		&m_pD3DDevice);
}

HRESULT CCUDAD3DDisplay::SetupDevice(void)
{
	HRESULT hr = S_OK;

	// Set WorldMatrix
	D3DXMatrixIdentity(&m_struWorldMatrix);
	hr |= m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_struWorldMatrix);

	// Set ViewMatrix
	D3DXVECTOR3 vecEye( 0, 0, -1);
	D3DXVECTOR3 vecAt( 0, 0, 0);
	D3DXVECTOR3 vecUp( 0, 1, 0);
	D3DXMATRIXA16 matView, matProj;
	D3DXMatrixLookAtLH(&m_struViewMatrix, &vecEye, &vecAt, &vecUp);
	hr |= m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_struViewMatrix);

	// Set ProjMatrix
	CRect rt;
	float fAspect;

	hr |= m_pD3DDevice->GetScissorRect(&rt);
	fAspect = (float)rt.Width() / (float)rt.Height();
	D3DXMatrixOrthoLH(&m_struProjMatrix, (float)rt.Width(), (float)rt.Height(), -1.0f, 1.0f); // 平行投影
	hr |= m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_struProjMatrix);

	// Render State
	hr |= m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	hr |= m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	hr |= m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// Texture State
// 	hr |= m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	hr |= m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
// 	hr |= m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
// 	hr |= m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
// 	hr |= m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
// 	hr |= m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
// 	//hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
// 	//hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
// 	//hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 7);
// 	hr |= m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 	hr |= m_pD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	return hr;
}

HRESULT CCUDAD3DDisplay::RestoreDevice(void)
{
	int i;
	int iTestCount = 3;
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	//If the device is lost, enter a loop waiting for
	//it to be restored.
	if(hr == D3DERR_DEVICELOST)
	{
		//Keep testing the level until it says we 
		//can reset.
		for(i=0; i<iTestCount; i++)
		{
			//Give up control to other applications
			Sleep(1000);

			//Check to see if things are ready to be reset
			hr = m_pD3DDevice->TestCooperativeLevel();
			if(hr == D3DERR_DEVICENOTRESET)
			{
				break;
			}
		}

		//Reset the device using the saved parameters
		if(FAILED(m_pD3DDevice->Reset(&m_struPresentParameters)))
		{
			hr = D3DERR_DEVICELOST;
		}
	}	

	return hr;
}

HRESULT CCUDAD3DDisplay::DestroyDevice(void)
{
	//Release the D3D Device
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	return S_OK;
}

HRESULT CCUDAD3DDisplay::CreateGeometry(void)
{
	int iControlWidth, iControlHeight;
	//float fDisplayWidth, fDisplayHeight;
	//float fAdaptedTextWHRate;
	HRESULT hr = S_OK;
	RECT rcDisplaySize;

	// Check Texture
// 	if(m_pImageTexture == NULL)
// 	{
// 		return E_FAIL;
// 	}

	// Release Old VertexBuffer
	if(FAILED(DestroyGeometry()))
	{
		return E_FAIL;
	}

	// Init Vertex
	::GetWindowRect(m_hDisplayWnd, &rcDisplaySize);
	iControlWidth = rcDisplaySize.right - rcDisplaySize.left;
	iControlHeight = rcDisplaySize.bottom - rcDisplaySize.top;

	m_iMeshWidth = iControlWidth/2;
	m_iMeshHeight = iControlHeight/2;

// 	fAdaptedTextWHRate = (float)m_struTextInfo.iTextWidth / (float)m_struTextInfo.iTextHeight;
// 	if(iControlWidth <= iControlHeight)
// 	{
// 		fDisplayWidth = (float)iControlWidth;
// 		fDisplayHeight = fDisplayWidth / fAdaptedTextWHRate;
// 	}
// 	else
// 	{
// 		fDisplayHeight = (float)iControlHeight;
// 		fDisplayWidth = fAdaptedTextWHRate * fDisplayHeight;
// 	}
// 	fDisplayWidth *= m_fDisplayZoom;
// 	fDisplayHeight *= m_fDisplayZoom;

// 	StruCustomVertex struVInfo[4] = 
// 	{
// 		{-fDisplayWidth/2.0f, -fDisplayHeight/2.0f, 0.0f, 0.0f, 0.0f},
// 		{fDisplayWidth/2.0f, -fDisplayHeight/2.0f, 0.0f, 1.0f, 0.0f},
// 		{-fDisplayWidth/2.0f, fDisplayHeight/2.0f, 0.0f, 0.0f, 1.0f},
// 		{fDisplayWidth/2.0f, fDisplayHeight/2.0f, 0.0f, 1.0f, 1.0f}
// 	};

	m_iNumVertices = m_iMeshWidth *m_iMeshHeight;
	//m_iNumVertices = 128*128;

	hr |= m_pD3DDevice->CreateVertexBuffer(/*sizeof(struVInfo)/sizeof(struVInfo[0])*/m_iNumVertices*sizeof(StruCustomVertex), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, \
		&m_pVertexBuffer, NULL);

// 	void *lpVBuffer;
// 	if(FAILED(m_pVertexBuffer->Lock(0, sizeof(struVInfo)/sizeof(struVInfo[0])*sizeof(StruCustomVertex), (void **)&lpVBuffer, 0)))
// 	{
// 		DestroyGeometry();
// 
// 		return E_FAIL;
// 	}
// 
// 	memcpy(lpVBuffer, (void *)struVInfo, sizeof(struVInfo)/sizeof(struVInfo[0])*sizeof(StruCustomVertex));
// 
// 	hr |= m_pVertexBuffer->Unlock();

	//Since we are not going to use another source...
	//hr |= m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//hr |= m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(StruCustomVertex));

	return hr;
}

HRESULT CCUDAD3DDisplay::DestroyGeometry(void)
{
	if(m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	return S_OK;
}

HRESULT CCUDAD3DDisplay::CreateCustomTextures(int iTextWidth, int iTextHeight, D3DFORMAT StruTextFormat, int iPixelBytesNum)
{
	int iAdaptedTextWidth = iTextWidth, iAdaptedTextHeight = iTextHeight;
	HRESULT hr = S_OK;

	if(iTextWidth <= 0 && iTextHeight <= 0 && iPixelBytesNum <= 0)
	{
		AfxMessageBox(_T("Create Textures Failed!"));		

		return E_FAIL;
	}

	if(!TextureSizeAdapt(iTextWidth, iTextHeight, &iAdaptedTextWidth, &iAdaptedTextHeight))
	{
		return E_FAIL;
	}

	if(FAILED(DestroyTextureObject()))
	{
		return E_FAIL;
	}

	m_struTextInfo.iTextWidth = iAdaptedTextWidth;
	m_struTextInfo.iTextHeight = iAdaptedTextHeight;
	m_struTextInfo.StruTextFormat = StruTextFormat;
	m_struTextInfo.iPixelBytesNum = iPixelBytesNum;

	hr |= m_pD3DDevice->CreateTexture(m_struTextInfo.iTextWidth, \
		m_struTextInfo.iTextHeight, \
		0, \
		0, \
		m_struTextInfo.StruTextFormat, \
		D3DPOOL_MANAGED, \
		&m_pImageTexture, \
		NULL);

	return hr;
}

HRESULT CCUDAD3DDisplay::LoadTextureData(UCHAR *lpDataBuffer, int iTextWidth, int iTextHeight)
{
	int i, j;
	int iter_i, iter_j;
	int iTextBufferStartX, iTextBufferStartY;
	int iDataBufferStartX, iDataBufferStartY;
	int iDataBufferEndX, iDataBufferEndY;
	UCHAR ucPixelValue;
	HRESULT hr = S_OK;
	D3DLOCKED_RECT rcWorkRect;
	RECT rcLockRect = {0, 0, m_struTextInfo.iTextWidth, m_struTextInfo.iTextHeight};

	// Init Pos
	if(m_struTextInfo.iTextWidth >= iTextWidth)
	{
		iTextBufferStartX = (m_struTextInfo.iTextWidth-iTextWidth) / 2;
		iDataBufferStartX = 0;
		iDataBufferEndX = iDataBufferStartX + iTextWidth;
	}
	else
	{
		iTextBufferStartX = 0;
		iDataBufferStartX = (iTextWidth-m_struTextInfo.iTextWidth) / 2;
		iDataBufferEndX = iDataBufferStartX + m_struTextInfo.iTextWidth;
	}

	if(m_struTextInfo.iTextHeight >= iTextHeight)
	{
		iTextBufferStartY = (m_struTextInfo.iTextHeight-iTextHeight) / 2;
		iDataBufferStartY = 0;
		iDataBufferEndY = iDataBufferStartY + iTextHeight;
	}
	else
	{
		iTextBufferStartY = 0;
		iDataBufferStartY = (iTextHeight-m_struTextInfo.iTextHeight) / 2;
		iDataBufferEndY = iDataBufferStartY + m_struTextInfo.iTextHeight;
	}

	hr |= m_pImageTexture->LockRect(0, &rcWorkRect, &rcLockRect, D3DLOCK_DISCARD);

	memset((BYTE *)rcWorkRect.pBits, 0, rcWorkRect.Pitch*m_struTextInfo.iTextHeight);
	for(j=iDataBufferEndY-1, iter_j=iTextBufferStartY; j>=iDataBufferStartY; j--, iter_j++)
	{
		for(i=iDataBufferStartX, iter_i=iTextBufferStartX; i<iDataBufferEndX; i++, iter_i++)
		{
			ucPixelValue = lpDataBuffer[j*iTextWidth+i];
			*(DWORD *)((BYTE *)rcWorkRect.pBits + iter_j*rcWorkRect.Pitch + m_struTextInfo.iPixelBytesNum*iter_i) = D3DCOLOR_XRGB(ucPixelValue, ucPixelValue, ucPixelValue);
		}
	}

	hr |= m_pImageTexture->UnlockRect(0);

	return hr;
}

HRESULT CCUDAD3DDisplay::DestroyTextureObject(void)
{
	if(m_pImageTexture != NULL)
	{
		m_pD3DDevice->SetTexture(0, NULL);
		m_pImageTexture->Release();
		m_pImageTexture = NULL;
	}

	return S_OK;
}

HRESULT CCUDAD3DDisplay::DestroyD3D(void)
{
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	return S_OK;
}

bool CCUDAD3DDisplay::TextureSizeAdapt(int iOrignalTextWidth, int iOrignalTextHeight, int *lpAdaptedTextWidth, int *lpAdaptedTextHeight)
{
	if(iOrignalTextWidth <= 0 || iOrignalTextHeight <= 0 || lpAdaptedTextWidth == NULL || lpAdaptedTextHeight == NULL)
	{
		return false;
	}

	if (iOrignalTextWidth > 64)//宽度校正
	{
		if (iOrignalTextWidth <= 128)
		{
			*lpAdaptedTextWidth = 128;
		}
		else if (iOrignalTextWidth <= 256) 
		{
			*lpAdaptedTextWidth = 256;
		}
		else if (iOrignalTextWidth <= 512)
		{
			*lpAdaptedTextWidth = 512;
		}
		else if (iOrignalTextWidth <= 1024)
		{
			*lpAdaptedTextWidth = 1024;
		}
		else
		{
			*lpAdaptedTextWidth = 2048;
		}
	}
	else
	{
		if (iOrignalTextWidth > 32)
		{
			*lpAdaptedTextWidth = 64;
		}
		else if (iOrignalTextWidth > 16)
		{
			*lpAdaptedTextWidth = 32;
		}
		else if (iOrignalTextWidth > 8)
		{
			*lpAdaptedTextWidth = 16;
		}
		else if (iOrignalTextWidth > 4)
		{
			*lpAdaptedTextWidth = 8;
		}
		else if (iOrignalTextWidth > 2)
		{
			*lpAdaptedTextWidth = 4;
		}
		else
		{
			*lpAdaptedTextWidth = 2;
		}
	}

	if (iOrignalTextHeight > 64)//高度校正
	{
		if (iOrignalTextHeight <= 128)
		{
			*lpAdaptedTextHeight = 128;
		}
		else if (iOrignalTextHeight <= 256)
		{
			*lpAdaptedTextHeight = 256;
		}
		else if (iOrignalTextHeight <= 512)
		{
			*lpAdaptedTextHeight = 512;
		}
		else if (iOrignalTextHeight <= 1024)
		{
			*lpAdaptedTextHeight = 1024;
		}
		else
		{
			*lpAdaptedTextHeight = 2048;
		}
	}
	else
	{
		if (iOrignalTextHeight > 32)
		{
			*lpAdaptedTextHeight = 64;
		}
		else if (iOrignalTextHeight > 16)
		{
			*lpAdaptedTextHeight = 32;
		}
		else if (iOrignalTextHeight > 8)
		{
			*lpAdaptedTextHeight = 16;
		}
		else if (iOrignalTextHeight > 4)
		{
			*lpAdaptedTextHeight = 8;
		}
		else if (iOrignalTextHeight > 2)
		{
			*lpAdaptedTextHeight = 4;
		}
		else
		{
			*lpAdaptedTextHeight = 2;
		}
	}

	return true;
}