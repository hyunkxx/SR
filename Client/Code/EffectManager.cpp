#include "stdafx.h"
#include "..\EffectManager.h"
#include "Export_Function.h"
#include "EffectPool.h"

CEffectManager::CEffectManager(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CEffectManager::CEffectManager(const CEffectManager & rhs)
	: Engine::CGameObject(rhs)
{
}

CEffectManager::~CEffectManager()
{
}

HRESULT CEffectManager::Ready_Object(void)
{
	CGameObject::Ready_Object();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CEffectManager::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CEffectManager::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}

void CEffectManager::Render_Object(void)
{
	m_pEffectPool->RenderEffect(CEffectPool::EFFECT_TYPE::EXPLOSION);
	m_pEffectPool->RenderEffect(CEffectPool::EFFECT_TYPE::FIRE);
}

const _vec3 CEffectManager::Get_Info(void)
{
	return _vec3();
}


HRESULT CEffectManager::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pEffectPool = CEffectPool::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEffectPool, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"effect_pool", pComponent });

	return S_OK;
}

CEffectManager * CEffectManager::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectManager*		pInstance = new CEffectManager(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("TestPlayer Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffectManager::Free(void)
{
	__super::Free();
}
