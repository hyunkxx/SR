#pragma once
#include "Camera.h"
class CStaticCamera : public CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticCamera(const CStaticCamera& rhs);
	virtual ~CStaticCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	virtual void		 Camera_Setting(_vec3	Target_Pos) {}
private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(const _float& fTimeDelta);

private:
	_float		m_fDistance = 10.f;
	_float		m_fSpeed = 10.f;
	_float		m_fAngle = 0.f;
	_vec3		m_vPos = { 0.f,0.f,0.f };

public:
	static	CStaticCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
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

