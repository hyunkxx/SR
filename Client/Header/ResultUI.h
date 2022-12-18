#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CResultUI : public Engine::CGameObject
{
private:
	explicit CResultUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResultUI(const CResultUI& rhs);
	virtual ~CResultUI();
public:
	static CResultUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
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
public:
private:
	void KeyInput();
	void UpdateTransform();
	void SetWinText(const _float& fDeltaTime);
	void SetLoseText(const _float& fDeltaTime);
	void RenderButton();
private: /* UI value */
	float m_fSmoothStart = 0.f;
	float m_fHideTimer = 0.f;

	bool m_bHideText = false;

	bool m_bUpdateTransform;

	CRcTex*		m_pRcTex = nullptr;

	CTexture*	m_pNonClickTexture = nullptr;
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
	const _float m_fOriginY = -100.f;
	const _float m_fDeltaY = +110.f;
private:
	bool m_bShow;
private:
	_int	m_nTitleCount = 0;
	_int	m_nTextCount = 0;

	_float	m_fTitleTimer = 0.f;
	_float	m_fTextTimer = 0.f;

	wstring m_strText[4];
	wstring m_strLose[4];
	wstring m_strBuff;

	wstring m_strCreateText = L"";
	wstring m_strDestroyText = L"";
	wstring m_strKillText = L"";
};

