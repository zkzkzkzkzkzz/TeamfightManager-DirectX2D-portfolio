#pragma once

#include "CTexture.h"

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);

private:
	ComPtr<ID3D11Device>			m_Device;		// GPU 메모리 관리, 객체 생성
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU 렌더링 명령 

	ComPtr<IDXGISwapChain>			m_SwapChain;	// 스왚체인(출력 버퍼 지정)

	// OM(OutputMergeState)
	Ptr<CTexture>					m_RTTex;
	Ptr<CTexture>					m_DSTex;
	//ComPtr<ID3D11Texture2D>			m_DSTex;		// 뎊스 스텐실 텍스쳐	
	//ComPtr<ID3D11DepthStencilView>	m_DSView;		// 뎊스 스텐실 뷰

	HWND							m_hRenderWnd;
	Vec2							m_vRenderResolution;
	
	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];

	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void ClearRenderTarget(float(&Color)[4]);
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11RasterizerState>	GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState>	GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>		GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }

private:
	int CreateSwapChain();
	int CreateTargetView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();
};

