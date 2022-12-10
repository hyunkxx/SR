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

	// CreateThread : 과거 c표준 함수들을 설계할 당시에 쓰레드에 대해서는 전혀 고려하지 않았다.
	// CreateThread함수로 생성된 쓰레드들은 c 표준 함수를 쓰레드 내부에서 사용할 경우 문제가 발생해서 현재는
	// 더 안전한 _beginthreadex 함수 사용을 권장한다.

#pragma region 예시

	/*int	iData = 10;

	unsigned int		dwThreadID[3]{};
	HANDLE				hThread[3]{};

	hThread[i] = (HANDLE)_beginthreadex(nullptr,		 // 보안 속성 : 기본값(핸들의 상속 여부, nullptr인 경우 상속에서 제외)
		0,			 // 디폴트 스택 사이즈(1바이트)
		Thread_Main, // 쓰레드로 동작할 함수의 주소(__stdcall 규약)
		&iData,		 // 쓰레드 함수로 전달할 함수의 매개 변수
		0,			 // 쓰레드의 생성 및 실행을 조절하기 위한 flag
		&dwThreadID[i]);    // 쓰레드 id반환*/

#pragma endregion 예시

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
