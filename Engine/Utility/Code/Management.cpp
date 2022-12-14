#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
	Free();
}

CComponent * CManagement::Get_Component(const _tchar * pLayerTag,
	const _tchar * pObjTag,
	const _tchar * pComponentTag,
	COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

HRESULT CManagement::Add_GameObject(const _tchar * pObjTag, CGameObject * pObject)
{
	if (nullptr == m_pScene)
		return S_FALSE;
	m_pScene->Add_GameObject(pObjTag, pObject);
	return S_OK;
}

CLayer * CManagement::Find_Layer(const _tchar * pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;
	return m_pScene->Get_Layer(pLayerTag);
}

CGameObject* CManagement::Swap_Object(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pObj)
{
	return m_pScene->Swap_Object(pLayerTag, pObjTag, pObj);
}

void CManagement::Delete_Object(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	m_pScene->Delete_Object(pLayerTag, pObjTag);
}


CLayer * CManagement::Get_Layer(const _tchar * pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;
	return  m_pScene->Get_Layer(pLayerTag);
}

CGameObject * CManagement::Get_Object(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	return m_pScene->Get_Object(pLayerTag, pObjTag);
}



HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);

	Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(void)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void CManagement::RenderGUI()
{
	m_pScene->RenderGUI();
}

void CManagement::Free(void)
{
	Safe_Release(m_pScene);
}
