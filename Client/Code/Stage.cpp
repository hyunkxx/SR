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
#include "Aim_UI.h"
#include "Aim_UI_Pers.h"
#include "ShootEffect.h"
#include "Gun_Shoot_Effect.h"

#include"TempOccupationScore.h"
#include "Building.h"
#include "TankManager.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CStage::~CStage()
{
	
}

HRESULT CStage::Ready_Scene(void)
{
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

	CGameObject* pVolume = Engine::Get_Object(L"UI", L"Volume_UI");
	if (static_cast<CUI_Volume*>(pVolume)->Get_Volume_Show())
	{	ShowCursor(true);}
	else
	{	ShowCursor(false);}

	Engine::PlaySound_SR(L"coh_ingame2.mp3", STAGE_SOUND, m_fSound);

	Engine::Update_BulletMgr(fTimeDelta);
	Engine::Update_EnermyMgr(fTimeDelta);
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CStage::LateUpdate_Scene(void)
{
	Collison_Object();
	Engine::LateUpdate_BulletMgr();
	Engine::LateUpdate_EnermyMgr();
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{
	// _DEBUG�� ���


	CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticView = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimView = Engine::Get_Object(L"Environment", L"AimCamera");

	if ((static_cast<CTankCamera*>(pTankView)->Get_CameraOn()) || static_cast<CStaticCamera*>(pStaticView)->Get_CameraOn())
	{
		// �������� ���� �ð�
		Render_Font(L"Font_AnSang5", CUI_FontMgr::GetInstance()->Get_Time_MIn(), &_vec2(WINCX_HALF - PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_White());
		Render_Font(L"Font_AnSang5", CUI_FontMgr::GetInstance()->Get_Time_Colon(), &_vec2(WINCX_HALF, PERCENTY), CUI_FontMgr::GetInstance()->Get_White());
		Render_Font(L"Font_AnSang5", CUI_FontMgr::GetInstance()->Get_Time_TenSec(), &_vec2(WINCX_HALF + PERCENTX * 2, PERCENTY), CUI_FontMgr::GetInstance()->Get_White());
		Render_Font(L"Font_AnSang5", CUI_FontMgr::GetInstance()->Get_Time_OneSec(), &_vec2(WINCX_HALF + PERCENTX * 4, PERCENTY), CUI_FontMgr::GetInstance()->Get_White());

		// �� ų ī��Ʈ
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_BlueTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY), CUI_FontMgr::GetInstance()->Get_Hecks_B());
		//Render_Font(L"Font_AnSang4", CUI_FontMgr::GetInstance()->Get_RedTeam_Kill(), &_vec2(_float(WINCX) - PERCENTX * 4.f, PERCENTY * 7.f), CUI_FontMgr::GetInstance()->Get_Hecks_R());

		// ��ũ ���� or �̸�
		Render_Font(L"Font_AnSang3", CUI_FontMgr::GetInstance()->Get_Tank_Name(), &_vec2(WINCX_HALF - PERCENTX * 10.f, WINCY - PERCENTY * 5.f), D3DXCOLOR(0.f, 0.64f, 0.f, 1.f));
	}
	else if (static_cast<CAimCamera*>(pAimView)->Get_CameraOn())
	{
		// ���� ī�޶� Ȯ�� ��ġ ��Ʈ �߰� ����
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

	pGameObject = CTankCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 15.f, -5.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TankCamera", pGameObject), E_FAIL);
	static_cast<CTankCamera*>(pGameObject)->Set_Camera(true);

	pGameObject = CStaticCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 20.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);
	static_cast<CStaticCamera*>(pGameObject)->Set_Camera(false);

	pGameObject = CAimCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 20.f, -5.f),
		&_vec3(0.f, 1.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AimCamera", pGameObject), E_FAIL);
	static_cast<CAimCamera*>(pGameObject)->Set_Camera(false);

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


	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environment_Object(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	_vec3 vPos, vRot;
	/* �� */
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			vPos = { float(rand() % 700 - 100 * i) , 0.f , float(rand() % 700 - 100 * j) };
			vRot = { 0.f , (float)(rand() % 180) , 0.f };

			pGameObject = CBuilding::Create(m_pGraphicDev, L"Rock_object", vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			static_cast<CBuilding*>(pGameObject)->SetRotation(vRot);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(
				static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
		}
	}
	/* �ǹ� */
	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			vPos = { float(rand() % 700 - 100 * i) , 0.f , float(rand() % 700 - 100 * j) };
			vRot = { 0.f , (float)(rand() % 180) , 0.f };

			pGameObject = CBuilding::Create(m_pGraphicDev, L"Building_object", vPos);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			static_cast<CBuilding*>(pGameObject)->SetRotation(vRot);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(
				static_cast<CBuilding*>(pGameObject)->GetID().c_str(), pGameObject), E_FAIL);
		}
	}


	pGameObject = CTestBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestBox", pGameObject), E_FAIL);

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

	//bottomdirenermy
	for (_int i = 0; 5> i; i++)
	{
		m_eData.eID = OBJID::OBJID_BDENERMY;
		m_eData.vPos.x = (float)VTXITV*(VTXCNTX / 2) + rand() % 200;
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)VTXITV*(VTXCNTZ / 2) + rand() % 100;

		CGameObject* pEnermy = CBottomDirEnermy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDENERMY);
	}

	//Default_Enermy
	_int iEnermy_Count = 5;
	CUI_FontMgr::GetInstance()->Set_AllCount(iEnermy_Count- 1);

	for (_int i = 0; (iEnermy_Count) > i; i++)
	{
		m_eData.eID = OBJID::OBJID_DEFAULT_ENERMY;
		m_eData.vPos.x = (float)VTXITV*(VTXCNTX / 2) + rand() % 200;
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)VTXITV*(VTXCNTZ / 2) + rand() % 250;

		CGameObject* pEnermy = CDefault_Enermy::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ENERMY);
	}
	//Ally
	for (_int i = 0; 5> i; i++)
	{
		m_eData.eID = OBJID::OBJID_DEFAULT_ALLY;
		m_eData.vPos.x = (float)(rand() % 125);
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)(rand() % 90);

		CGameObject* pEnermy = CDefault_Ally::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_DEFAULT_ALLY);
	}
	for (_int i = 0; 5 > i; i++)
	{
		m_eData.eID = OBJID::OBJID_BDALLY;
		m_eData.vPos.x = (float)(rand() % 125) + 100;
		m_eData.vPos.y = 0;
		m_eData.vPos.z = (float)(rand() % 150);

		CGameObject* pEnermy = CBottomDirAlly::Create(m_pGraphicDev, &m_eData);
		NULL_CHECK_RETURN(pEnermy, E_FAIL);
		Engine::Enermy_Add(pEnermy, OBJID::OBJID_BDALLY);
	}

	pGameObject = CShootEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootEffect", pGameObject), E_FAIL);

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

	pGameObject = CAim_UI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI", pGameObject), E_FAIL);

	pGameObject = CAim_UI_Pers::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aim_UI_Pers", pGameObject), E_FAIL);

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

	for (auto&iter = pEnvironment_Object->Get_mapObject()->begin(); pEnvironment_Object->Get_mapObject()->end() != iter; iter++)
	{
		for (auto&Dest = pGameLogic->Get_mapObject()->begin(); pGameLogic->Get_mapObject()->end() != Dest; Dest++)
		{
			if (!dynamic_cast<ICollisionable*>(iter->second) || !dynamic_cast<ICollisionable*>(Dest->second))
				continue;

			if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), iter->second->Get_Dist(), Dest->second->Get_Dist()))
				continue;

			if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(iter->second)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
				dynamic_cast<ICollisionable*>(Dest->second)->OBB_Collision_EX();
		}
	}

	//����) �Ѿ˰� �浹ó�� �ϰ� �����ֵ� �̷��� �߰��ϸ� ��
	for (int i = 0; BULLET_ID::BULLET_END > i; i++)
	{
		for (auto& iter = (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->begin(); iter != (CBulletMgr::GetInstance()->Get_Bullet_List((BULLET_ID)i))->end(); iter++)
		{
			for (auto& Dest = pEnvironment_Object->Get_mapObject()->begin(); pEnvironment_Object->Get_mapObject()->end() != Dest; Dest++)
			{
				if (!dynamic_cast<ICollisionable*>(*iter) || !dynamic_cast<ICollisionable*>(Dest->second))
					continue;

				if (!Engine::Sphere_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_Info(), dynamic_cast<ICollisionable*>(Dest->second)->Get_Info(), (*iter)->Get_Dist(), Dest->second->Get_Dist()))
					continue;

				if (Engine::OBB_Collision(dynamic_cast<ICollisionable*>(*iter)->Get_OBB(), dynamic_cast<ICollisionable*>(Dest->second)->Get_OBB()))
				{
					(*iter)->Set_Dead(true);
					// ä������ ���� �� �ߵ��� (�׿��� num ġ�� ��)
					//CUI_FontMgr::GetInstance()->Set_LiveCount(_uint(1));
					//CUI_FontMgr::GetInstance()->SendChatLog(wstring(L"��"), wstring(L"\t���� �ı�") + wstring( ���� ����));
				}

			}
			// ���⿡ �߰��� �浹ó�� �ϵ���
		}
	}

}

void CStage::Key_Input(const _float& fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_F1) == KEY_STATE::TAP)
	{
		if (m_fSound <= 0.f)
		{
			m_fSound = 0.f;
		}
		else
		{
			m_fSound -= 0.1f;
		}

	}
	if (Get_DIKeyState_Custom(DIK_F2) == KEY_STATE::TAP)
	{
		if (m_fSound >= 1.f)
		{
			m_fSound = 1.f;
		}
		else
		{
			m_fSound += 0.1f;
		}
	}

	// Test HP UI �ѤѤѤѤѤ� ���� ����
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
