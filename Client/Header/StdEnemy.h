#pragma once
#include "GameObject.h"
#include "Voxel.h"
#include "Collisionable.h"
BEGIN(Engine)

class CEffectPool;
class CTransform;
class CVoxel;

END

class CStdEnemy :public CGameObject, public ICollisionable
{
public:
	explicit CStdEnemy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStdEnemy(const CStdEnemy& rhs);
	virtual ~CStdEnemy();

public:
	virtual HRESULT		Ready_Object(void) override;
	HRESULT				Ready_Object(void* pArg);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual const   _vec3		Get_Info(void)			override;
	virtual			void		Move_Info(_vec3 _Info)	override;
	virtual			void		OBB_Collision_EX(void)	override;
	virtual			void		Update_OBB(void)		override;
	virtual			OBB*        Get_OBB(void)			override;
	void  Set_DeadMotionPlay() { Deadtest = true; }
public:
	void				Detect(_float fTimeDelta);
	_float				Dist(CTransform * _Target);
	_bool				Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	void Minus_HP_UI(_float HP_minus) { fCurHp -= HP_minus; }
	void Plus_HP_UI(_float HP_plus) { fCurHp += HP_plus; }
	_float GetHp(void) { return fCurHp; }
	virtual	HRESULT		Add_Component(void);
	void Dead_Motion(const _float & fTimeDelta);
private:
	CTransform* m_pTransformCom = nullptr;
	CTransform* m_pTransformHead = nullptr;
	CTransform* m_pTransformPosin = nullptr;

	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
private:
	_float m_fTime = 0.f;
	_bool  m_bTurn = false;
	_float Range = 70.f;
	_bool LeftCheck;
	_vec3* vPos = {};
	_float fAccum = 0.f;
	_int m_iMotionCount = 0;
	_bool DeadMotionCheck = false;
	_bool   Deadtest = false;
	AIACTION m_Action = AIACTION::AIACTION_END;
	EData*      m_EData;
	BULLET_ID Temp;
	_float m_fMaxHp, fCurHp, fPosinDist;
	_float m_fReloadTime, m_fReload;
	_int   m_iCannonSpeed, TempBullet;
public:
	static CStdEnemy*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStdEnemy*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;
};
