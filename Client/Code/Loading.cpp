#include "stdafx.h"
#include "..\Header\Loading.h"

#include "Export_Function.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_hThread(nullptr)
	, m_eID(LOADING_END)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szString, sizeof(_tchar) * 128);
}


CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	// CreateThread : ���� cǥ�� �Լ����� ������ ��ÿ� �����忡 ���ؼ��� ���� ������� �ʾҴ�.
	// CreateThread�Լ��� ������ ��������� c ǥ�� �Լ��� ������ ���ο��� ����� ��� ������ �߻��ؼ� �����
	// �� ������ _beginthreadex �Լ� ����� �����Ѵ�.

#pragma region ����

	/*int	iData = 10;

	unsigned int		dwThreadID[3]{};
	HANDLE				hThread[3]{};

	hThread[i] = (HANDLE)_beginthreadex(nullptr,		 // ���� �Ӽ� : �⺻��(�ڵ��� ��� ����, nullptr�� ��� ��ӿ��� ����)
		0,			 // ����Ʈ ���� ������(1����Ʈ)
		Thread_Main, // ������� ������ �Լ��� �ּ�(__stdcall �Ծ�)
		&iData,		 // ������ �Լ��� ������ �Լ��� �Ű� ����
		0,			 // �������� ���� �� ������ �����ϱ� ���� flag
		&dwThreadID[i]);    // ������ id��ȯ*/

#pragma endregion ����

	m_hThread = (HANDLE)_beginthreadex(nullptr,	0, Thread_Main,	this, 0, nullptr);   

	m_eID = eID;


	return S_OK;
}

unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}


_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szString, L"Loading Component");

	// ui
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Back_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Front_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_Back_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Chat_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Log_Back_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Posin_UI_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_BlueTeam_Kill_Back_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	Engine::Ready_Sound();


	lstrcpy(m_szString, L"Loading Texture");

	//loading all mesh 
	Engine::Initalize();


	// component
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Sand.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Location", Engine::CLocation::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_CubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/desert.dds", TEX_CUBE)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	// ui
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/last_Hp_Back.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Last_Hp_Mid.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Front_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/last_Hp_Front.png", TEX_NORMAL)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/last_Hp_Back.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/last_WorldHpF.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Steam_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Steam.png", TEX_NORMAL)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Chat_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Chatting_Board1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Log_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Chatting_Board1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Posin_UI_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Posin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Bubble_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Emoticon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_OccupationColor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/OccupationColor_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Volume_Set.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex0", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex0", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_BlueTeam_Kill_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Background.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_BlueTeam_Kill_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Ui_BlueScoreBlank.png", TEX_NORMAL)), E_FAIL);

	lstrcpy(m_szString, L"Complete");


	m_bFinish = true;

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
