#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END

class CShootEffect :
	public CGameObject
{
public:
	explicit CShootEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShootEffect(const CShootEffect& rhs);
	virtual ~CShootEffect();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	void				Set_Target(CTransform* pTarget) { m_pTarget = pTarget; }
	void				Set_Dist(_float fLookDist, _float fRightDist, _float fUpDist) { m_fLookDist = fLookDist; m_fRightDist = fRightDist; m_fUpDist = fUpDist; }
	void				Set_Smoke(_bool _bSmoke) { m_bSmoke = _bSmoke; }
private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTarget = nullptr;
	
	_float				m_fLookDist = 0.f;
	_float				m_fRightDist = 0.f;
	_float				m_fUpDist = 0.f;
	_float				m_fFrame = 0.f;
	_bool				m_bSmoke = false;
	
	EFFECT_ID			m_eID = EFFECT_ID::TANK_SHOOT_SMOKE;
public:
	static CShootEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

