#include "stdafx.h"
#include "..\Header\ModeSelectMenu.h"

#include "Export_Function.h"

#include "Utility.h"
#include "Rush.h"
#include "Stage.h"
#include "ObjectMesh.h"
#include "SimpleCamera.h"
#include "DefaultUI.h"
#include "TankManager.h"
#include "UI_FontMgr.h"
#include "VehicleInfoUI.h"
#include "ModeButton.h"
#include "GameButton.h"
#include "MainMenu.h"
#include "GameMode.h"

CModeSelectMenu::CModeSelectMenu(LPDIRECT3DDEVICE9 pGraphic)
	: CScene(pGraphic)
	, m_vCamPosition({ 0.f, 3.f, -13.f })
	, m_pCamera(nullptr)
{
}

CModeSelectMenu::~CModeSelectMenu()
{
}

CModeSelectMenu* CModeSelectMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CModeSelectMenu*	pInstance = new CModeSelectMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CModeSelectMenu::Free(void)
{
	__super::Free();
}

HRESULT CModeSelectMenu::Ready_Scene(void)
{
	(Ready_Layer_Environment(L"Background"), E_FAIL);
	(Ready_Layer_GameLogic(L"GameObject"), E_FAIL);
	(Ready_Layer_UI(L"UI"), E_FAIL);

	return S_OK;
}

_int CModeSelectMenu::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (CGameMode::GetInstance()->m_bGameExit)
	{
		DestroyWindow(g_hWnd);
	}

	Engine::PlaySound_SR(L"coh_menu.mp3", SELECT_MENU_BGM, m_fSound);

	KeyInput();

	if (CGameMode::GetInstance()->m_bWarPressed || CGameMode::GetInstance()->m_bRushPressed)
	{
		//War
		if (CGameMode::GetInstance()->m_bWarPressed)
		{
			if (CGameMode::GetInstance()->m_bGameStart)
			{
				Engine::StopSound(SELECT_SOUND);
				Engine::PlaySound_SR(L"enter.mp3", SELECT_SOUND, 1.f);

				CScene*		pScene = nullptr;

				pScene = CMainMenu::Create(m_pGraphicDev);

				NULL_CHECK_RETURN(pScene, -1);
				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			}
		}
		//Rush
		else if (CGameMode::GetInstance()->m_bRushPressed)
		{
			if (CGameMode::GetInstance()->m_bGameStart)
			{
				Engine::StopSound(SELECT_SOUND);
				Engine::PlaySound_SR(L"enter.mp3", SELECT_SOUND, 1.f);

				CScene*		pScene = nullptr;

				pScene = CRush::Create(m_pGraphicDev);

				NULL_CHECK_RETURN(pScene, -1);
				FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			}
		}
	}

	return iExit;
}

void CModeSelectMenu::LateUpdate_Scene(void)
{


	CScene::LateUpdate_Scene();
}

void CModeSelectMenu::Render_Scene(void)
{
}

HRESULT CModeSelectMenu::Ready_Layer_Environment(const _tchar * pLayerTag)
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

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CModeSelectMenu::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*	pLayer = CLayer::Create();

	m_umapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CModeSelectMenu::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*	pLayer = CLayer::Create();

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	m_pBackground = pGameObject = CDefaultUI::Create(m_pGraphicDev,
		L"SeletMenuBackground",
		L"../Bin/Resource/Texture/Ui/Background2.png");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SeletMenuBackground", pGameObject), E_FAIL);

	static_cast<CDefaultUI*>(m_pBackground)->SetPosition({ 400.f,300.f, 1.f });
	static_cast<CDefaultUI*>(m_pBackground)->SetScale({ 400.f,300.f,0.f });

	pGameObject = m_pWarButton = CModeButton::Create(m_pGraphicDev, CModeButton::GMODE::WAR);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarButton", pGameObject), E_FAIL);
	static_cast<CModeButton*>(m_pWarButton)->Set_PosX(220);
	static_cast<CModeButton*>(m_pWarButton)->Set_PosY(200);

	pGameObject = m_pRushButton = CModeButton::Create(m_pGraphicDev, CModeButton::GMODE::RUSH);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RushButton", pGameObject), E_FAIL);
	static_cast<CModeButton*>(m_pRushButton)->Set_PosX(580);
	static_cast<CModeButton*>(m_pRushButton)->Set_PosY(200);

	pGameObject = m_pStartButton = CGameButton::Create(m_pGraphicDev, CGameButton::TYPE::START);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartButton", pGameObject), E_FAIL);
	static_cast<CGameButton*>(m_pStartButton)->Set_PosX(WINCX * 0.5f);
	static_cast<CGameButton*>(m_pStartButton)->Set_PosY(420);

	pGameObject = m_pExitButton = CGameButton::Create(m_pGraphicDev, CGameButton::TYPE::EXIT);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ExitButton", pGameObject), E_FAIL);
	static_cast<CGameButton*>(m_pExitButton)->Set_PosX(WINCX * 0.5f);
	static_cast<CGameButton*>(m_pExitButton)->Set_PosY(490);

	m_umapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

void CModeSelectMenu::KeyInput()
{
}

void CModeSelectMenu::Collison_Object(void)
{
}