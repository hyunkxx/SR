#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CBossHitPoint :
	public CGameObject
{
public:
	explicit CBossHitPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossHitPoint(const CBossHitPoint& rhs);
	virtual ~CBossHitPoint();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void				Hit_Point_Set(_vec3 Pos, _float Time);

private:
	HRESULT		Add_Component(void);

private:

	CTransform*			m_pTransformHitPoint = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRcTex*				m_pBufferCom = nullptr;

	_float				m_pRender_Time = 90.f;
	_float				m_pRender_Count = 100.f;
public:
	virtual void Free(void) override;
	static CBossHitPoint* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

