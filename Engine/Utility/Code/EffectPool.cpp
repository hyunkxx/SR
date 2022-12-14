#include "..\..\Header\EffectPool.h"

#include "Effector.h"
#include "ExplosionEffect.h"
#include "FireEffect.h"
#include "BulletEffect.h"
#include "GroundEffect.h"
#include "Utility.h"

USING(Engine)

CEffectPool::CEffectPool(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, nMaxPoolSize(200)
{
	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecExplosions.push_back(
			CExplosionEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}

	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecFire.push_back(
			CFireEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}


	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecBullet.push_back(
			CBulletEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}

	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecGround.push_back(
			CGroundEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}
}

CEffectPool::CEffectPool(const CEffectPool & rhs)
	: CComponent(rhs.m_pGraphicDev)
	, nMaxPoolSize(200)
{
	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecExplosions.push_back(
			CExplosionEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}

	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecFire.push_back(
			CFireEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}

	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecBullet.push_back(
			CBulletEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}

	for (_uint i = 0; i < nMaxPoolSize; ++i)
	{
		m_vecGround.push_back(
			CGroundEffect::Create(m_pGraphicDev, { 0.f,0.f,0.f }));
	}
}


CEffectPool::~CEffectPool()
{
}

CEffectPool* CEffectPool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectPool* pInstance = new CEffectPool(pGraphicDev);
	if (FAILED(pInstance))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

_int CEffectPool::Update_Component(const _float & fTimeDelta)
{
	for (auto Effect = m_ExplosionPool.begin(); Effect != m_ExplosionPool.end(); )
	{
		(*Effect)->Update_Component(fTimeDelta);
		if (!(*Effect)->GetRunngin())
		{

			(*Effect)->Reset();
			Effect = m_ExplosionPool.erase(Effect);
		}
		else
		{
			++Effect;
		}
	}

	for (auto Effect = m_FirePool.begin(); Effect != m_FirePool.end(); )
	{
		(*Effect)->Update_Component(fTimeDelta);

		if (!(*Effect)->GetRunngin())
		{
			(*Effect)->Reset();
			Effect = m_FirePool.erase(Effect);
		}
		else
		{
			++Effect;
		}
	}

	for (auto Effect = m_BulletPool.begin(); Effect != m_BulletPool.end(); )
	{
		(*Effect)->Update_Component(fTimeDelta);

		if (!(*Effect)->GetRunngin())
		{
			(*Effect)->Reset();
			Effect = m_BulletPool.erase(Effect);
		}
		else
		{
			++Effect;
		}
	}

	for (auto Effect = m_GroundPool.begin(); Effect != m_GroundPool.end(); )
	{
		(*Effect)->Update_Component(fTimeDelta);

		if (!(*Effect)->GetRunngin())
		{
			(*Effect)->Reset();
			Effect = m_GroundPool.erase(Effect);
		}
		else
		{
			++Effect;
		}
	}

	return _int();
}

CComponent* CEffectPool::Clone(void)
{
	return new CEffectPool(*this);
}

void CEffectPool::Free(void)
{
	for (auto Effect = m_vecExplosions.begin(); Effect != m_vecExplosions.end(); )
	{
		Safe_Release(*Effect);
		Effect = m_vecExplosions.erase(Effect);
	}

	for (auto Effect = m_vecFire.begin(); Effect != m_vecFire.end(); )
	{
		Safe_Release(*Effect);
		Effect = m_vecFire.erase(Effect);
	}

	for (auto Effect = m_vecBullet.begin(); Effect != m_vecBullet.end(); )
	{
		Safe_Release(*Effect);
		Effect = m_vecBullet.erase(Effect);
	}

	for (auto Effect = m_vecGround.begin(); Effect != m_vecGround.end(); )
	{
		Safe_Release(*Effect);
		Effect = m_vecGround.erase(Effect);
	}
	__super::Free();
}

void CEffectPool::UseEffect(EFFECT_TYPE eType, _vec3 vPos)
{
	if (Utility::Cuilling(m_pGraphicDev, vPos))
		return;

	switch (eType)
	{
	case EFFECT_TYPE::EXPLOSION:
		AddExplosionPool(vPos);
		break;
	case EFFECT_TYPE::FIRE:
		AddFirePool(vPos);
		break;
	case EFFECT_TYPE::BULLET:
		AddBulletPool(vPos);
		break;
	case EFFECT_TYPE::GROUND:
		AddGroundPool(vPos);
		break;
	}
}

void CEffectPool::RenderEffect(EFFECT_TYPE eType)
{
	for (auto Effect = m_ExplosionPool.begin(); Effect != m_ExplosionPool.end(); ++Effect)
	{
		(*Effect)->RenderEffect();
	}

	for (auto Effect = m_FirePool.begin(); Effect != m_FirePool.end(); ++Effect)
	{
		(*Effect)->RenderEffect();
	}

	for (auto Effect = m_BulletPool.begin(); Effect != m_BulletPool.end(); ++Effect)
	{
		(*Effect)->RenderEffect();
	}

	for (auto Effect = m_GroundPool.begin(); Effect != m_GroundPool.end(); ++Effect)
	{
		(*Effect)->RenderEffect();
	}
}

void CEffectPool::AddExplosionPool(_vec3 vPos)
{
	if (m_ExplosionPool.size() >= nMaxPoolSize - 1)
		return;

	auto& Effect = m_vecExplosions.begin();
	for (; Effect != m_vecExplosions.end(); ++Effect)
	{
		if (!(*Effect)->GetRunngin())
		{
			m_ExplosionPool.push_back(*Effect);
			break;
		}
	}

	(*Effect)->SetPosition(vPos);
	(*Effect)->SetRunning(true);
}

void CEffectPool::AddFirePool(_vec3 vPos)
{
	if (m_FirePool.size() >= nMaxPoolSize - 1)
		return;

	auto& Effect = m_vecFire.begin();
	for (; Effect != m_vecFire.end(); ++Effect)
	{
		if (!(*Effect)->GetRunngin())
		{
			m_FirePool.push_back(*Effect);
			break;
		}
	}

	(*Effect)->SetPosition(vPos);
	(*Effect)->SetRunning(true);
}

void CEffectPool::AddBulletPool(_vec3 vPos)
{
	if (m_BulletPool.size() >= nMaxPoolSize - 1)
		return;

	auto& Effect = m_vecBullet.begin();
	for (; Effect != m_vecBullet.end(); ++Effect)
	{
		if (!(*Effect)->GetRunngin())
		{
			m_BulletPool.push_back(*Effect);
			break;
		}
	}

	(*Effect)->SetPosition(vPos);
	(*Effect)->SetRunning(true);
}

void CEffectPool::AddGroundPool(_vec3 vPos)
{
	if (m_GroundPool.size() >= nMaxPoolSize - 1)
		return;

	auto& Effect = m_vecGround.begin();
	for (; Effect != m_vecGround.end(); ++Effect)
	{
		if (!(*Effect)->GetRunngin())
		{
			m_GroundPool.push_back(*Effect);
			break;
		}
	}

	(*Effect)->SetPosition(vPos);
	(*Effect)->SetRunning(true);
}
