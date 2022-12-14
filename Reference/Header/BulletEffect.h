#pragma once

#include "Effector.h"

BEGIN(Engine)

class ENGINE_DLL CBulletEffect : public CEffector
{
	const int MaxObjectCount = 5;
protected:
	explicit CBulletEffect(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPos);
	explicit CBulletEffect(const CBulletEffect& rhs);
	virtual ~CBulletEffect();
public:
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
	virtual _int		Update_Component(const _float& fTimeDelta) override;

	virtual void RenderEffect() override;
	virtual void Reset() override;
public:
	static CBulletEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _vec3 vPosition);
	void SetColor(D3DXCOLOR color[]);
private:
	float m_fSpeed = 20.f;
	vector<_vec3> m_vecDir;
};

END