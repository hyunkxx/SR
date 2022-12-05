#pragma once

#include "Effector.h"

BEGIN(Engine)

class ENGINE_DLL CExplosionEffect : public CEffector
{
	const int MaxObjectCount = 10;
protected:
	explicit CExplosionEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
	explicit CExplosionEffect(const CExplosionEffect& rhs);
	virtual ~CExplosionEffect();
public:
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
	virtual _int		Update_Component(const _float& fTimeDelta) override;

	virtual void RenderEffect() override;
	virtual void Reset() override;
public:
	static CExplosionEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
private:
	float m_fSpeed = 40.f;
	vector<_vec3> m_vecDir;
};

END