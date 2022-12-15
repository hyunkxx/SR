#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END


class CMoonWalkCamera :
	public CCamera
{
public:
	explicit CMoonWalkCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMoonWalkCamera(const CMoonWalkCamera& rhs);
	virtual ~CMoonWalkCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
public:
	void		Camera_Setting(_vec3	Target_Pos);
	void		Reset_Pos(void) { m_vPos = { 0.f, 100.f, 0.f }; }
private:
	void		Mouse_Fix(void);
	void		Target_Renewal(const _float& fTimeDelta);

public:
	static	CMoonWalkCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	_vec3		m_vPos = { 0.f, 0.f, 0.f };
	_vec3		m_vLook;
	_vec3		Target_Pos;
	_vec3		m_vDir;
private:
	virtual void Free(void) override;
};

