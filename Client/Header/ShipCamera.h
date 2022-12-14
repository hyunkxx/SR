#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CShipCamera :
	public CCamera
{
public:
	explicit CShipCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShipCamera(const CShipCamera& rhs);
	virtual ~CShipCamera();
public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void		 Camera_Setting(_vec3	Target_Pos) {}
	void				Set_Dist(_float fDist){ m_fDist = fDist; }
public:
	void		Set_Target(CTransform* Target);
	void		Reset_Pos(void) { m_vPos = { 0, 100.f, 0 }; }
	void		Eye_Lock(_bool Lock) { m_bLock = Lock; }
private:
	void		Mouse_Fix(void);
	void		Target_Renewal(const _float& fTimeDelta);
	_float		m_fDist = 0.f;
public:
	static	CShipCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);
private:
	_vec3		m_vPos = { 0.f, 0.f, 0.f };
	_bool		m_bFix = false;
	_bool		m_bLock = false;
	_vec3		m_vLook;
	CTransform* Target_Trans = nullptr;

private:
	virtual void Free(void) override;
};

