#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CWarningUI : public CGameObject
{
public:
	enum class MODE { WAR, RUSH };
public:
	explicit CWarningUI(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode);
	explicit CWarningUI(const CWarningUI& rhs);
	virtual ~CWarningUI();
public:
	static CWarningUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev, MODE eMode);
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const _vec3	Get_Info(void) { return _vec3(); }
	virtual		  void	Move_Info(_vec3 _Info) {}
private:
	virtual void Free(void) override;
	HRESULT		Add_Component(void);
public:
	void Set_ScaleX(const _float& fX) { m_fScaleX = fX; m_bUpdateTransform = true; }
	void Set_ScaleY(const _float& fY) { m_fScaleY = fY; m_bUpdateTransform = true; }
	void Set_ScaleZ(const _float& fZ) { m_fScaleZ = fZ; m_bUpdateTransform = true; }

	void Set_PosX(const _float& fX) { m_fPosX = fX; m_bUpdateTransform = true; }
	void Set_PosY(const _float& fY) { m_fPosY = fY; m_bUpdateTransform = true; }
	void Set_PosZ(const _float& fZ) { m_fPosZ = fZ; m_bUpdateTransform = true; }
private:
	MODE m_eMode;
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

	_int	iRotaion;
private:
	CGameObject* m_pPlayer;

	bool m_bUpdateTransform;

	bool m_bTrigger = false;
	float m_fTimer = 0.f;
	const float m_fTimeOut = 10.f;

	bool m_bHide = true;
	float m_fHideTimer = 0.f;
	const float m_fHideTimeOut = 0.5f;
};

