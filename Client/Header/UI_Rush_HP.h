#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Rush_HP : public Engine::CGameObject
{
private:
	explicit CUI_Rush_HP(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Rush_HP(const CUI_Rush_HP& rhs);
	virtual ~CUI_Rush_HP();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Rush_HP*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*						m_pTexture[10];
	CTransform*				m_pTransform[10];


	_matrix  m_matProj;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX[10];
	_float					m_fPosY[10];
	_float					m_fPosZ;


	_bool					m_bShow[10];


	_int						m_LifeCount;



public:

	void Key_Input(const _float& fTimeDelta);

	void Update_Life_Count(void);


};

