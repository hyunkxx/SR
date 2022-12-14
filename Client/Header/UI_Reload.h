#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Reload : public Engine::CGameObject
{
private:
	explicit CUI_Reload(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Reload(const CUI_Reload& rhs);
	virtual ~CUI_Reload();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Reload*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	CTexture*					m_pFTexture = nullptr;
	CTransform*				m_pFTransform = nullptr;

	_matrix  m_matProj1 , m_matProj2;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;


	_float					m_fFScaleX;
	_float					m_fFScaleY;
	_float					m_fFScaleZ;

	_float					m_fFPosX;
	_float					m_fFPosY;
	_float					m_fFPosZ;

	_float					m_fFOrgin_ScaleX;
	_float					m_fFOrgin_PosX;


	_float					m_fSpeed;

	wstring				m_szTankType;
	Tank_State			TankInfo;

	_float					m_fTime = 0.f;

public:

	void				Set_Speed(_float _fSpeed) { m_fSpeed = _fSpeed; }

	void				 Update_Speed(void);
	void				 UI_Speed_minus(_float Fuel_minus) { m_fSpeed -= Fuel_minus; }
	void				 UI_Speed_plus(_float  Fuel_plus) { m_fSpeed += Fuel_plus; }



};

