#pragma once
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END
class CDroneCamera :
	public CCamera
{
public:
	explicit CDroneCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDroneCamera(const CCamera& rhs);
	virtual ~CDroneCamera();
public:
	HRESULT						Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				LateUpdate_Object(void) override;
	virtual const   _vec3		Get_Info(void) { return _vec3(0.f, 0.f, 0.f); };
	virtual			void		Move_Info(_vec3 _Info) {};
	void						Reset_Pos(void);
	virtual void				Camera_Setting(_vec3	Target_Pos);
	void						Target_Setting(CTransform*	Target_Pos);
	void						Count_On(_float Count);
public:
	static	CDroneCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)(WINCX) / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);
private:
	CTransform* m_pTargetTrans = nullptr;
	_float		m_fCameraCount = 0.f;
	_float		m_fCountTime = 0.f;
	_bool		m_bCount = false;

private:
	virtual void Free(void) override;

};
