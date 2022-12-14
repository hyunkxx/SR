#pragma once
#include "GameObject.h"
#include "Voxel.h"
BEGIN(Engine)

class CEffectPool;
class CTransform;
class CVoxel;

END

class CEnemyTurret :public CGameObject
{
public:
	explicit CEnemyTurret(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnemyTurret(const CEnemyTurret& rhs);
	virtual ~CEnemyTurret();

public:
	virtual HRESULT		Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
public:
	void Detect(_float fTimeDelta);
	_float Dist(CTransform * _Target);
	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	virtual	HRESULT		Add_Component(void);

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
	_float Range = 90.f;
	_bool LeftCheck;
	_float m_fReloadTime = 0.f, m_fReload = 0.5f, fPosinDist = 3.f, m_iCannonSpeed = 400.f;
	_vec3* vPos = {};
	AIACTION m_Action = AIACTION::AIACTION_END;
public:
	static CEnemyTurret*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CEnemyTurret*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;
};

