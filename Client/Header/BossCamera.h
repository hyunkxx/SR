#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CBossCamera :
	public CCamera
{
public:
	explicit CBossCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossCamera(const CBossCamera & rhs);
	virtual ~CBossCamera();

public:
	HRESULT				Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Camera_Setting(_vec3	Target_Pos)override;

public:
	void		Eye_Lock(_bool Lock) { m_bLock = Lock; }
	void		Set_Pos(_vec3 Pos) { m_vPos = Pos; }

private:
	void		Mouse_Fix(void);
	void		Target_Renewal(const _float& fTimeDelta);
	_float		m_fDist = 0.f;

public:
	static	CBossCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(50.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 500.f);

private:
	_vec3		m_vPos = { 0.f, 0.f, 0.f };
	_vec3		m_AH_64A_Pos;
	_bool		m_bFix = false;
	_bool		m_bLock = false;
	_vec3		m_vLook;
	CTransform* m_pTarget_Trans = nullptr;

	_float		m_fCamera_Count = 0.f;
	_bool		m_bUp = true;
private:
	virtual void Free(void) override;
};

