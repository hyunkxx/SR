#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CBoomEffect :
	public CGameObject
{
public:
	explicit CBoomEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoomEffect(const CBoomEffect& rhs);
	virtual ~CBoomEffect();
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
public:
	void		 Set_Pos(_vec3 Pos) {m_vPos = Pos;}
private:
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	EFFECT_ID			m_eID = EFFECT_ID::BOOMER_BOOM_SMOKE;
private:
	_float			m_fFrame = 0.f;
	_vec3			m_vPos;
public:
	static CBoomEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

