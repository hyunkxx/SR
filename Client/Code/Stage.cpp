#include "stdafx.h"
#include "..\Header\Stage.h"
#include "Export_Function.h"

#include "Tank_01.h"
#include "Humvee.h"

#include "BackGround.h"
#include "Terrain.h"

#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "DroneCamera.h"
#include "BoomCamera.h"

#include "Boom_Support.h"
#include "BattleShip_Support.h"

#include"Default_Enermy.h"
#include "TestBox.h"
#include"Default_Ally.h"
#include"BottomDirEnermy.h"
#include"BottomDirAlly.h"

#include"RightLocation.h"
#include"RightTopLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"

#include "Posin_UI.h"
#include "Player_Chatting.h"
#include "UI_Log_Back.h"
#include "UI_FontMgr.h"
#include "UI_Player_Hp_Back.h"
#include "UI_Player_Hp.h"
#include "UI_Player_Hp_Front.h"
#include "UI_World_Bubble.h"
#include "UI_Volume.h"
#include "UI_Compass.h"
#include "UI_Start.h"
#include "Aim_UI.h"
#include "Aim_UI_Pers.h"
#include "ShootEffect.h"
#include "BoomEffect.h"
#include "Gun_Shoot_Effect.h"
#include "Bomber.h"
#include "BattleShip.h"

#include "ButtonUI.h"

#include "EffectPool.h"
#include "EffectManager.h"
#include"TempOccupationScore.h"
#include "Building.h"
#include "TankManager.h"
#include "GameMode.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	float Start = 80.f;
	float End = 300.f;
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(255, 230, 210, 0));

	m_pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	m_pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	m_pGraphicDev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	m_pGraphicDev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	CGameMode::GetInstance()->InitGameMode(500, 20000, 800);

	Engine::StopSound(SELECT_MENU_BGM);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment_Object(L"Environment_Object"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	CUI_FontMgr::GetInstance()->Update(fTimeDelta);
	Key_Input(fTimeDelta);

	//CGameObject* pVolume = Engine::Get_Object(L"UI", L"Volume_UI");
	//if (static_cast<CUI_Volume*>(pVolume)->Get_Volume_Show())
	//{	ShowCursor(true);}
	//else
	//{	ShowCursor(false);}

	Engine::PlaySound_SR(L"coh_ingame2.mp3", STAGE_SOUND, CUI_Volume::s_fBGMSound);

	Engine::Update_BulletMgr(fTimeDelta);
	Engine::Update_EnermyMgr(fTimeDelta);
	Engine::Update_CameraMgr(fTimeDelta);
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CStage::LateUpdate_Scene(void)
{
	Collison_Object();
	Engine::LateUpdate_BulletMgr();
	Engine::LateUpdate_EnermyMgr();
	Engine::LateUpdate_CameraMgr();
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{
	// _DEBUGï¿½ï¿½ ï¿½ï¿½ï¿½
	CGameObject* pHelpWin = Engine::Get_Object(L"UI", L"Start_UI");
	_bool showF1Win = static_cast<CUI_Start*>(pHelpWin)->Get_HelpWin();


	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA || Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_MIn(), &_vec2(WINCX_HALF - PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_Colon(), &_vec2(WINCX_HALF, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_TenSec(), &_vec2(WINCX_HALF + PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_OneSec(), &_vec2(WINCX_HALF + PERCENTX * 4, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());

		// ï¿½ï¿½ Å³ Ä«ï¿½ï¿½Æ®								
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_BlueTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY), CUI_FontMgr::GetInstance()->Get_Hecks_B());
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_RedTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY * 7.f), CUI_FontMgr::GetInstance()->Get_Hecks_R());

		// ï¿½ï¿½Å© ï¿½ï¿½ï¿½ï¿½ or ï¿½Ì¸ï¿½								27.45% ï¿½ï¿½ï¿½ï¿½, 28.63% ï¿½ï¿½ï¿½ ï¿½ï¿½ 39.22%
		if (!showF1Win)
		{
			Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Tank_Name(), &_vec2(PERCENTX, WINCY_HALF + PERCENTY * 20.f), D3DXCOLOR(0.2745f, 0.2863f, .3922f, 1.f));
		}
	}
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*		pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CStage::Free(void)
{
	CUI_FontMgr::GetInstance()->DestroyInstance();
	Engine::StopAll();
	__super::Free();
}

HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	
	// CTerrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	//Location

	pGameObject = CLeftLocation::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LeftLocation", pGameObject), E_FAIL);

	pGameObject = CLeftTopLocation::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LeftTopLocation", pGameObject), E_FAIL);

	pGameObject = CRightTopLocation::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RightTopLocation", pGameObject), E_FAIL);

	pGameObject = CRightLocation::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RightLocation", pGameObject), E_FAIL);

	// skybox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);



	CCamera* pCameraObject = CTankCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 15.f, -5.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"TankCamera", pCameraObject), E_FAIL);
	Engine::Camera_Change(L"TankCamera");

	pCameraObject = CStaticCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 20.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"StaticCamera", pCameraObject), E_FAIL);

	pCameraObject = CAimCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 20.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"AimCamera", pCameraObject), E_FAIL);

	pCameraObject = CDroneCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"DroneCamera", pCameraObject), E_FAIL);

	pCameraObject = CBoomCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"BoomCamera", pCameraObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environment_Object(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	_vec3 vPos, vRot;
	CreateMap(pLayer);

	pGameObject = CTestBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestBox", pGameObject), E_FAIL);

	pGameObject = m_pEffectManager = CEffectManager::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectManager", pGameObject), E_FAIL);


	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = *CTankManager::GetInstance()->GetVehicle();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerVehicle", pGameObject), E_FAIL);

	pGameObject = CBomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bomber", pGameObject), E_FAIL);

	pGameObject = CBattleShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BattleShip", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);
	for (_int i = 0; 200 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::CANNONBALL);
	}

	for (_int i = 0; 1000 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::MASHINE_BULLET);
	}

	for (_int i = 0; 1000 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::MASHINE_BULLET_RELOAD);
	}
	for (_int i = 0; 200 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::CANNONBALL_RELOAD);
	}

	for (_int i = 0; 5 > i; i++)
	{
		CGameObject* pBullet = CSmoke_Bullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SMOKE_BULLET);
	}

	for (_int i = 0; 5 > i; i++)
	{
		CGameObject* pBullet = CBoom_Bullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::BOOM_BULLET);
	}
	// Skill
	pGameObject = CBoom_Support::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boom_Support", pGameObject), E_FAIL);

	pGameObject = CBattleShip_Support::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BattleShip_Support", pGameObject), E_FAIL);


	//bottomdirenermy
	_int iBottomDir_Count = 5;

	for (_int i = 0; iBottomDir_Count> i; i++)
	{
		_int tanktype = rand() % 4;
		switch (tanktype)
		{
		case 0:
			m_eData.TankType = TANKTYPE::HUMVEE;
			break;
		case 1:
			m_eData.TankType = TANKTYPE::SMALL_TANK;
			break;
		case 2:
			m_eData.TankType = TANKTYPE::MIDDLE_TANK;
			break;
		case 3:
			m_eData.TankType = TANKTYPE::BIG_TANK;
			break;
		case 4:
			m_eData.TankType = TANKTYPE::LONG_TANK;
			break;
		default:
			return NULL;
			break;
		}
		m_eData.eID = OBJID::OBJID_BDENERMY;
		m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 130 - 20.f);
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 110 - 40.f);

		CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
	}

	//Default_Enermy
	_int iEnermy_Count = 5;
	CUI_FontMgr::GetInstance()->Set_AllCount(iEnermy_Count  + iBottomDir_Count - 2);
	for (_int i = 0; iEnermy_Count > i; i++)
	{
		_int tanktype = rand() % 4;
		switch (tanktype)
		{
		case 0:
			m_eData.TankType = TANKTYPE::HUMVEE;
			break;
		case 1:
			m_eData.TankType = TANKTYPE::SMALL_TANK;
			break;
		case 2:
			m_eData.TankType = TANKTYPE::MIDDLE_TANK;
			break;
		case 3:
			m_eData.TankType = TANKTYPE::BIG_TANK;
			break;
		case 4:
			m_eData.TankType = TANKTYPE::LONG_TANK;
			break;
		default:
			return NULL;
			break;
		}
		m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
		m_eData.vPos.x = (float)(VTXITV*VTXCNTX - rand() % 110 - 40.f);
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)(VTXITV*VTXCNTZ - rand() % 120 - 30.f);

		CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
	}
	//Ally
	for (_int i = 0; 5> i; i++)
	{
		_int tanktype = rand() % 4;
		switch (tanktype)
		{
		case 0:
			m_eData.TankType = TANKTYPE::HUMVEE;
			m_eData.vPos.x = (float)(rand() % 30 + 120.f);
			m_eData.vPos.y = 0;
			break;
		case 1:
			m_eData.TankType = TANKTYPE::SMALL_TANK;
			m_eData.vPos.x = (float)(rand() % 30 + 90.f);
			m_eData.vPos.y = 0;
			break;
		case 2:
			m_eData.TankType = TANKTYPE::MIDDLE_TANK;
			m_eData.vPos.x = (float)(rand() % 30 + 60.f);
			m_eData.vPos.y = 0;

			break;
		case 3:
			m_eData.TankType = TANKTYPE::BIG_TANK;
			m_eData.vPos.x = (float)(rand() % 30 + 30.f);
			m_eData.vPos.y = 0;

			break;
		default:
			return NULL;
			break;
		}
		m_eData.eID = OBJID::OBJID_DEFAULT_ALLY;
		m_eData.vPos.z = (float)(rand() % 150);
		CGameObject* pEnermy = CDefault_Ally::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ALLY);
	}
	for (_int i = 0; 5 > i; i++)
	{
		_int tanktype = rand() % 4;
		switch (tanktype)
		{
		case 0:
			m_eData.TankType = TANKTYPE::HUMVEE;

			m_eData.vPos.y = 0;
			m_eData.vPos.z = (float)(rand() % 30 + 120.f);
			break;
		case 1:
			m_eData.TankType = TANKTYPE::SMALL_TANK;
			m_eData.vPos.y = 0;
			m_eData.vPos.z = (float)(rand() % 30 + 90.f);
			break;
		case 2:
			m_eData.TankType = TANKTYPE::MIDDLE_TANK;
			m_eData.vPos.y = 0;
			m_eData.vPos.z = (float)(rand() % 30 + 60.f);
			break;
		case 3:
			m_eData.TankType = TANKTYPE::BIG_TANK;

			m_eData.vPos.y = 0;
			m_eData.vPos.z = (float)(rand() % 30 + 30.f);
			break;

		default:
			return NULL;
			break;
		}
		m_eData.eID = OBJID::OBJID_BDALLY;
		m_eData.vPos.x = (float)(rand() % 150);

		CGameObject* pEnermy = CBottomDirAlly::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDALLY);
	}


	pGameObject = CShootEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootEffect", pGameObject), E_FAIL);

	pGameObject = CBoomEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BoomEffect", pGameObject), E_FAIL);

	pGameObject = CGun_Shoot_Effect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gun_ShootEffect", pGameObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CUI_Player_Hp::Create(m_pGraphicDev, 450.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Hp2", pGameObject), E_FAIL);

pGameObject = CUI_Player_Hp_Front::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Hp1", pGameObject), E_FAIL);

	pGameObject = CUI_Player_Hp_Back::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Hp3", pGameObject), E_FAIL);

	pGameObject = CTempOccupationScore::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Occuopation", pGameObject), E_FAIL);

	pGameObject = CPlayer_Chatting::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Chatting", pGameObject), E_FAIL);

	pGameObject = CUI_Log_Back::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Log_Back", pGameObject), E_FAIL);

	pGameObject = CPosin_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Posin_UI", pGameObject), E_FAIL);

	pGameObject = CUI_World_Bubble::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"World_Bubble", pGameObject), E_FAIL);

	pGameObject = CUI_Volume::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Volume_UI", pGameObject), E_FAIL);

	pGameObject = CUI_Compass::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Compass_UI", pGameObject), E_FAIL);

	pGameObject = CUI_Start::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Start_UI", pGameObject), E_FAIL);

	pGameObject = CAim_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI", pGameObject), E_FAIL);

	pGameObject = CAim_UI_Pers::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI_Pers", pGameObject), E_FAIL);

	/* Select Vehicle UI */
	_vec3 vPos;
	ZeroMemory(&vPos, sizeof(_vec3));

	vPos = { 0.f, 0.f, 0.f };
	pGameObject = m_pButton[0] = CButtonUI::Create(m_pGraphicDev, VEHICLE::HUMVEE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_00", pGameObject), E_FAIL);

	vPos = { 80.f, 0.f, 0.f };
	pGameObject = m_pButton[1] = CButtonUI::Create(m_pGraphicDev, VEHICLE::SMALL_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_01", pGameObject), E_FAIL);
	vPos = { 160.f, 0.f, 0.f };
	pGameObject = m_pButton[2] = CButtonUI::Create(m_pGraphicDev, VEHICLE::MIDDLE_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_02", pGameObject), E_FAIL);

	vPos = { 240.f, 0.f, 0.f };
	pGameObject = m_pButton[3] = CButtonUI::Create(m_pGraphicDev, VEHICLE::BIG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_03", pGameObject), E_FAIL);

	vPos = { 320.f, 0.f, 0.f };
	pGameObject = m_pButton[4] = CButtonUI::Create(m_pGraphicDev, VEHICLE::LONG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_04", pGameObject), E_FAIL);

	for (int i = 0 ; i < 3 ; ++i)
	{
		vPos = { 200.f * (i + 1), 0.f, 0.f };
		static_cast<CButtonUI*>(m_pButton[i])->Set_PosX(vPos.x);
	}

	vPos = { 290.f , 380.f, 0.f };
	static_cast<CButtonUI*>(m_pButton[3])->Set_PosX(vPos.x);
	static_cast<CButtonUI*>(m_pButton[3])->Set_PosY(vPos.y);

	vPos = { 500.f , 380.f, 0.f };
	static_cast<CButtonUI*>(m_pButton[4])->Set_PosX(vPos.x);
	static_cast<CButtonUI*>(m_pButton[4])->Set_PosY(vPos.y);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

void CStage::CameraChangeSetting()
{
}

void CStage::Collison_Object(void)
{
	CLayer* pEnvironment_Object = Get_Layer(L"Environment_Object");
	CLayer* pGameLogic = Get_Layer(L"GameLogic");

	vector<CGameObject*> DAlly = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ALLY);
	vector<CGameObject*> BDAlly = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDALLY);
	vector<CGameObject*> DEnemy = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	vector<CGameObject*> BDEnemy = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
	for (auto&iter = pEnvironment_Object->Get_mapObject()->begin(); pEnvironment_Object->Get_mapObject()->end() != iter; iter++)
	{
		for (auto&Dest = pGameLogic->Get_mapObject()->begin(); pGameLogic->Get_mapObject()->end() != Dest; Dest++)
		{
			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(Dest->second))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), iter->second->Get_Dist(), Dest->second->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
			{
				dynamic_cast<ICollisionable*>(Dest->second)->OBB_Collision_EX();
				CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"ï¿½Î»ï¿½ï¿½"), wstring(L"ï¿½ï¿½ï¿½ï¿½ ï¿½Îµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½!"));
			}
		}
		for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), iter->second->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>(iter->second)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CDefault_Ally*>(*iters)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CDefault_Ally*>(*iters)->ObjectCol(false);
				}
			}
		}
		for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), iter->second->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>(iter->second)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CBottomDirAlly*>(*iters)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CBottomDirAlly*>(*iters)->ObjectCol(false);
				}
			}
		}
		for (auto& iters = BDEnemy.begin(); iters < BDEnemy.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), iter->second->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>(iter->second)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CBottomDirEnermy*>(*iters)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CBottomDirEnermy*>(*iters)->ObjectCol(false);
				}
			}
		}
		for (auto& iters = DEnemy.begin(); iters < DEnemy.end(); ++iters)
		{

			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), iter->second->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>(iter->second)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CDefault_Enermy*>(*iters)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CDefault_Enermy*>(*iters)->ObjectCol(false);
				}
			}
		}
	}

	//¿¹½Ã) ÃÑ¾Ë°ú Ãæµ¹Ã³¸® ÇÏ°í ½ÍÀº¾Öµé ÀÌ·¸°Ô Ãß°¡ÇÏ¸é µÊ
	for (int i = 0; BULLET_ID::MASHINE_BULLET_RELOAD > i; i++)
	{
		for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
		{
			if ((*iter)->Get_Dead())
				continue;

			// ï¿½Ñ¾ï¿½ vs È¯ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½æµ¹
			for (auto& Dest = pEnvironment_Object->Get_mapObject()->begin(); pEnvironment_Object->Get_mapObject()->end() != Dest; Dest++)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(Dest->second))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), (*iter)->Get_Dist(), Dest->second->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
				{
					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::EXPLOSION, vPos);

					(*iter)->Set_Dead(true);
					continue;
				}

			}

			//ï¿½Ñ¾ï¿½ vs ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½æµ¹
			for (auto& Dest = pGameLogic->Get_mapObject()->begin(); pGameLogic->Get_mapObject()->end() != Dest; Dest++)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(Dest->second))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), (*iter)->Get_Dist(), Dest->second->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
				{
					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					(*iter)->Set_Dead(true);
					continue;
				}
			}


			for (auto& iters = DEnemy.begin(); iters < DEnemy.end(); ++iters)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
				{
					(*iter)->Set_Dead(true);
					continue;
					//(*iters)->Set_Dead(true);

					/*dynamic_cast<CDefault_Enermy*>(*iters)->Set_DisCountLocation();
					m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
					m_eData.vPos.x = (float)VTXITV*(VTXCNTX / 2) + rand() % 200;
					m_eData.vPos.y = 0;
					m_eData.vPos.z = (float)VTXITV*(VTXCNTZ / 2) + rand() % 100;
					m_eData.TankType = TANKTYPE::HUMVEE;
					CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);*/
				}
			}
			for (auto& iters = BDEnemy.begin(); iters < BDEnemy.end(); ++iters)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
				{
					(*iter)->Set_Dead(true);
					continue;
					//(*iters)->Set_Dead(true);
					/*dynamic_cast<CBottomDirEnermy*>(*iters)->Set_DisCountLocation();
					m_eData.eID = OBJID::OBJID_BDENERMY;
					m_eData.vPos.x = (float)VTXITV*(VTXCNTX / 2) + rand() % 200;
					m_eData.vPos.y = 0;
					m_eData.vPos.z = (float)VTXITV*(VTXCNTZ / 2) + rand() % 100;

					CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
					NULL_CHECK(pEnermy);
					Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);*/
					
				}
			}
		}
	}

	for (auto& iter = DAlly.begin(); iter < DAlly.end(); ++iter)
	{
		for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{//dynamic_cast<ICollisionable*>(*iter)->OBB_Collision_EX();
			}
		}

	}


	for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
	{
		for (auto& iter = iters + 1; iter <BDAlly.end(); ++iter)
		{
			if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{//ï¿½Õ¿ï¿½ï¿½Ö´Â°ï¿½ ï¿½ï¿½ï¿½Ñ´ï¿½ï¿½ï¿½ ï¿½Ú¿ï¿½ï¿½Ö´Â°Å¸ï¿½ exï¿½Î¸ï¿½ï¿½ï¿½
				OBB* A = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<ICollisionable*>(*iters)->OBB_Collision_EX();
				}
				else
					dynamic_cast<ICollisionable*>(*iter)->OBB_Collision_EX();
			}
		}
	}

	for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
	{
		for (auto& iter = iters + 1; iter <DAlly.end(); ++iter)
		{
			if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{//ï¿½Õ¿ï¿½ï¿½Ö´Â°ï¿½ ï¿½ï¿½ï¿½Ñ´ï¿½ï¿½ï¿½ ï¿½Ú¿ï¿½ï¿½Ö´Â°Å¸ï¿½ exï¿½Î¸ï¿½ï¿½ï¿½
				OBB* A = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();
				if (A->vPos.z > B->vPos.z)
				{
					dynamic_cast<ICollisionable*>(*iters)->OBB_Collision_EX();
				}
				else
					dynamic_cast<ICollisionable*>(*iter)->OBB_Collision_EX();
			}
		}
	}
}

void CStage::Key_Input(const _float& fTimeDelta)
{
	// Test HP UI ï¿½Ñ¤Ñ¤Ñ¤Ñ¤Ñ¤ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
#pragma region
	CGameObject* pPlayerHp_UI = Engine::Get_Object(L"UI", L"Player_Hp2");

	if (Get_DIKeyState_Custom(DIK_LEFT) == KEY_STATE::HOLD)
	{
		static_cast<CUI_Player_Hp*>(pPlayerHp_UI)->Minus_HP_UI(3.f);
	}
	if (Get_DIKeyState_Custom(DIK_RIGHT) == KEY_STATE::HOLD)
	{
		static_cast<CUI_Player_Hp*>(pPlayerHp_UI)->Plus_HP_UI(3.f);
	}

#pragma endregion

}


HRESULT CStage::CreateMap(CLayer* pLayer)
{
	CGameObject*		pGameObject = nullptr;

	_vec3 vPos;
	float fRot;

	vPos = { 315.f, 0.f,  315.f };

	pGameObject = CBuilding::Create(m_pGraphicDev, L"Oasis", vPos, CBuilding::TYPE::BUILDING);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CBuilding*>(pGameObject)->SetRotation(0.f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(
		static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);


	for (int j = 0 ; j < 9 ; ++j)
	{
		for (int i = -1 ; i < 8 ; ++i)
		{
			int ObjectNumber = rand() % (UINT)CBuilding::TYPE::MAX + 2;

			vPos = { float((rand() % 80) + (100 * i)) , 0.f , (float(rand() % 80) + (100 * j)) };
			fRot = (float)(rand() % 180);
			
			if ((vPos.x < 150.f && vPos.z < 150.f) || (vPos.x > 430.f && vPos.z > 430.f))
				continue;

			switch ((CBuilding::TYPE)ObjectNumber)
			{
			case CBuilding::TYPE::ROCK:
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Rock_object", vPos, CBuilding::TYPE::ROCK);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			case CBuilding::TYPE::PLANT_1:
				vPos.y += 3.f;
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Plant_1", vPos, CBuilding::TYPE::PLANT_1);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			case CBuilding::TYPE::PLANT_2:
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Plant_2", vPos, CBuilding::TYPE::PLANT_2);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			case CBuilding::TYPE::PLANT_3:
				vPos.y += 3.f;
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Plant_3", vPos, CBuilding::TYPE::PLANT_3);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			default:
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Building_000_object", vPos, CBuilding::TYPE::BUILDING);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			}
		}
	}

	return S_OK;
}