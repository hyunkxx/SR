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

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

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

	Engine::Ready_Sound();


	lstrcpy(m_szString, L"Loading Texture");

	//loading all mesh 
	Engine::Initalize();



	// Rush
#pragma region
	// UI
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Rush_HP_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Rush_Life.png", TEX_NORMAL)), E_FAIL);




#pragma endregion


	// component

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainSandTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Sand.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TerrainGrassTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_001.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Location", Engine::CLocation::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_CubeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/desert.dds", TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_See", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/See.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	// ui
	// HP
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/player_HP2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Hp_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Player_HP_Bar.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/WHP.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Hp_Tex_2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/WHP_2.png", TEX_NORMAL)), E_FAIL);
	// HP->Reload
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Reload_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Reload_Bar%d.png", TEX_NORMAL, 2)), E_FAIL);

	// chatting 
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Steam_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Steam.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Player_Chat_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Chatting_Board1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Log_Back_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Chatting_Board1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_World_Bubble_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Emoticon.png", TEX_NORMAL)), E_FAIL);

	//option
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Volume_Set.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex0", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex0", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Down_Tex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Down1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Volume_Up_Tex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Botton_Up1.png", TEX_NORMAL)), E_FAIL);

	// F1
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Start_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/shutter door1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Slider_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Fuel3.png", TEX_NORMAL)), E_FAIL);

	// Aim camera UI
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Posin_UI_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Posin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Compass_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Compass.png", TEX_NORMAL)), E_FAIL);

	// Stage UI
	// 점령
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_OccupationColor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/OccupationColor_%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Aside", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Aside.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Bside", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Bside.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ScoreBoard", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/ScoreBoard.png", TEX_NORMAL)), E_FAIL);
	// 미니맵
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_MiniMap_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Mini_map2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Minimap_P_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/OccupationColor_2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Minimap_A_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Player_HP_Bar.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Minimap_E_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/OccupationColor_1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Minimap_Rader_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Rader_10.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_MiniMap_A_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/occA%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_MiniMap_B_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/occB%d.png", TEX_NORMAL, 3)), E_FAIL);
	// 연료계
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Fuel_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/fuel2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Fuel_Bar_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Fuel3.png", TEX_NORMAL)), E_FAIL);
	// 속도계
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_Speed_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Speed1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_UI_SpeedBar_Tex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/UI_SpeedBar1.png", TEX_NORMAL)), E_FAIL);

	/* Select Button */
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"UIBackground", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Black.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_non", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_non.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"humvee_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/humvee_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"humvee_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/humvee_btn_clicked.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"small_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/small_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"small_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/small_btn_clicked.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"middle_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/middle_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"middle_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/middle_btn_clicked.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"big_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/big_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"big_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/big_btn_clicked.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"long_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/long_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"long_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/long_btn_clicked.png", TEX_NORMAL)), E_FAIL);

	/* AI button */

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_btn_non", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/button.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_humvee_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_humvee_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_humvee_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_humvee_btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_small_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_small_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_small_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_small_btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_middle_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_middle_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_middle_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_middle_btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_big_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_big_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_big_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_big_btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_long_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_long_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_long_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_long_btn_click.png", TEX_NORMAL)), E_FAIL);

	/* Dir BUtton */

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_left_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_left_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_left_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_left_btn_click.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_right_btn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_right_btn.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"ai_right_btn_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/ai_right_btn_click.png", TEX_NORMAL)), E_FAIL);

	/* BaseUI */
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"base_back", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BaseUI/BaseBackground.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"base_ally_front", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BaseUI/BaseAllyFront.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"base_enemy_front", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/BaseUI/BaseEnemyFront.png", TEX_NORMAL)), E_FAIL);

	/* Button */
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_war", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_war.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_war_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_war_click.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_rush", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_rush.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_rush_click", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_rush_click.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_start", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_start.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"btn_exit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Button/btn_exit.png", TEX_NORMAL)), E_FAIL);

	/* Result UI */
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"game_result", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/game_result.png", TEX_NORMAL)), E_FAIL);

	/* Grass */
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Grass", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Grass2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass2.png", TEX_NORMAL)), E_FAIL);


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
