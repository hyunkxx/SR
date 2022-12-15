#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Player_Hp : public Engine::CGameObject
{
private:
	explicit CUI_Player_Hp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Player_Hp(const CUI_Player_Hp& rhs);
	virtual ~CUI_Player_Hp();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Player_Hp*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;

	// front
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	//Back
	CTexture*					m_pBTexture = nullptr;
	CTransform*				m_pBTransform = nullptr;

	// Reload
	CTexture*					m_pRTexture = nullptr;
	CTransform*				m_pRTransform = nullptr;



	_matrix  m_matProj;

public:

private:


	// HP Bar (Front)
	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;

	_float					m_fOrgin_ScaleX;
	_float					m_fOrgin_PosX;

	// HP Back (²®µ¥±â)
	_float					m_fBScaleX;
	_float					m_fBScaleY;
	_float					m_fBScaleZ;

	_float					m_fBPosX;
	_float					m_fBPosY;
	_float					m_fBPosZ;
	
	// Reload (Front Bar)
	_float					m_fRScaleX;
	_float					m_fRScaleY;
	_float					m_fRScaleZ;

	_float					m_fRPosX;
	_float					m_fRPosY;
	_float					m_fRPosZ;

	_float					m_fROrgin_ScaleX;
	_float					m_fROrgin_PosX;


	_float HP_Percent;
	_float Reload_Percent;
	_bool Reload_Fire  = false;
	wstring szTankName;
	Tank_State			TankInfo;

public:
	void				Update_Tank_Info(void);

	void				 Update_HP(void);

	void				Update_Reload(void);

};

