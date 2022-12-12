#include "..\..\Header\Effector.h"


USING(Engine)

_uint CEffector::EffectCount = 0;

CEffector::CEffector(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition)
	: CComponent(pGraphicDevice)
	, m_vPosition(vPosition)
	, m_fDuration(1.5f)
	, m_fLocalTime(0.f)
	, m_fGravityLocal(-2.f)
{
	m_vecEffect.reserve(60);
	EffectCount++;
}

CEffector::CEffector(const CEffector & rhs)
	: CComponent(rhs)
	, m_vPosition(rhs.m_vPosition)
	, m_fDuration(1.5f)
	, m_fLocalTime(0.f)
	, m_fGravityLocal(-2.f)
{
	m_vecEffect.reserve(60);
	EffectCount++;
}

CEffector::~CEffector()
{
}

void CEffector::Free(void)
{
	__super::Free();

	for (auto iter = m_vecEffect.begin(); iter != m_vecEffect.end(); ++iter)
		Safe_Release(*iter);

	EffectCount--;
}

_int CEffector::Update_Component(const _float & fTimeDelta)
{

	return 0;
}

CComponent* CEffector::Clone(void)
{
	return new CEffector(*this);
}

CEffector * CEffector::Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition)
{
	CEffector* pInstance = new CEffector(pGraphicDevice, vPosition);

	if (!pInstance)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed CEffector Create()");
	}

	return pInstance;
}