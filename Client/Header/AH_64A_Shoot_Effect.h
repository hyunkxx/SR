#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CAH_64A_Shoot_Effect :
	public CGameObject
{
public:
	explicit CAH_64A_Shoot_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAH_64A_Shoot_Effect(const CAH_64A_Shoot_Effect& rhs);
	virtual ~CAH_64A_Shoot_Effect();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	void		 Set_Target(CTransform* pTarget) { m_pTarget = pTarget; }
	void		 Set_Left_Effect(_bool Effect) { m_bLeft = Effect; }
private:	
	HRESULT		Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTarget = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	EFFECT_ID			m_eID = EFFECT_ID::AH_64A_EFFECT;
private:
	_float			m_fFrame = 0.f;
	_bool			m_bLeft;
public:
	static CAH_64A_Shoot_Effect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;


};

