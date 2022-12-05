#pragma once

#include "Include.h"
#include "Component.h"

BEGIN(Engine)
class CEffector;
class CExplosionEffect;

class ENGINE_DLL CEffectPool : public CComponent
{
	const _uint nMaxPoolSize;
public:
	enum class EFFECT_TYPE { EXPLOSION, FIRE, MAX };
	static CEffectPool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	explicit CEffectPool(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffectPool(const CEffectPool& rhs);
	virtual ~CEffectPool();
public:
	virtual _int		Update_Component(const _float& fTimeDelta);
	virtual CComponent*	Clone(void)	override;
	virtual void		Free(void)	override;
public:
	void UseEffect(EFFECT_TYPE eType, _vec3 vPos);
	void RenderEffect(EFFECT_TYPE eType);
private:
	void AddExplosionPool(_vec3 vPos);
	void AddFirePool(_vec3 vPos);
private:

public:
	vector<CEffector*> m_vecExplosions;
	vector<CEffector*> m_vecFire;

	list<CEffector*> m_ExplosionPool;
	list<CEffector*> m_FirePool;
};

END

