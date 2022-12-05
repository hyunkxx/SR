#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_World_HP_Back : public Engine::CGameObject
{
private:
	explicit CUI_World_HP_Back(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_World_HP_Back(const CUI_World_HP_Back& rhs);
	virtual ~CUI_World_HP_Back();

public:
	HRESULT Ready_Object(_float fOrgin_HP);
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_World_HP_Back*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fOrigin_HP);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	_matrix matView;
	_matrix  m_matProj;

private:
	_float					m_fOrgin_HP;
	_float					m_fHP;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;

	_float					m_fOrgin_ScaleX;
	_float					m_fOrgin_PosX;

public:
	const _float Get_HP_UI(void) { return m_fHP; }
	void				 Update_HP(void);
	void				 Minus_HP_UI(_float HP_minus) { m_fHP -= HP_minus; }
	void				 Plus_HP_UI(_float HP_plus) { m_fHP += HP_plus; }


};

