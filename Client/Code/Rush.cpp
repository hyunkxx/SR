#include "stdafx.h"
#include "..\Header\Rush.h"
#include "Export_Function.h"

#include "Tank_01.h"
#include "RushTank.h"
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
#include "AH_64A.h"

//Boss
#include "Boss.h"
#include "BossHitPoint.h"
#include "Boss_Bullet.h"
#include "Boss_Bomber.h"
#include "BossCamera.h"
#include "RedCarpet.h"

//camera
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "DroneCamera.h"
#include "BoomCamera.h"
#include "ShipCamera.h"
#include "AH_64A_Camera.h"
#include "AH_64A_AimCamera.h"
#include "AH_64A_EndCamera.h"
//Enemy
#include"StdEnemy.h"
//UI
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
//#include "UI_Rush_HP.h"		(HP)목숨 - 갯수
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
#include "AH_64A_Shoot_Effect.h"

#include "BaseUI.h"
#include "Grass.h"
#include "Terrain.h"

/* System */
#include "TankManager.h"
#include "GameMode.h"

#include "BossSkill.h"
#include "WarningUI.h"
#include "RushMode.h"
#include "BossHP.h"

CRush::CRush(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CRush::~CRush()
{
}

HRESULT CRush::Ready_Scene(void)
{
	ShowCursor(false);

	float Start = 80.f;
	float End = 300.f;
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(255, 255, 255, 0));

	m_pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	m_pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	m_pGraphicDev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	m_pGraphicDev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	//CUI_FontMgr::GetInstance()->Set_Rush_LifeCount(3);
	CRushMode::GetInstance()->Initalize(4000.f, 2000.f);


	Engine::StopSound(SELECT_MENU_BGM);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment_Object(L"Environment_Object"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	return S_OK;
}

_int CRush::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	//Engine::PlaySound_SR(L"BattleBGM.mp3", STAGE_SOUND, CUI_Volume::s_fBGMSound);

	Engine::Update_BulletMgr(fTimeDelta);
	Engine::Update_CSP_EffectMgr(fTimeDelta);
	Engine::Update_EnermyMgr(fTimeDelta);
	Engine::Update_CameraMgr(fTimeDelta);

	return S_OK;
}

void CRush::LateUpdate_Scene(void)
{
	Collison_Object();
	Engine::LateUpdate_BulletMgr();
	Engine::LateUpdate_CSP_EffectMgr();
	Engine::LateUpdate_EnermyMgr();
	Engine::LateUpdate_CameraMgr();
	__super::LateUpdate_Scene();
}

void CRush::Render_Scene(void)
{
	//_tchar Introduce[64] = L"옛날꺼 복구\n넘버패드 + -";
	//Render_Font(L"Font_Retro", Introduce, &_vec2(20.f, 70.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	CUI_FontMgr::GetInstance()->RUSH_RENDER();
}

CRush * CRush::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRush*		pInstance = new CRush(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CRush::Free(void)
{
	CUI_FontMgr::GetInstance()->DestroyInstance();
	Engine::StopAll();
	__super::Free();
}

HRESULT CRush::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

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
	Engine::Get_Camera()->Set_Lock(false);

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

	pCameraObject = CBossCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"BossApearCamera", pCameraObject), E_FAIL);


	for (int i = 0; i < 300; i++)
	{
		pGameObject = CGrass::Create(m_pGraphicDev, CGrass::TYPE::GRASS1);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(
			static_cast<CGrass*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
		_vec3 vPos = { (float)(rand() % 700 - 100), (float)(rand() % 2), (float)(rand() % 700 - 100) };
		static_cast<CGrass*>(pGameObject)->SetTransform(vPos, 30.f * i);
	}

	for (int i = 0; i < 300; i++)
	{
		pGameObject = CGrass::Create(m_pGraphicDev, CGrass::TYPE::GRASS2);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(
			static_cast<CGrass*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
		_vec3 vPos = { (float)(rand() % 700 - 100), (float)(rand() % 3), (float)(rand() % 700 - 100) };
		static_cast<CGrass*>(pGameObject)->SetTransform(vPos, 30.f * i);
	}

	pCameraObject = CAH_64A_Camera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"AH_64A_Camera", pCameraObject), E_FAIL);



	pCameraObject = CAH_64A_AimCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"AH_64A_AimCamera", pCameraObject), E_FAIL);

	pCameraObject = CAH_64A_EndCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 2.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pCameraObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Camera(L"AH_64A_EndCamera", pCameraObject), E_FAIL);


	// CTerrain
	pGameObject = CTerrain::Create(m_pGraphicDev, CTerrain::TYPE::GRASS);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CRush::Ready_Layer_Environment_Object(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	pGameObject = m_pEffectManager = CEffectManager::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectManager", pGameObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CRush::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CRushTank::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerVehicle", pGameObject), E_FAIL);
	_vec3 vPos = { 150.f,0.f,0.f };
	static_cast<CRushTank*>(pGameObject)->Move_Info(vPos);

	pGameObject = CBossSkill::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossSkill", pGameObject), E_FAIL);

	pGameObject = CBomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bomber", pGameObject), E_FAIL);

	pGameObject = CBattleShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BattleShip", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CAH_64A::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AH_64A", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CBoss::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss", pGameObject), E_FAIL);

	pGameObject = CBoss_Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bomber1", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);
	
	pGameObject = CBoss_Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bomber2", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CBoss_Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bomber3", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CBoss_Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bomber4", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CBoss_Bomber::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bomber5", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	pGameObject = CRedCarpet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RedCarpet", pGameObject), E_FAIL);
	pGameObject->Set_Dead(true);

	for (_int i = 0; BOOM_BULLET > i; i++)
	{
		for (_int j = 0; j < 200; j++)
		{
			CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pBullet, E_FAIL);
			Engine::Bullet_Supply(pBullet, (BULLET_ID)i);
		}
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

	for (_int i = 0; 10 > i; i++)
	{
		CGameObject* pBullet = CBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SHIP_BULLET);
	}

	for (_int i = 0; 5 > i; i++)
	{
		CGameObject* pBullet = CSmoke_Bullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SMOKE_BULLET);
	}

	for (_int i = 0; 10 > i; i++)
	{
		CGameObject* pBullet = CBoom_Bullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::BOOM_BULLET);
	}
	for (_int i = 0; 5 > i; i++)
	{
		CGameObject* pBullet = CBoss_Bullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		Engine::Bullet_Supply(pBullet, BULLET_ID::BOSS_BULLET);
	}

	for (_int i = 0; 10 > i; i++)
	{
		CGameObject* pBullet = CShipBullet::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pBullet, E_FAIL);
		static_cast<CShipBullet*>(pBullet)->Set_ID(BULLET_ID::SHIP_REAL_BULLET);
		Engine::Bullet_Supply(pBullet, BULLET_ID::SHIP_REAL_BULLET);
	}

	//Enemy
	for (_int i = 0; 4> i; i++)
	{
		m_eData.TankType = TANKTYPE::BIG_TANK;
		m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
		m_eData.vPos.x = 140.f;
		m_eData.vPos.y = 0;
		m_eData.vPos.z = 150.f + i * 20.f;

		CGameObject* pEnermy = CStdEnemy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
	}

	for (_int i = 0; 4> i; i++)
	{
		m_eData.TankType = TANKTYPE::BIG_TANK;
		m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
		m_eData.vPos.x = 160.f;
		m_eData.vPos.y = 0;
		m_eData.vPos.z = 150.f + i * 20.f;

		CGameObject* pEnermy = CStdEnemy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
	}


	// Skill
	pGameObject = CBoom_Support::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boom_Support", pGameObject), E_FAIL);

	pGameObject = CBattleShip_Support::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BattleShip_Support", pGameObject), E_FAIL);



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

	for (_int i = 0; 30 > i; i++)
	{
		pGameObject = CAH_64A_Shoot_Effect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Effect_Supply(pGameObject, EFFECT_ID::AH_64A_EFFECT);
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

HRESULT CRush::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	//pGameObject = CUI_Rush_HP::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rush_HP", pGameObject), E_FAIL);

	pGameObject = CWarningUI::Create(m_pGraphicDev, CWarningUI::MODE::RUSH);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"warning_ui", pGameObject), E_FAIL);

	pGameObject = CAim_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI", pGameObject), E_FAIL);

	pGameObject = CAim_UI_Pers::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI_Pers", pGameObject), E_FAIL);

	pGameObject = CBossHitPoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Hit_Point", pGameObject), E_FAIL);

	pGameObject = CBossHP::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_HP", pGameObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CRush::Ready_LightInfo(void)
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

void CRush::CameraChangeSetting()
{
}

void CRush::Collison_Object(void)
{
	CLayer* pEnvironment_Object = Get_Layer(L"Environment_Object");
	CLayer* pGameLogic = Get_Layer(L"GameLogic");
	vector<CGameObject*> StdEnemy = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	
	CGameObject* pPlayer = Get_Object(L"GameLogic", L"PlayerVehicle");
	CGameObject* pBoss = Get_Object(L"GameLogic", L"Boss");

	for (_int i = 0; BULLET_ID::MASHINE_BULLET > i; i++)
	{

		for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
		{

			if ((*iter)->Get_Dead())
				continue;
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
				}
			}

		}
	}


	// 적과 유저 총알 충돌처리
	for (_int i = 5; BULLET_ID::BOOM_BULLET > i; i++)
	{
		for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
		{
			if ((*iter)->Get_Dead())
				continue;
			for (auto& iters = StdEnemy.begin(); iters < StdEnemy.end(); ++iters)
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
						dynamic_cast<CStdEnemy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::SMALL_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MIDDLE_CANNONBALL)
						dynamic_cast<CStdEnemy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::MIDDLE_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::BIG_CANNONBALL)
						dynamic_cast<CStdEnemy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::BIG_TANK).fDamage);
					else if (static_cast<CBullet*>(*iter)->Get_ID() == BULLET_ID::MASHINE_BULLET)
						dynamic_cast<CStdEnemy*>(*iters)->Minus_HP_UI(CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE).fDamage);

					if (dynamic_cast<CStdEnemy*>(*iters)->GetHp() <= 0)
					{
						dynamic_cast<CStdEnemy*>(*iters)->Set_DeadMotionPlay();
					}
					continue;
				}
			}
		}
	}




	// 보스 충돌처리 여기 몰아넣기
	if (pBoss)
	{
		// 유저 총알과 보스 충돌처리
		for (_int i = 5; BULLET_ID::BOOM_BULLET > i; i++)
		{
			for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
			{
				if ((*iter)->Get_Dead())
					continue;

				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(pBoss))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(pBoss)->Get_Info(), (*iter)->Get_Dist(), (pBoss)->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(pBoss)->Get_OBB()))
				{
					/*(*iter)->Set_Dead(true);

					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);

					if (dynamic_cast<CStdEnemy*>(pBoss)->GetHp() <= 0)
					{
						dynamic_cast<CStdEnemy*>(pBoss)->Set_DeadMotionPlay();
					}
					continue;*/
				}

			}
		}
	}
	// 유저랑 충돌 몰아 놓기 무적상태 체크 후 충돌처리
	if (!static_cast<CRushTank*>(pPlayer)->Get_God_Mode())
	{

		// 유저랑 적 총알 충돌처리
		for (_int i = 0; BULLET_ID::MASHINE_BULLET > i; i++)
		{

			for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
			{

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), (*iter)->Get_Dist(), pPlayer->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB()))
				{
					_vec3 vPos = static_cast<CBullet*>(*iter)->Get_OBB()->vPos;
					static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, vPos);
				}

			}

		}

		// 보스랑 충돌
		if (Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_Info(), dynamic_cast<ICollisionable*>(pBoss)->Get_Info(), (pPlayer)->Get_Dist(), pBoss->Get_Dist()))
		{


			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(pPlayer)->Get_OBB(), dynamic_cast<ICollisionable*>(pBoss)->Get_OBB()))
				pPlayer->Set_Dead(true);
		}
	}



}
