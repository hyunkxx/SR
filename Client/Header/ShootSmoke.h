#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END

class CShootSmoke :
	public CGameObject
{
public:
	explicit CShootSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShootSmoke(const CShootSmoke& rhs);
	virtual ~CShootSmoke();
public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
public:
	void				Effect_Setting(_vec3 Pos);

private:
	HRESULT				Add_Component(void);
private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	_vec3				m_vPos;

	_float				m_fFrame = 0.f;
	_bool				m_bFrameRepeat = false;
	_float				m_fDeadTime = 0.f;
	EFFECT_ID			m_eID = EFFECT_ID::SHIP_SMOKE_EFFECT;
public:
	static CShootSmoke*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

