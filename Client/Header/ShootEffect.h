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
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	_float			m_fFrame = 0.f;

public:
	static CShootEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

