#include "stdafx.h"
#include "..\Header\MainMenu.h"

#include "Export_Function.h"

#include "Utility.h"
#include "Stage.h"
#include "ObjectMesh.h"
#include "SimpleCamera.h"
#include "DefaultUI.h"
#include "TankManager.h"
#include "UI_FontMgr.h"
#include "VehicleInfoUI.h"

CMainMenu::CMainMenu(LPDIRECT3DDEVICE9 pGraphic)
	: CScene(pGraphic)
	, m_vCamPosition({ 0.f, 3.f, -13.f })
	, m_pCamera(nullptr)
	, m_nCurrentIndex(0)
{
}

CMainMenu::~CMainMenu()
{
}

CMainMenu* CMainMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainMenu*	pInstance = new CMainMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainMenu::Free(void)
{
	__super::Free();
}

HRESULT CMainMenu::Ready_Scene(void)
{
	(Ready_Layer_Environment(L"Background"), E_FAIL);
	(Ready_Layer_GameLogic(L"GameObject"), E_FAIL);
	(Ready_Layer_UI(L"UI"), E_FAIL);

	return S_OK;
}

_int CMainMenu::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	Engine::PlaySound_SR(L"coh_menu.mp3", SELECT_MENU_BGM, m_fSound);

	KeyInput();
	UpdateCam(fTimeDelta);

	//static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->Rotation(fTimeDelta);

	if (Get_DIKeyState_Custom(DIK_RETURN) == KEY_STATE::TAP)
	{
		Engine::PlaySound_SR(L"enter.mp3", SELECT_SOUND, 1.f);

		CreateVehicle();

		CScene*		pScene = nullptr;

		pScene = CStage::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pScene, -1);
		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
	}

	return iExit;
}

void CMainMenu::LateUpdate_Scene(void)
{


	CScene::LateUpdate_Scene();
}

void CMainMenu::Render_Scene(void)
{
}

HRESULT CMainMenu::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CLayer*	pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	m_pCamera = pGameObject = CSimpleCamera::Create(m_pGraphicDev,
		&m_vCamPosition,
		&_vec3(0.f, 0.f, 0.f),
		&_vec3(0.f, 1.f, 0.f));

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SimpleCamera", pGameObject), E_FAIL);

	m_pVehicle[(UINT)VEHICLE::HUMVEE] = pGameObject = CObjectMesh::Create(m_pGraphicDev, L"Humvee");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Humvee", pGameObject);
	static_cast<CObjectMesh*>(pGameObject)->SetPosition({ -10.f,0.f,0.f });

	m_pVehicle[(UINT)VEHICLE::SMALL_TANK] = pGameObject = CObjectMesh::Create(m_pGraphicDev, L"Small");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Small", pGameObject);
	static_cast<CObjectMesh*>(pGameObject)->SetPosition({ -5.f,0.f,0.f });

	m_pVehicle[(UINT)VEHICLE::MIDDLE_TANK] = pGameObject = CObjectMesh::Create(m_pGraphicDev, L"Middle");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Middle", pGameObject);
	static_cast<CObjectMesh*>(pGameObject)->SetPosition({ 0.f,0.f,0.f });

	m_pVehicle[(UINT)VEHICLE::BIG_TANK] = pGameObject = CObjectMesh::Create(m_pGraphicDev, L"Big");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Big", pGameObject);
	static_cast<CObjectMesh*>(pGameObject)->SetPosition({ 5.f,0.f,0.f });

	m_pVehicle[(UINT)VEHICLE::LONG_TANK] = pGameObject = CObjectMesh::Create(m_pGraphicDev, L"Long");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Long", pGameObject);
	static_cast<CObjectMesh*>(pGameObject)->SetPosition({ 10.f,0.f,0.f });

	static_cast<CObjectMesh*>(m_pVehicle[(UINT)m_nCurrentIndex])->Selected();

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMainMenu::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*	pLayer = CLayer::Create();

	m_umapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CMainMenu::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*	pLayer = CLayer::Create();

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	m_pInfoBackground = pGameObject = CDefaultUI::Create(m_pGraphicDev,
		L"black",
		L"../Bin/Resource/Texture/Ui/white.png");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"white", pGameObject), E_FAIL);

	static_cast<CDefaultUI*>(m_pInfoBackground)->SetPosition({ WINCX - 230.f, WINCY - 190.f, 0.1f });
	static_cast<CDefaultUI*>(m_pInfoBackground)->SetScale({ 140.f ,135.f,0.f });

	m_pBackground = pGameObject = CDefaultUI::Create(m_pGraphicDev,
		L"SeletMenuBackground",
		L"../Bin/Resource/Texture/Ui/white.png");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SeletMenuBackground", pGameObject), E_FAIL);

	static_cast<CDefaultUI*>(m_pBackground)->SetPosition({ 400.f,300.f, 1.f });
	static_cast<CDefaultUI*>(m_pBackground)->SetScale({ 400.f,300.f,0.f });

	m_pVehicleInfo = pGameObject = CVehicleInfoUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VehicleInfo", pGameObject), E_FAIL);


	m_umapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

void CMainMenu::CreateVehicle()
{
	switch (m_nCurrentIndex)
	{
	case (UINT)VEHICLE::HUMVEE:
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::HUMVEE);
		CUI_FontMgr::GetInstance()->SavePlayerName(CTankManager::GetInstance()->GetName(VEHICLE::HUMVEE));
		break;
	case (UINT)VEHICLE::SMALL_TANK:
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::SMALL_TANK);
		CUI_FontMgr::GetInstance()->SavePlayerName(CTankManager::GetInstance()->GetName(VEHICLE::SMALL_TANK));
		break;
	case (UINT)VEHICLE::MIDDLE_TANK:
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::MIDDLE_TANK);
		CUI_FontMgr::GetInstance()->SavePlayerName(CTankManager::GetInstance()->GetName(VEHICLE::MIDDLE_TANK));
		break;
	case (UINT)VEHICLE::BIG_TANK:
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::BIG_TANK);
		CUI_FontMgr::GetInstance()->SavePlayerName(CTankManager::GetInstance()->GetName(VEHICLE::BIG_TANK));
		break;
	case (UINT)VEHICLE::LONG_TANK:
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::LONG_TANK);
		CUI_FontMgr::GetInstance()->SavePlayerName(CTankManager::GetInstance()->GetName(VEHICLE::LONG_TANK));
		break;
	case (UINT)VEHICLE::MAX:
	default:
		MSG_BOX("해당 차량은 미구현상태 입니다. Humvee로 생성합니다.");
		CTankManager::GetInstance()->CreateVehicle(m_pGraphicDev, VEHICLE::HUMVEE);
		CUI_FontMgr::GetInstance()->SavePlayerName(L"Humvee");
		break;
	}
}

void CMainMenu::KeyInput()
{
	if (Get_DIKeyState_Custom(DIK_RIGHT) == KEY_STATE::TAP)
	{
		if (m_nCurrentIndex < (UINT)VEHICLE::MAX - 1)
		{
			Engine::StopSound(PASS_SOUND);
			Engine::PlaySound_SR(L"pass.mp3", PASS_SOUND, m_fSound);
			static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->UnSelect();
			m_nCurrentIndex++;
			static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->Selected();

			static_cast<CVehicleInfoUI*>(m_pVehicleInfo)->SetIndex(m_nCurrentIndex);
		}
	}
	if (Get_DIKeyState_Custom(DIK_LEFT) == KEY_STATE::TAP)
	{
		if (m_nCurrentIndex > 0)
		{
			Engine::StopSound(PASS_SOUND);
			Engine::PlaySound_SR(L"pass.mp3", PASS_SOUND, m_fSound);
			static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->UnSelect();
			m_nCurrentIndex--;
			static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->Selected();

			static_cast<CVehicleInfoUI*>(m_pVehicleInfo)->SetIndex(m_nCurrentIndex);
		}
	}
}

void CMainMenu::UpdateCam(const _float & fTimeDelta)
{

	float x = Utility::Lerp(static_cast<CSimpleCamera*>(m_pCamera)->Get_Info().x, static_cast<CObjectMesh*>(m_pVehicle[m_nCurrentIndex])->Get_Info().x, 2.f * fTimeDelta);

	static_cast<CSimpleCamera*>(m_pCamera)->SetEye({ x, 3.f , -13.f });
	static_cast<CSimpleCamera*>(m_pCamera)->SetLookAt({ x, 0.f, -8.f });
}

void CMainMenu::Collison_Object(void)
{
}