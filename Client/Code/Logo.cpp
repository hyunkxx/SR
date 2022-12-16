#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"
#include "MainMenu.h"
#include "TestPlayer.h"
#include "BackGround.h"
#include "ModeSelectMenu.h"
#include "Stage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Scene(fTimeDelta);

	Engine::PlaySound_SR(L"coh_menu.mp3", SELECT_MENU_BGM, 1.f);

	if (true == m_pLoading->Get_Finish())
	{
		if (Get_DIKeyState_Custom(DIK_RETURN) == KEY_STATE::TAP)
		{
			CScene*		pScene = nullptr;

			Engine::PlaySound_SR(L"enter.mp3", SELECT_SOUND, 1.f);

			pScene = CModeSelectMenu::Create(m_pGraphicDev);

			NULL_CHECK_RETURN(pScene, -1);
			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{
	// _DEBUG¿ë Ãâ·Â
	Render_Font(L"Font_Retro", m_pLoading->Get_String(), &_vec2(PERCENTX, WINCY - 32.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	__super::Free();
}
HRESULT CLogo::Ready_Prototype(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	//ÅÊÅ© 
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TankBody", Engine::CTank_Body::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TankPosin", Engine::CTankPosin::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_TankHead", Engine::CTank_Head::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_VIBullet", Engine::CVIBullet::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_VIBullet_Re", Engine::CVIBullet_Re::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_HitPoint", Engine::CHitPoint::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_VISmoke", Engine::CVISmoke::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/Back.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShootEffectTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Smoke/Smoke_%d.png", TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Gun_ShootEffectTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Gun_Smoke/Gun_Effect_0%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Gun_Smoke_Shell", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Smoke_Shell/Smoke_shell_%d.png", TEX_NORMAL, 63)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Boom", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Boom/Boom_%d.png", TEX_NORMAL, 33)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Posin_Smoke", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/PosinSmoke/Posin_Smoke_0%d.png", TEX_NORMAL, 4)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_0", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_1.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_3", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_3.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_4", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_4.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_4_1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_4.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_4_2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_4.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"target_4_3", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ui/target_4.png", TEX_NORMAL)), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// CBackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	m_umapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}
