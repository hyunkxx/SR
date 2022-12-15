#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "ImguiManager.h"
#include "Export_Function.h"
#include "Stage.h"
#include "TankManager.h"
#include "GameMode.h"

CMainApp::CMainApp()
	: m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	srand((_uint)time(nullptr));
	ShowCursor(false);
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	ImguiManager::GetInstance()->Initalize(m_pGraphicDev);

	return S_OK;
}

int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	SetUp_InputDev();
	Update_InputDev();

	ImguiManager::GetInstance()->NewFrame();

	NULL_CHECK_RETURN(m_pManagementClass, -1);
	m_pManagementClass->Update_Scene(fTimeDelta);


	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	NULL_CHECK(m_pManagementClass);
	m_pManagementClass->LateUpdate_Scene();
}

void CMainApp::Render_MainApp(void)
{
	Engine::Get_GraphicDev()->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::RenderGUI();
	ImguiManager::GetInstance()->EndFrame();
	Engine::Render_Begin(D3DXCOLOR(.2f, 0.2f, 0.2f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);
	ImguiManager::GetInstance()->Render();
	Engine::Render_End();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();
	
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_Default", L"����", 8, 13, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font((*ppGraphicDev), L"Font_GOTHIC", L"���", 30, 30, FW_HEAVY), E_FAIL);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Sitka1", L"Sitka Text", 5, 10, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Sitka1_1", L"Sitka Text", 6, 11, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Sitka2", L"Sitka Text", 8, 13, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Sitka3", L"Sitka Text", 10, 15, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Sitka4", L"Sitka Text", 15, 20, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang1", L"�Ȼ��2006����", 5, 10, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang2", L"�Ȼ��2006����", 8, 13, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang3", L"�Ȼ��2006����", 10, 15, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang4", L"�Ȼ��2006����", 15, 20, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang5", L"�Ȼ��2006����", 20, 25, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang7", L"�Ȼ��2006����", 30, 35, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_AnSang6", L"�Ȼ��2006����", 50, 65, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Retro2", L"Galmuri6 Regular", 10, 15, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Retro", L"Galmuri7 Regular", 15, 25, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Retro3", L"Galmuri7 Regular", 10, 15, FW_HEAVY);
	Engine::Ready_Font((*ppGraphicDev), L"Font_Retro1", L"Galmuri7 Regular", 25, 35, FW_HEAVY);

	//(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, TRUE);		// ���� ���� �ݻ� ���� ���� ����
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);			// z���ۿ� ������ ���� ���� ����ϵ� ������ �������� ���� �Ǵ��ϴ� �ɼ�
	//(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z���ۿ� �ȼ��� ���� ���� �������� ���� �����ϴ� ����
	

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	FAILED_CHECK_RETURN(Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene*			pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);


	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp *	pInstance = new	CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMainApp::Free(void)
{
	ImguiManager::DestroyInstance();
	CTankManager::DestroyInstance();
	

	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);


	Engine::Release_Utility();
	Engine::Release_System();
}
