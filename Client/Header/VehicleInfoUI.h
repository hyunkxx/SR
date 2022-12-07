#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CVehicleInfoUI : public CGameObject
{
public:
	explicit CVehicleInfoUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVehicleInfoUI();
public:
	static CVehicleInfoUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT		Add_Component(void);
	virtual void Free(void) override;
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;

	virtual const   _vec3	Get_Info(void) { return _vec3(); }
	virtual			void	Move_Info(_vec3 _Info) {}
public:
	void Set_ScaleX(const _float& fX) { m_fScaleX = fX; }
	void Set_ScaleY(const _float& fY) { m_fScaleY = fY; }
	void Set_ScaleZ(const _float& fZ) { m_fScaleZ = fZ; }

	void Set_PosX(const _float& fX) { m_fPosX = fX; }
	void Set_PosY(const _float& fY) { m_fPosY = fY; }
	void Set_PosZ(const _float& fZ) { m_fPosZ = fZ; }
public:
	void SetIndex(int nIndex) { m_nCurrentIndex = nIndex; m_bUpdateUI = true; };
private:
	void SmoothString(float fSpeed, float fDeltaTime, _tchar* dest, _tchar* src);
	void UpdateString();
private:
	bool		m_bUpdateUI = true;
	TANK_STATE	data;
	_int		m_nCurrentIndex;
	_int		m_nIndex = 0;
	_float		m_Timer = 0.f;

	_tchar		m_szTitle[32] = L"";
	_tchar		m_szTitleBuff[32] = L"";

	_tchar		m_szName[32] = L"";
	_tchar		m_szNameBuff[32] = L"";

	_tchar		m_szSpeed[32] = L"";
	_tchar		m_szSpeedBuff[32] = L"";

	_tchar		m_szMaxSpeed[32] = L"";
	_tchar		m_szMaxSpeedBuff[32] = L"";

	_tchar		m_szRotSpeed[32] = L"";
	_tchar		m_szRotSpeedBuff[32] = L"";

	_tchar		m_szDamage[32] = L"";
	_tchar		m_szDamageBuff[32] = L"";

	_tchar		m_szReloadSpeed[32] = L"";
	_tchar		m_szReloadSpeedBuff[32] = L"";

	_tchar		m_szHP[32] = L"";
	_tchar		m_szHPBuff[32] = L"";

	_tchar		m_szBulletSpeed[32] = L"";
	_tchar		m_szBulletSpeedBuff[32] = L"";


	CRcTex*		m_pRcTex = nullptr;
	CTexture*	m_pTexture = nullptr;
	CTransform*	m_pTransform = nullptr;

	_matrix  m_matProj;

	_float	m_fScaleX;
	_float	m_fScaleY;
	_float	m_fScaleZ;

	_float	m_fPosX;
	_float	m_fPosY;
	_float	m_fPosZ;
	_bool	m_bPosinOn = true;
};

