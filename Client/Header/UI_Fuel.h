#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Fuel : public Engine::CGameObject
{
private:
	explicit CUI_Fuel(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Fuel(const CUI_Fuel& rhs);
	virtual ~CUI_Fuel();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Fuel*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	// fuel bar
	CTexture*					m_pFTexture = nullptr;
	CTransform*				m_pFTransform = nullptr;


	_matrix  m_matProj1 , m_matProj2;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;


	// fuel bar
	_float					m_fFScaleX;
	_float					m_fFScaleY;
	_float					m_fFScaleZ;

	_float					m_fFPosX;
	_float					m_fFPosY;
	_float					m_fFPosZ;

	_float					m_fFOrigin_ScaleY;
	_float					m_fFOrigin_PosY;

	wstring				m_szTankType;
	Tank_State			TankInfo;

	_float					m_fFuel;
	_float					m_fOrigin_fuel;

public:

	void				Set_Fuel(_float _fFuel) { m_fFuel = _fFuel; }
	_float			Get_Fuel(void) { return m_fFuel; }


	void				 Update_Fuel(const _float& fTimeDelta);
	void				 UI_Fuel_minus(_float Fuel_minus) { m_fFuel -= Fuel_minus; }
	void				 UI_Fuel_plus(_float  Fuel_plus) { m_fFuel += Fuel_plus; }


};

