#include "stdafx.h"
#include "..\Header\Stage.h"
#include "Export_Function.h"

//
#include "Tank_01.h"
#include "Humvee.h"
#include "BackGround.h"
#include "Terrain.h"
#include"Default_Enermy.h"
#include"Default_Ally.h"
#include"BottomDirEnermy.h"
#include"BottomDirAlly.h"
#include "Building.h"
#include "BattleShip.h"
#include "ShipBullet.h"

//camera
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "DroneCamera.h"
#include "BoomCamera.h"
#include "ShipCamera.h"
// ai
#include"RightLocation.h"
#include"RightTopLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"CreateAi.h"

// ui
#include "Posin_UI.h"
#include "Player_Chatting.h"
#include "UI_Log_Back.h"
#include "UI_FontMgr.h"
#include "UI_Player_Hp.h"
#include "UI_World_Bubble.h"
#include "UI_Volume.h"
#include "UI_Compass.h"
#include "UI_Start.h"
#include "UI_MiniMap.h"
#include "UI_Speed.h"
//#include "UI_Fuel.h"
#include "Aim_UI.h"
#include "Aim_UI_Pers.h"
#include"TempOccupationScore.h"

// effect skill
#include "ShootEffect.h"
#include "BoomEffect.h"
#include "Gun_Shoot_Effect.h"
#include "Bomber.h"
#include "Boom_Support.h"
#include "BattleShip_Support.h"
#include "EffectPool.h"
#include "EffectManager.h"
#include "ShootSmoke.h"

/* Button */
#include "ButtonUI.h"
#include "DirButton.h"
#include "AICreateButton.h"

#include "BaseUI.h"
#include "ResultUI.h"

/* System */
#include "TankManager.h"
#include "GameMode.h"
#include "ModeSelectMenu.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	Engine::StopAll();
	ShowCursor(false);

	float Start = 10.f;
	float End = 300.f;
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(255, 240, 210, 0));
	//m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(255, 255, 255, 0));

	m_pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	m_pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	m_pGraphicDev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	m_pGraphicDev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	CGameMode::GetInstance()->InitGameMode(500, 100, 2000);

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
	Engine::PlaySound_SR(L"WinterBGM.mp3", STAGE_SOUND, CUI_Volume::s_fBGMSound);

	Engine::Update_BulletMgr(fTimeDelta);
	Engine::Update_CSP_EffectMgr(fTimeDelta);
	Engine::Update_EnermyMgr(fTimeDelta);
	Engine::Update_CameraMgr(fTimeDelta);
	_int iExit = __super::Update_Scene(fTimeDelta);

	CUI_FontMgr::GetInstance()->Update(fTimeDelta);

	if (CGameMode::GetInstance()->m_bGameEnd
		&& Get_DIKeyState_Custom(DIK_RETURN) == KEY_STATE::TAP)
	{
		/* ?? �־ȵǴµ� */
		Engine::StopSound(STAGE_SOUND);
		CScene*		pScene = nullptr;

		Engine::StopSound(SELECT_SOUND);
		Engine::PlaySound_SR(L"enter.mp3", SELECT_SOUND, 1.f);

		pScene = CModeSelectMenu::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pScene, -1);
		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
	}

	if (CGameMode::GetInstance()->m_bOnTrigger)
	{
		if (CGameMode::GetInstance()->UseMenu())
		{
			CCameraMgr::GetInstance()->Get_Camera()->Set_MouseFix(true);
		}
		else
		{
			CCameraMgr::GetInstance()->Get_Camera()->Set_MouseFix(false);
		}

		CGameMode::GetInstance()->m_bOnTrigger = false;
	}

	return iExit;
}

void CStage::LateUpdate_Scene(void)
{
	Collison_Object();
	Engine::LateUpdate_BulletMgr();
	Engine::LateUpdate_CSP_EffectMgr();
	Engine::LateUpdate_EnermyMgr();
	Engine::LateUpdate_CameraMgr();
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{
	if (CGameMode::GetInstance()->m_bGameEnd)
		return;

	changeColor += 0.1f;

	if (changeColor >= 1.f)
	{
		changeColor = 0.f;
	}

	// _DEBUG�� ���
	CGameObject* pHelpWin = Engine::Get_Object(L"UI", L"Start_UI");
	_bool showF1Win = static_cast<CUI_Start*>(pHelpWin)->Get_HelpWin();

	// ��ũ ���� or �̸�								27.45% ����, 28.63% ��� �� 39.22%
	if ((!showF1Win) && Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Tank_Name(), &_vec2(PERCENTX * 12.f, WINCY_HALF + PERCENTY * 35.f), D3DXCOLOR(changeColor, 1.f - changeColor, changeColor, 1.f));
	}

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA || Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{
		// �������� ���� �ð�
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_MIn(), &_vec2(WINCX_HALF - PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_Colon(), &_vec2(WINCX_HALF, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_TenSec(), &_vec2(WINCX_HALF + PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());
		Render_Font(L"Font_Retro", CUI_FontMgr::GetInstance()->Get_Time_OneSec(), &_vec2(WINCX_HALF + PERCENTX * 4, PERCENTY), CUI_FontMgr::GetInstance()->Get_Black());

		// �� ų ī��Ʈ								
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_BlueTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY), CUI_FontMgr::GetInstance()->Get_Hecks_B());
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_RedTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY * 7.f), CUI_FontMgr::GetInstance()->Get_Hecks_R());
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
	CGameMode::DestroyInstance();

	StopAll();

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

	pCameraObject = CShipCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"ShipCamera", pCameraObject), E_FAIL);

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

	for (_int i = 0; 200 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SMALL_CANNONBALL);
	}

	for (_int i = 0; 200 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::MIDDLE_CANNONBALL);
	}

	for (_int i = 0; 200 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::BIG_CANNONBALL);
	}

	for (_int i = 0; 10 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SHIP_BULLET);
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
	for (_int i = 0; 10 > i; i++)
	{
		CGameObject* pBullet = CShipBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		static_cast<CShipBullet*>(pBullet)->Set_ID(BULLET_ID::SHIP_REAL_BULLET);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SHIP_REAL_BULLET);
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
	CUI_FontMgr::GetInstance()->Set_AllCount(iEnermy_Count + iBottomDir_Count - 2);
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

	pGameObject = CCreateAi::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CreateAi", pGameObject), E_FAIL);


	for (_int i = 0; 10 > i; i++)
	{
		pGameObject = CShootEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Effect_Supply(pGameObject, EFFECT_ID::TANK_SHOOT_SMOKE);
	}
	for (_int i = 0; 10 > i; i++)
	{
		pGameObject = CBoomEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Effect_Supply(pGameObject, EFFECT_ID::BOOMER_BOOM_SMOKE);
	}
	for (_int i = 0; 10 > i; i++)
	{
		pGameObject = CGun_Shoot_Effect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Effect_Supply(pGameObject, EFFECT_ID::HUMVEE_SHOOT_EFFECT);
	}
	for (_int i = 0; 10 > i; i++)
	{
		pGameObject = CShootSmoke::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Effect_Supply(pGameObject, EFFECT_ID::SHIP_SMOKE_EFFECT);
	}

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CUI_Player_Hp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Hp2", pGameObject), E_FAIL);

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

	pGameObject = CUI_Speed::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Speed_UI", pGameObject), E_FAIL);

	pGameObject = CUI_MiniMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniMap_UI", pGameObject), E_FAIL);

	//pGameObject = CUI_Fuel::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fuel_UI", pGameObject), E_FAIL);



	/* Select Vehicle UI */
	_vec3 vPos;
	ZeroMemory(&vPos, sizeof(_vec3));

	vPos = { 100.f, 0.f, 0.f };
	pGameObject = m_pButton[0] = CButtonUI::Create(m_pGraphicDev, VEHICLE::HUMVEE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_00", pGameObject), E_FAIL);
	static_cast<CButtonUI*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 250.f, 0.f, 0.f };
	pGameObject = m_pButton[1] = CButtonUI::Create(m_pGraphicDev, VEHICLE::SMALL_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_01", pGameObject), E_FAIL);
	static_cast<CButtonUI*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 400.f, 0.f, 0.f };
	pGameObject = m_pButton[2] = CButtonUI::Create(m_pGraphicDev, VEHICLE::MIDDLE_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_02", pGameObject), E_FAIL);
	static_cast<CButtonUI*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 550.f, 0.f, 0.f };
	pGameObject = m_pButton[3] = CButtonUI::Create(m_pGraphicDev, VEHICLE::BIG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_03", pGameObject), E_FAIL);
	static_cast<CButtonUI*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 700.f, 0.f, 0.f };
	pGameObject = m_pButton[4] = CButtonUI::Create(m_pGraphicDev, VEHICLE::LONG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_04", pGameObject), E_FAIL);
	static_cast<CButtonUI*>(pGameObject)->Set_PosX(vPos.x);

	/* Create AI UI */
	ZeroMemory(&vPos, sizeof(_vec3));

	vPos = { 100.f, 0.f, 0.f };
	pGameObject = m_pButton[0] = CAICreateButton::Create(m_pGraphicDev, VEHICLE::HUMVEE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ai_btn_00", pGameObject), E_FAIL);
	static_cast<CAICreateButton*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 250.f, 0.f, 0.f };
	pGameObject = m_pButton[1] = CAICreateButton::Create(m_pGraphicDev, VEHICLE::SMALL_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ai_btn_01", pGameObject), E_FAIL);
	static_cast<CAICreateButton*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 400.f, 0.f, 0.f };
	pGameObject = m_pButton[2] = CAICreateButton::Create(m_pGraphicDev, VEHICLE::MIDDLE_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ai_btn_02", pGameObject), E_FAIL);
	static_cast<CAICreateButton*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 550.f, 0.f, 0.f };
	pGameObject = m_pButton[3] = CAICreateButton::Create(m_pGraphicDev, VEHICLE::BIG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ai_btn_03", pGameObject), E_FAIL);
	static_cast<CAICreateButton*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 700.f, 0.f, 0.f };
	pGameObject = m_pButton[4] = CAICreateButton::Create(m_pGraphicDev, VEHICLE::LONG_TANK);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ai_btn_04", pGameObject), E_FAIL);
	static_cast<CAICreateButton*>(pGameObject)->Set_PosX(vPos.x);

	/* Dir Button */
	vPos = { 330.f, 0.f, 0.f };
	pGameObject = m_pLeftButton = CDirButton::Create(m_pGraphicDev, CDirButton::DIR::LEFT);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_left", pGameObject), E_FAIL);
	static_cast<CDirButton*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 470.f, 0.f, 0.f };
	pGameObject = m_pRightButton = CDirButton::Create(m_pGraphicDev, CDirButton::DIR::RIGHT);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"btn_right", pGameObject), E_FAIL);
	static_cast<CDirButton*>(pGameObject)->Set_PosX(vPos.x);
	m_umapLayer.insert({ pLayerTag, pLayer });

	vPos = { 200.f, 0.f, 0.f };
	pGameObject = m_pAllyHP = CBaseUI::Create(m_pGraphicDev, CBaseUI::TYPE::ALLY);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ally_hp", pGameObject), E_FAIL);
	static_cast<CBaseUI*>(pGameObject)->Set_PosX(vPos.x);

	vPos = { 600.f, 0.f, 0.f };
	pGameObject = m_pEnemyHP = CBaseUI::Create(m_pGraphicDev, CBaseUI::TYPE::ENEMY);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"enemy_hp", pGameObject), E_FAIL);
	static_cast<CBaseUI*>(pGameObject)->Set_PosX(vPos.x);

	pGameObject = CResultUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"game_result", pGameObject), E_FAIL);

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
	CGameObject* TempCreateAi = pGameLogic->Get_GameObject(L"CreateAi");
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
				CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"�λ��"), wstring(L"���� �ε������ϴ�!"));
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

	//����) �Ѿ˰� �浹ó�� �ϰ� �����ֵ� �̷��� �߰��ϸ� ��
	for (int i = 0; BULLET_ID::MASHINE_BULLET_RELOAD > i; i++)
	{
		for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
		{
			if ((*iter)->Get_Dead())
				continue;

			CBullet* pBullet = dynamic_cast<CBullet*>(*iter);
			if (pBullet)
			{
				_vec3 vPos = pBullet->Get_Info();

				if (vPos.y <= 0.5f)
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::GROUND, vPos);
			}

			// �Ѿ� vs ȯ�� ������Ʈ �浹
			for (auto& Dest = pEnvironment_Object->Get_mapObject()->begin(); pEnvironment_Object->Get_mapObject()->end() != Dest; Dest++)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(Dest->second))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), (*iter)->Get_Dist(), Dest->second->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
				{
					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;

					if (i == BULLET_ID::MASHINE_BULLET)
					{
						static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::BULLET, vPos);

						const CBuilding::TYPE& eType = static_cast<CBuilding*>(Dest->second)->GetType();

						switch (eType)
						{
						case CBuilding::TYPE::BASE_ALLY:
							CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ALLY] -= 100.f;
							break;
						case CBuilding::TYPE::BASE_ENEMY:
							CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ENEMY] -= 100.f;
							break;
						default:
							break;
						}
					}
					else if (i == BULLET_ID::CANNONBALL || i == BULLET_ID::SMALL_CANNONBALL || i == BULLET_ID::MIDDLE_CANNONBALL || i == BULLET_ID::BIG_CANNONBALL)
					{
						static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::EXPLOSION, vPos);

						const CBuilding::TYPE& eType = static_cast<CBuilding*>(Dest->second)->GetType();

						switch (eType)
						{
						case CBuilding::TYPE::BASE_ALLY:
							CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ALLY] -= 500.f;
							break;
						case CBuilding::TYPE::BASE_ENEMY:
							CGameMode::GetInstance()->m_fBaseCurHP[(UINT)CGameMode::TYPE::ENEMY] -= 500.f;
							break;
						default:
							break;
						}
					}

					(*iter)->Set_Dead(true);
					continue;
				}

			}
			//�Ѿ� vs ���� ������Ʈ �浹(Collisionable�� ������ �ִ°� ���� �ۿ� ��� �ܼ� ���� �ϳ��� �Ѿ˰��� �浹ó����� ���� ��.)
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

					if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::CANNONBALL)
						dynamic_cast<CTankSet*>((*Dest).second)->Minus_HP(30.f);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CTankSet*>((*Dest).second)->Minus_HP(10.f);

					//Engine::PlaySound_SR(L"SE_Tank_S_STOP_00.wav", HIT_SOUND, CUI_Volume::s_fShotSound);			ã�� ���� ��
					if (dynamic_cast<CTankSet*>((*Dest).second)->Get_HP() <= 0)
						(*Dest).second->Set_Dead(true);

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

					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::SMALL_CANNONBALL)
						dynamic_cast<CDefault_Enermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MIDDLE_CANNONBALL)
						dynamic_cast<CDefault_Enermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::BIG_CANNONBALL)
						dynamic_cast<CDefault_Enermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CDefault_Enermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE).fDamage);

					//Engine::PlaySound_SR(L"SE_Tank_S_STOP_00.wav", HIT_SOUND, CUI_Volume::s_fShotSound);			ã�� ���� ��
					if (dynamic_cast<CDefault_Enermy*>(*iters)->GetHp() <= 0)
					{
						(*iters)->Set_Dead(true);
						dynamic_cast<CCreateAi*>(TempCreateAi)->Set_FieldCount(1);//������ �����ȱ�
						dynamic_cast<CDefault_Enermy*>(*iters)->Set_DisCountLocation();
					}
					continue;
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

					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::SMALL_CANNONBALL)
						dynamic_cast<CBottomDirEnermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MIDDLE_CANNONBALL)
						dynamic_cast<CBottomDirEnermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::BIG_CANNONBALL)
						dynamic_cast<CBottomDirEnermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CBottomDirEnermy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE).fDamage);

					//Engine::PlaySound_SR(L"SE_Tank_S_STOP_00.wav", HIT_SOUND, CUI_Volume::s_fShotSound);			ã�� ���� ��
					if (dynamic_cast<CBottomDirEnermy*>(*iters)->GetHp() <= 0)
					{
						(*iters)->Set_Dead(true);
						dynamic_cast<CBottomDirEnermy*>(*iters)->Set_DisCountLocation();
						dynamic_cast<CCreateAi*>(TempCreateAi)->Set_FieldCount(1);//������ �����ȱ�
					}
					continue;
				}
			}
			for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
			{

				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
				{
					(*iter)->Set_Dead(true);

					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::SMALL_CANNONBALL)
						dynamic_cast<CDefault_Ally*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MIDDLE_CANNONBALL)
						dynamic_cast<CDefault_Ally*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::BIG_CANNONBALL)
						dynamic_cast<CDefault_Ally*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CDefault_Ally*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE).fDamage);

					//Engine::PlaySound_SR(L"SE_Tank_S_STOP_00.wav", HIT_SOUND, CUI_Volume::s_fShotSound);			ã�� ���� ��
					if (dynamic_cast<CDefault_Ally*>(*iters)->GetHp() <= 0)
					{
						(*iters)->Set_Dead(true);
						dynamic_cast<CDefault_Ally*>(*iters)->Set_DisCountLocation();
					}
					continue;
				}
			}
			for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
			{

				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(*iters))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*iter)->Get_Dist(), (*iters)->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
				{
					(*iter)->Set_Dead(true);

					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::CANNONBALL)
						dynamic_cast<CBottomDirAlly*>(*iters)->Minus_HP_UI(30.f);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::SMALL_CANNONBALL)
						dynamic_cast<CBottomDirAlly*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MIDDLE_CANNONBALL)
						dynamic_cast<CBottomDirAlly*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::BIG_CANNONBALL)
						dynamic_cast<CBottomDirAlly*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CBottomDirAlly*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE).fDamage);

					if (dynamic_cast<CBottomDirAlly*>(*iters)->GetHp() <= 0)
					{
						(*iters)->Set_Dead(true);
						dynamic_cast<CBottomDirAlly*>(*iters)->Set_DisCountLocation();
					}
					continue;
				}
			}
		}
	}

	CGameObject* pPlayer = pGameLogic->Get_GameObject(L"PlayerVehicle");

	// ������ AI �� �浹ó��
	for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
	{
		if (!dynamic_cast<ICollisionable*>(pPlayer) || !dynamic_cast<ICollisionable*>(*iters))
			continue;

		if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), pPlayer->Get_Dist(), (*iters)->Get_Dist()))
			continue;

		if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
		{
			OBB* A = dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB();
			OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();

			dynamic_cast<CTankSet*>(pPlayer)->OBB_Collision_EX();


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
		if (!dynamic_cast<ICollisionable*>(pPlayer) || !dynamic_cast<ICollisionable*>(*iters))
			continue;

		if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), pPlayer->Get_Dist(), (*iters)->Get_Dist()))
			continue;

		if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
		{
			OBB* A = dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB();
			OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();

			dynamic_cast<CTankSet*>(pPlayer)->OBB_Collision_EX();

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
		if (!dynamic_cast<ICollisionable*>(pPlayer) || !dynamic_cast<ICollisionable*>(*iters))
			continue;

		if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), pPlayer->Get_Dist(), (*iters)->Get_Dist()))
			continue;

		if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
		{
			OBB* A = dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB();
			OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();

			dynamic_cast<CTankSet*>(pPlayer)->OBB_Collision_EX();

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

		if (!dynamic_cast<ICollisionable*>(pPlayer) || !dynamic_cast<ICollisionable*>(*iters))
			continue;

		if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), pPlayer->Get_Dist(), (*iters)->Get_Dist()))
			continue;

		if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
		{
			OBB* A = dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB();
			OBB* B = dynamic_cast<ICollisionable*>(*iters)->Get_OBB();

			dynamic_cast<CTankSet*>(pPlayer)->OBB_Collision_EX();

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

	for (auto& Dest = DAlly.begin(); Dest < DAlly.end(); ++Dest)
	{
		auto iter = Dest;
		iter++;
		for (; iter < DAlly.end(); ++iter)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iter))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iter)->Get_Info(), (*Dest)->Get_Dist(), (*iter)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iter)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();

				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CDefault_Ally*>(*iter)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CDefault_Ally*>(*iter)->ObjectCol(false);
				}
			}
		}

		for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

	for (auto& Dest = BDAlly.begin(); Dest < BDAlly.end(); ++Dest)
	{
		auto iter = Dest;
		iter++;
		for (; iter < BDAlly.end(); ++iter)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iter))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iter)->Get_Info(), (*Dest)->Get_Dist(), (*iter)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iter)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();

				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CBottomDirAlly*>(*iter)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CBottomDirAlly*>(*iter)->ObjectCol(false);
				}
			}
		}

		for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		for (auto& iters = BDEnemy.begin(); iters < BDEnemy.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

	for (auto& Dest = BDEnemy.begin(); Dest < BDEnemy.end(); ++Dest)
	{
		auto iter = Dest;
		iter++;
		for (; iter < BDEnemy.end(); ++iter)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iter))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iter)->Get_Info(), (*Dest)->Get_Dist(), (*iter)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iter)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();

				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CBottomDirEnermy*>(*iter)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CBottomDirEnermy*>(*iter)->ObjectCol(false);
				}
			}
		}


		for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		for (auto& iters = DEnemy.begin(); iters < DEnemy.end(); ++iters)
		{

			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

	for (auto& Dest = DEnemy.begin(); Dest < DEnemy.end(); ++Dest)
	{
		auto iter = Dest;
		iter++;

		for (; iter < DEnemy.end(); ++iter)
		{

			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iter))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iter)->Get_Info(), (*Dest)->Get_Dist(), (*iter)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iter)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
				OBB* B = dynamic_cast<ICollisionable*>(*iter)->Get_OBB();

				if (A->vPos.x > B->vPos.x)
				{
					dynamic_cast<CDefault_Enermy*>(*iter)->ObjectCol(true);
				}
				else
				{
					dynamic_cast<CDefault_Enermy*>(*iter)->ObjectCol(false);
				}
			}

		}

		for (auto& iters = BDAlly.begin(); iters < BDAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		for (auto& iters = DAlly.begin(); iters < DAlly.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		for (auto& iters = BDEnemy.begin(); iters < BDEnemy.end(); ++iters)
		{
			if (!dynamic_cast<ICollisionable*>((*Dest)) || !dynamic_cast<ICollisionable*>(*iters))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_Info(), dynamic_cast<ICollisionable*>(*iters)->Get_Info(), (*Dest)->Get_Dist(), (*iters)->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>((*Dest))->Get_OBB(), dynamic_cast<ICollisionable*>(*iters)->Get_OBB()))
			{
				OBB* A = dynamic_cast<ICollisionable*>((*Dest))->Get_OBB();
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

		
	}
}


HRESULT CStage::CreateMap(CLayer* pLayer)
{
	CGameObject*		pGameObject = nullptr;

	_vec3 vPos;
	float fRot;

	vPos = { 315.f, 0.f,  315.f };

	pGameObject = CBuilding::Create(m_pGraphicDev, L"Oasis", vPos, CBuilding::TYPE::OASIS);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CBuilding*>(pGameObject)->SetRotation(0.f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(
		static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);

	pGameObject = m_pBase_Ally = CBuilding::Create(m_pGraphicDev, L"Base_ally", vPos, CBuilding::TYPE::BASE_ALLY);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CBuilding*>(pGameObject)->SetRotation(D3DXToRadian(110.f));
	vPos = { 20.f, 7.f, 20.f };
	static_cast<CBuilding*>(pGameObject)->SetPosition(vPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(
		static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);


	pGameObject = m_pBase_Enemy = CBuilding::Create(m_pGraphicDev, L"Base_enemy", vPos, CBuilding::TYPE::BASE_ENEMY);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CBuilding*>(pGameObject)->SetRotation(D3DXToRadian(20.f));
	vPos = { 610.f, 7.f, 610.f };
	static_cast<CBuilding*>(pGameObject)->SetPosition(vPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(
		static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);

	for (int j = 0; j < 9; ++j)
	{
		for (int i = -1; i < 8; ++i)
		{
			int ObjectNumber = rand() % (UINT)CBuilding::TYPE::TYPE_MAX;

			vPos = { float((rand() % 80) + (100 * i)) , 0.f , (float(rand() % 80) + (100 * j)) };
			fRot = (float)(rand() % 180);

			if ((vPos.x < 150.f && vPos.z < 150.f) || (vPos.x > 430.f && vPos.z > 430.f))
				continue;

			switch ((CBuilding::TYPE)ObjectNumber)
			{
			case CBuilding::TYPE::BUILDING_1:
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Building_000_object", vPos, CBuilding::TYPE::BUILDING_1);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
			case CBuilding::TYPE::BUILDING_2:
				pGameObject = CBuilding::Create(m_pGraphicDev, L"Building_001_object", vPos, CBuilding::TYPE::BUILDING_2);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				static_cast<CBuilding*>(pGameObject)->SetRotation(fRot);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(
					static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
				break;
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
				break;
			}
		}
	}

	return S_OK;
}