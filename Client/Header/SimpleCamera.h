#pragma once
#include "GameObject.h"
#include "Collisionable.h"

class CSimpleCamera : public CGameObject
{
public:
	static CSimpleCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vEye, const _vec3* vLook, const _vec3* vUp);
protected:
	explicit CSimpleCamera(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vEye, const _vec3* vLook, const _vec3* vUp);
	explicit CSimpleCamera(const CSimpleCamera& rhs);
	virtual ~CSimpleCamera();
public:
	virtual		  HRESULT	Ready_Object(
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const float& fFOV,
		const float& fAspect,
		const float& fNear,
		const float& fFar);
	virtual		  _int		Update_Object(const _float& fTimeDelta);
	virtual		  void		LateUpdate_Object(void);
	virtual		  void		Render_Object(void);
	virtual		  void		RenderGUI(void);
	virtual		  void		Free() override;
	virtual void		 Camera_Setting(_vec3	Target_Pos) {}
public:
	virtual       _bool		Get_Dead(void) { return m_bDead; }
	const _vec3				Get_Info(void) { return m_vEye; };
	void					Move_Info(_vec3 _Info) { m_vEye += _Info; };
	void					SetPosition(const _vec3& vPos) { m_vEye = m_vEye; }
	void					SetLookAt(const _vec3& vLook) { m_vAt = vLook; }
	void					SetEye(const _vec3& vEye) { m_vEye = vEye; }
private:
	_vec3 m_vEye;
	_vec3 m_vAt;
	_vec3 m_vUp;

	CAMERA_ID m_eID;
	
	_matrix m_matView;
	_matrix m_matProj;

	float m_fFOV = D3DXToRadian(60.f);
	float m_fAspect = WINCX / WINCY;
	float m_fNear = 0.1f;
	float m_fFar = 1000.f;

	const _vec3 UP{ 0.f, 1.f, 0.f };
};