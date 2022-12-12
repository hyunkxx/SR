#pragma once
#include "Camera.h"

class CAimCamera :
	public CCamera
{
public:
	explicit CAimCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAimCamera(const CAimCamera& rhs);
	virtual ~CAimCamera();
public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
private:
	void		Mouse_Move(const _float & fTimeDelta);
	void		Mouse_Fix(void);
	void		Key_Input(const _float & fTimeDelta);
	virtual void		 Camera_Setting(_vec3	Target_Pos) {}
public:
	static	CAimCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

	const _float Get_FOV(void) { return CAimCamera::m_fFov; }

private:
	_vec3	m_vLook;
private:
	virtual void Free(void) override;
};

