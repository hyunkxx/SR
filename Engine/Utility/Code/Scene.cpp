#include "..\..\Header\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}


CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto	iter = find_if(m_umapLayer.begin(), m_umapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_umapLayer.end())
		return nullptr;

	return  iter->second;
}

CGameObject * CScene::Get_Object(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_umapLayer.begin(), m_umapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_umapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_umapLayer.begin(), m_umapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_umapLayer.end())
		return nullptr;
	
	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

HRESULT CScene::Add_GameObject(const _tchar * pObjTag, CGameObject * pObject)
{
	auto	iter = find_if(m_umapLayer.begin(), m_umapLayer.end(), CTag_Finder(pObjTag));

	if (iter == m_umapLayer.end())
		return S_FALSE;

	iter->second->Add_GameObject(pObjTag, pObject);
	return S_OK;
}

void CScene::Delete_Object(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_umapLayer.begin(), m_umapLayer.end(), CTag_Finder(pLayerTag));
	iter->second->Delete_Object(pObjTag);
}

HRESULT CScene::Ready_Scene(void)
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int	iResult = 0;

	for (auto& iter : m_umapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::LateUpdate_Scene(void)
{
	for (auto& iter : m_umapLayer)
		iter.second->LateUpdate_Layer();
}

void CScene::RenderGUI(void)
{
	for (auto& iter : m_umapLayer)
	{
		iter.second->RenderGUI();
	}
}

void CScene::Free(void)
{
	for_each(m_umapLayer.begin(), m_umapLayer.end(), CDeleteMap());
	m_umapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
