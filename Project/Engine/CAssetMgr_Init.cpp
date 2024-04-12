#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"


void CAssetMgr::init()
{
	InitSound();

	CreateDefaultMesh();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();

	InitContent();
}

void CAssetMgr::CreateDefaultMesh()
{
	CMesh* pMesh = nullptr;

	Vtx vPoint;
	UINT Idx = 0;
	pMesh = new CMesh(true);
	pMesh->Create(&vPoint, 1, &Idx, 1);\
	AddAsset(L"PointMesh", pMesh);


	Vtx arrVtx[4] = {};

	// 전역변수에 삼각형 위치 설정
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// 인덱스
	UINT arrIdx[6] = {};

	// 오른쪽 삼각형
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	// 왼쪽 삼각형
	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", pMesh);

	//=================
	// Topology LineStrip 용도
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;
	arrIdx[3] = 3;
	arrIdx[4] = 0;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh);

	//=================
	// CircleMesh 생성
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSlice = 50;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh);

	//=================
	// CircleMesh_Debug 생성
	vecIdx.clear();

	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	//=================
	// CrossMesh 생성
	v.vPos = Vec3(0.f, 0.5f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.f, -0.5f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); // 세로줄
	vecIdx.push_back(2); vecIdx.push_back(3); // 가로줄

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CrossMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	// 셰이더 생성
	Ptr<CGraphicsShader> pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	// Parameter
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "INT_0");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Texture 0");
	pShader->AddTexParam(TEX_PARAM::TEX_1, "Texture 1");
	pShader->AddTexParam(TEX_PARAM::TEX_2, "Texture 2");
	pShader->AddTexParam(TEX_PARAM::TEX_3, "Texture 3");
	pShader->AddTexParam(TEX_PARAM::TEX_4, "Texture 4");
	pShader->AddTexParam(TEX_PARAM::TEX_5, "Texture 5");

	AddAsset(L"Std2DShader", pShader.Get());


	//======================
	// 이펙트셰이더 생성
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"EffectShader", pShader.Get());


	//======================
	// 타일맵 셰이더 생성
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader.Get());


	//======================
	// 파티클 렌더 셰이더 생성
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트 진행은 하지만, 기록하지는 않는다
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"ParticleRenderShader", pShader.Get());


	//======================
	// GrayFilter Shader 생성
	// Mesh		:	RectMesh
	// RS_TYPE	:	CULL_BACK
	// DS_TYPE	:	NO_TEST_NO_WRITE
	// BS_TYPE	:	DEFAULT
	// Domain	:	DOMAIN_POSTPROCESS
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"GrayFilterShader", pShader.Get());


	//======================
	// Distortion Shader 생성
	// RS_TYPE	:	CULL_NONE
	// DS_TYPE	:	NO_TEST_NO_WRITE
	// BS_TYPE	:	DEFAULT
	// Domain	:	DOMAIN_POSTPROCESS
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"DistortionShader", pShader.Get());


	//======================
	// VCR Distortion Shader 생성
	// RS_TYPE	:	CULL_NONE
	// DS_TYPE	:	NO_TEST_NO_WRITE
	// BS_TYPE	:	DEFAULT
	// Domain	:	DOMAIN_POSTPROCESS
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_VCRDistortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_VCRDistortion");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"VCRDistortionShader", pShader.Get());


	//======================
	// Outline 생성
	// RS_TYPE	:	CULL_NONE
	// DS_TYPE	:	NO_TEST_NO_WRITE
	// BS_TYPE	:	DEFAULT
	// Domain	:	DOMAIN_POSTPROCESS
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Outline");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Outline");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"OutlineShader", pShader.Get());

	//======================
	// 디버그쉐이프 셰이더 생성
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader.Get());
}

void CAssetMgr::CreateDefaultMaterial()
{
	// 머테리얼 생성
	CMaterial* pMtrl = nullptr;
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"Std2DMtrl", pMtrl);


	// === 공용 오브젝트 ==========================================
	CreateDefaultGameMaterial();

	// === 타이틀 레벨 ============================================
	CreateTitleLevelMaterial();

	// === 로비 레벨 =============================================
	CreateLobbyLevelMaterial();

	// === 선수 머테리얼 =============================================
	CreateInGameObjectMaterial();


	// ============================================================

	// 타일맵 머테리얼 생성
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"TileMapShader"));
	AddAsset<CMaterial>(L"TileMapMtrl", pMtrl);

	// 파티클 머테리얼 생성
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"ParticleRenderShader"));
	AddAsset<CMaterial>(L"ParticleMtrl", pMtrl);
	

	// ============================================================


	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"GrayFilterShader"));
	AddAsset<CMaterial>(L"GrayFilterMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DistortionShader"));
	AddAsset<CMaterial>(L"DistortionMtrl", pMtrl);

	// VCRDistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"VCRDistortionShader"));
	AddAsset<CMaterial>(L"VCRDistortionMtrl", pMtrl);

	// OutlineMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"OutlineShader"));
	AddAsset<CMaterial>(L"OutlineMtrl", pMtrl);


	// ============================================================


	// 디버그쉐이프 머테리얼 생성
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(L"DebugShapeMtrl", pMtrl);
}

void CAssetMgr::CreateDefaultGameMaterial()
{
	CMaterial* pMtrl = nullptr;

	// 커서 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"CursorMtrl", pMtrl);

	// 텍스트 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"TextMtrl", pMtrl);

	// 이펙트 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"EffectMtrl", pMtrl);

	// 그림자 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"ShadowMtrl", pMtrl);

	// 알파블렌드 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"EffectShader"));
	AddAsset<CMaterial>(L"AlphaMtrl", pMtrl);	
}

void CAssetMgr::CreateTitleLevelMaterial()
{
	CMaterial* pMtrl = nullptr;

	// 타이틀배경 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"BackgroundMtrl", pMtrl);

	// 스타디움 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"StadiumMtrl", pMtrl);

	// 하늘 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"SkyMtrl", pMtrl);

	// 게임 타이틀 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"TitleMtrl", pMtrl);

	// 새 게임 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"NewGameMtrl", pMtrl);

	// 종료하기 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"EndGameMtrl", pMtrl);

	// 왼칼 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"LeftSwordMtrl", pMtrl);

	// 오른칼 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"RightSwordMtrl", pMtrl);
}

void CAssetMgr::CreateLobbyLevelMaterial()
{
	CMaterial* pMtrl = nullptr;

	// 로비 하우스 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"HouseMtrl", pMtrl);

	// 로비(하늘) 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"LobbySkyMtrl", pMtrl);	
	
	// 로비(땅) 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"LobbyGroundMtrl", pMtrl);

	// 헤더 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"HeaderMtrl", pMtrl);

	// 헤더 슬롯 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"HeaderSlotMtrl", pMtrl);

	// 달력 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"CalenderMtrl", pMtrl);

	// 코인 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"CoinMtrl", pMtrl);

	// 로고 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"LogoMtrl", pMtrl);

	// 아이콘 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"IconMtrl", pMtrl);

	// 텍스트UI 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"TextUIMtrl", pMtrl);

	// 로비 버튼 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"LobbyBtnMtrl", pMtrl);

	// 일반 버튼 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"BtnMtrl", pMtrl);

	// 진행 버튼 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"ProceedBtnMtrl", pMtrl);

	// 영입 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"RecruitmentMtrl", pMtrl);
}

void CAssetMgr::CreateInGameObjectMaterial()
{
	CMaterial* pMtrl = nullptr;

	// 선수 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"GamerMtrl", pMtrl);

	// 챔피언 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"ChampMtrl", pMtrl);

	// 탄막 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"ProjectileMtrl", pMtrl);

	// 인게임UI 머테리얼
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
	AddAsset<CMaterial>(L"InGameUIMtrl", pMtrl);
}



#include "CSetColorShader.h"
#include "CParticleUpdate.h"

void CAssetMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pShader = nullptr;

	// SetColorShader
	pShader = new CSetColorShader;
	AddAsset(L"SetColorShader", pShader.Get());

	// ParticleUpdateShader
	pShader = new CParticleUpdate;
	AddAsset(L"ParticleUpdateShader", pShader.Get());
}

#include "CSound.h"

void CAssetMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CAssetMgr::InitContent()
{
	// content 폴더에 있는 모든 에셋 파일명(상대경로)을 찾아낸다
	wstring strContentPath = CPathMgr::GetContentPath();
	FindFileName(strContentPath);

	// 찾은 파일 이름으로 에셋들을 로드
	for (size_t i = 0; i < m_vecAssetFileName.size(); ++i)
	{
		ASSET_TYPE Type = GetAssetTypeByExt(m_vecAssetFileName[i]);

		switch (Type)
		{
		case ASSET_TYPE::END:
		default:
			continue;
			break;
		case ASSET_TYPE::MESH:
			CAssetMgr::GetInst()->Load<CMesh>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::MESHDATA:
			//CAssetMgr::GetInst()->Load<CMeshData>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::PREFAB:
			CAssetMgr::GetInst()->Load<CPrefab>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::TEXTURE:
			CAssetMgr::GetInst()->Load<CTexture>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::MATERIAL:
			CAssetMgr::GetInst()->Load<CMaterial>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::SOUND:
			CAssetMgr::GetInst()->Load<CSound>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		case ASSET_TYPE::FSM:
			CAssetMgr::GetInst()->Load<CFSM>(m_vecAssetFileName[i], m_vecAssetFileName[i]);
			break;
		}
	}
}

void CAssetMgr::FindFileName(const wstring& _Directory)
{// 파일 탐색 결과 저장
	WIN32_FIND_DATA FIND_DATA = {};

	// 탐색을 시도할 디렉터리 경로 + 찾을 파일 타입 포맷(모든 파일, 폴더, 모든 확장자 탐색)
	wstring strDirectory = _Directory + L"*.*";

	// 탐색 핸들 생성
	HANDLE hFindHandle = FindFirstFile(strDirectory.c_str(), &FIND_DATA);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// 탐색 핸들을 이용해서, 파일을 다음 파일을 탐색, 더이상 없으면 false 반환
	while (FindNextFile(hFindHandle, &FIND_DATA))
	{
		// 찾은 파일이 Directory 타입인지 확인
		if (FIND_DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// .. 이름의 폴더는 상위폴더로 가는 기능
			if (!wcscmp(L"..", FIND_DATA.cFileName))
				continue;

			// 재귀함수로 하위 폴더내에 있는 파일이름을 탐색
			FindFileName(_Directory + FIND_DATA.cFileName + L"\\");
		}
		else
		{
			// 파일 타입인 경우, 디렉터리까지 붙여서 최종 경로를 만들고, 상대경로만 추출해서 m_vecAssetFileName 에 취합
			wstring strRelativePath = CPathMgr::GetRelativePath(_Directory + FIND_DATA.cFileName);
			m_vecAssetFileName.push_back(strRelativePath);
		}
	}

	FindClose(hFindHandle);
}

ASSET_TYPE CAssetMgr::GetAssetTypeByExt(const path& _relativePath)
{
	if (_relativePath.extension() == L".mesh")
		return ASSET_TYPE::MESH;
	if (_relativePath.extension() == L".mtrl")
		return ASSET_TYPE::MATERIAL;
	if (_relativePath.extension() == L".mdat")
		return ASSET_TYPE::MESHDATA;
	if (_relativePath.extension() == L".prefab")
		return ASSET_TYPE::PREFAB;

	if (_relativePath.extension() == L".png"
		|| _relativePath.extension() == L".bmp"
		|| _relativePath.extension() == L".jpg"
		|| _relativePath.extension() == L".jpeg"
		|| _relativePath.extension() == L".dds"
		|| _relativePath.extension() == L".tga")
		return ASSET_TYPE::TEXTURE;

	if (_relativePath.extension() == L".wav"
		|| _relativePath.extension() == L".mp3"
		|| _relativePath.extension() == L".ogg")
		return ASSET_TYPE::SOUND;

	return ASSET_TYPE::END;
}
