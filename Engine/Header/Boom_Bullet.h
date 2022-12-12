#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CVoxel;
class CTransform;

class ENGINE_DLL CBoom_Bullet :
	public CGameObject
{
public:
	explicit CBoom_Bullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoom_Bullet(const CBoom_Bullet& rhs);
	virtual ~CBoom_Bullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
private:
	HRESULT		Add_Component(void);

private:
	CVoxel*				m_pBody = nullptr;
	CTransform*			m_pTransformCom = nullptr;
public:
	void Set_ID(BULLET_ID eID);
	BULLET_ID	Get_ID(void) { return m_eID; }
	void Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY);


private:
	BULLET_ID m_eID;

	_vec3	m_vPos;
	_vec3	m_vDir;
	_float	m_fSpeed = 0.f;
	_float	m_fAccum = 0.f;
	_float	m_fAngleX = 0.f;
	_float	m_fAngleY = 0.f;
public:
	static CBoom_Bullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

END