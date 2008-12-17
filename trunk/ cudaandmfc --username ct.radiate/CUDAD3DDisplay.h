#pragma once

#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )

#pragma warning(disable:4819)

//#include "PgmSlice.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
//#include "CUDAUtility.h"
#include "CUDAUtility.h"

#define EPSILON 1e-6f

typedef struct _tagStruCustomTexVertex
{
	float fX, fY, fZ;
	float texfU, texfV;
} StruCustomTexVertex, *LPStruCustomTexVertex;

typedef struct _tagStruCustomVertex
{
	float fX, fY, fZ;
	DWORD color;
} StruCustomVertex, *LPStruCustomVertex;

typedef struct _tagStruTextureInfo
{
	int iTextWidth;
	int iTextHeight;
	D3DFORMAT StruTextFormat;
	int iPixelBytesNum;
} StruTextureInfo, *LPStruTextureInfo;

#define D3DFVF_CUSTOMTEXVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)


class CCUDAD3DDisplay
{
public:
	CCUDAD3DDisplay(void);
	/*virtual*/ ~CCUDAD3DDisplay(void);

	bool InitCUDAD3DDisplay(HWND hDisplayDevice);
	bool DisplayTexture(/*CPgmSlice *lpTexture,*/ D3DFORMAT StruTextFormat, int iPixelBytesNum);
	bool DisplayVertex(float fTime);
	bool Cleanup(void);
	bool SetDisplayZoom(float fDisplayZoom);

private:
	CCUDAD3DDisplay& operator=(const CCUDAD3DDisplay &cddRhs);
	//D3D Element
	bool PreInit(void);
	bool PreTerminate(void);
	HRESULT PreRender(void);
	bool PreReset(void);
	bool InitD3D(void);
	HRESULT Render(void);
	bool PostInit(void);
	bool PostTerminate(void);
	HRESULT PostRender(void);
	bool PostReset(void);
	bool HandleMessage(MSG *pMessage);

	void SetupMatrices(void);
	
	//Function Modal
	
	void VerifyModes(void);
	HRESULT SetWindowedDevice(void);
	HRESULT CreateDevice(void);
	HRESULT SetupDevice(void);
	HRESULT RestoreDevice(void);
	HRESULT DestroyDevice(void);
	HRESULT CreateGeometry(void);
	HRESULT DestroyGeometry(void);
	HRESULT CreateCustomTextures(int iTextWidth, int iTextHeight, D3DFORMAT StruTextFormat, int iPixelBytesNum);
	HRESULT LoadTextureData(UCHAR *lpDataBuffer, int iTextWidth, int iTextHeight);
	HRESULT DestroyTextureObject(void);
	HRESULT DestroyD3D(void);
	//Basic Operation
	bool TextureSizeAdapt(int iOriginalTextWidth, int iOriginalTextHeight, int *lpAdaptedTextWidth, int *lpAdaptedTextHeight);
	HWND m_hDisplayWnd;
	float m_fDisplayZoom;
	//CPgmSlice *m_lpResetDataPointer;
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pImageTexture;
	StruTextureInfo m_struTextInfo;
	D3DXMATRIX m_struWorldMatrix;
	D3DXMATRIX m_struViewMatrix;
	D3DXMATRIX m_struProjMatrix;
	D3DCAPS9 m_struHALCaps;
	D3DDISPLAYMODE m_struCurrentMode;
	D3DPRESENT_PARAMETERS m_struPresentParameters;
	D3DDEVICE_CREATION_PARAMETERS m_struCreationParameters;

	int m_iMeshWidth;
	int m_iMeshHeight;
	int m_iNumVertices;

	CCUDACLASS m_ccclass;

};
