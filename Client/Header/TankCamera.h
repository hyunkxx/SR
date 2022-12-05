#pragma once
#include "Camera.h"

class CTankCamera :
	public CCamera
{
public:
	explicit CTankCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTankCamera(const CTankCamera& rhs);
	virtual ~CTankCamera();
public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Mouse_Fix(void);
private:
	_bool		m_bFix = false;
	_bool		m_bCheck = false;
	_vec3		m_vLook;
public:
	static	CTankCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);
	
private:
	virtual void Free(void) override;
};

