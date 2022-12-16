#pragma once

#include "Effector.h"

BEGIN(Engine)

class ENGINE_DLL CGroundEffect : public CEffector
{
	const int MaxObjectCount = 10;
protected:
	explicit CGroundEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPos);
	explicit CGroundEffect(const CGroundEffect& rhs);
	virtual ~CGroundEffect();
public:
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
	virtual _int		Update_Component(const _float& fTimeDelta) override;

	virtual void RenderEffect() override;
	virtual void Reset() override;
public:
	static CGroundEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
	void SetColor(D3DXCOLOR color[]);
private:
	float m_fSpeed = 30.f;
	vector<_vec3> m_vecDir;
};

END