#pragma once
#include "GameObject.h"
#include "Collisionable.h"

BEGIN(Engine)

class CVIBullet;
class CTransform;
class CVIBullet_Re;

class ENGINE_DLL CBullet :
	public CGameObject , public ICollisionable
{

public:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void LateUpdate_Object(void);
	virtual void Render_Object(void);
	virtual void Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float fAngleY);
	
	//Collisionable  함수들
	virtual const   _vec3		Get_Info(void) { return m_stBody.vPos; }
	virtual			void		Move_Info(_vec3 _Info) {}
	virtual			void		OBB_Collision_EX(void) {}
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }
public:
	void Set_ID(BULLET_ID eID);
	BULLET_ID	Get_ID(void) { return m_eID; }

private:
	HRESULT		Add_Component(void);

private:
	CTransform* m_pTransform = nullptr;
	CVIBullet* m_pBUlletBuffer = nullptr;
	CVIBullet_Re* m_pBUllet_Re_Buffer = nullptr;
protected:
	_vec3	m_vPos;
	_vec3	m_vDir;
	_float	m_fSpeed = 0.f;
	_float	m_fAccum = 0.f;
	_float	m_fAngleX = 0.f;
	_float	m_fAngleY = 0.f;
	// 총알 광역 범위
	// 0 일시 충동한 오브젝트와만 상호 처리하고 0이 아닐시 HitRange * m_fScale 값으로 근처 적 타격 예정
	_float	m_fHitRange = 0.f;
	BULLET_ID m_eID;
public:
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void		Free(void);
};

END

