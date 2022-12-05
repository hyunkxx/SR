#pragma once
#include "GameObject.h"
#include "Voxel.h"
#include "Collisionable.h"
BEGIN(Engine)
class CTransform;
END

class CTankSet :
	public CGameObject , public ICollisionable
{
protected:
	explicit CTankSet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTankSet(const CTankSet& rhs);
	virtual ~CTankSet();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;


	virtual const _vec3	Get_Info(void)override;
	virtual	void		Move_Info(_vec3 _Info)override;
	virtual			void		RenderGUI(void)override{}
	virtual			void		OBB_Collision_EX(void);
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }

protected:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void)PURE;
	virtual void        Shoot_Bullet(BULLET_ID eID);
	void				Posin_Setting(const _vec3& _SetPos);
	void				Head_Setting(const _vec3& _SetPos);
	void				Body_Setting(const _vec3& _SetPos);

private:
	void				Reset_Speed(void) { m_stInfo.fAccum = 0.f; }

public:
	void				Rotation_Body(ROTATION eID, _float fAngle);
	void				Rotation_Head(ROTATION eID, _float fAngle);
	void				Rotation_Posin(ROTATION eID,_float fAngle);

	_float				Plus_Advance_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Advance_AccelSpeed(const _float& fTimeDelta);

	_float				Plus_Back_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Back_AccelSpeed(const _float& fTimeDelta);

	const _vec3				Get_HitPos(void) { return m_fHitPos; }
protected:
	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	Tank_State			m_stInfo;

	//Voxel Ãß°¡
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	_float				m_fDT = 0.f;

	_vec3				m_fHitPos;
//

protected:
	virtual void Free(void) override;
};