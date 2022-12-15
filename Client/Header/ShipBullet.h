#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBullet;
END

class  CShipBullet :
	public CGameObject
{
public:
	explicit CShipBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShipBullet(const CShipBullet& rhs);
	virtual ~CShipBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
private:
	HRESULT		Add_Component(void);

private:
	CTransform* m_pTransformCom = nullptr;
	CVIBullet* m_pBUlletBuffer = nullptr;

public:
	void Set_ID(BULLET_ID eID);
	BULLET_ID	Get_ID(void) { return m_eID; }
	void Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY);
	void	Reset_Trans(void);

private:
	BULLET_ID m_eID;
	_vec3	m_vTargetPos;
	_vec3	m_vPos;
	_vec3	m_vDir;
	_float	m_fSpeed = 0.f;
	_float	m_fAccum = 0.f;
	_float	m_fAngleX = 0.f;
	_float	m_fAngleY = 0.f;
public:
	static CShipBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

