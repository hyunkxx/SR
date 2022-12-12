#pragma once
#include "Effector.h"

class ENGINE_DLL CFireEffect : public CEffector
{
protected:
	explicit CFireEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
	explicit CFireEffect(const CFireEffect& rhs);
	virtual ~CFireEffect();

public:
	const int MaxObjectCount = 10;
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
	virtual _int		Update_Component(const _float& fTimeDelta) override;

	virtual void RenderEffect() override;
	virtual void Reset() override;
public:
	static CFireEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
private:
	float m_fSpeed = 150.f;
	vector<_vec3> m_vDir;
};

