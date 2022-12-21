#pragma once
#include "GameObject.h"
#include "Voxel.h"

BEGIN(Engine)
class CTransform;
class CRcTex;
END

class CBossSkill : public CGameObject
{
public:
	static CBossSkill* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	explicit CBossSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossSkill(const CBossSkill& rhs);
private:
	virtual ~CBossSkill();
public:
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	virtual void RenderGUI(void)override;
	virtual	HRESULT	Add_Component(void);
	virtual HRESULT	Ready_Object(void);
public:
	void SkillOn(bool value) { m_bSkillOn = value; };
private:
	//void SettupDirection();
	void SetOriginPos(const _vec3& value) { vPos = value; }
	void Set_Transform(const _float& fTimeDelta);
	void RotationWing(const float& fDeltaTime);
	void RotationBody();
	void Movement(const float& fDeltaTime);
	void SkillTimer(const float& fDeltaTime);
	void ShotFire();
private:
	CTransform*	m_pTransformBody = nullptr;
	CTransform*	m_pTransformHead = nullptr;
	CTransform*	m_pTransformTail = nullptr;

	CTransform* m_pTransform = nullptr;
	CTransform* m_pRightTransform = nullptr;
	CTransform* m_pLeftTransform = nullptr;

	_vec3 vRightPos = { 0.f,0.f,0.f };
	_vec3 vLefttPos = { 0.f,0.f,0.f };

	float fRadian = 0.f;
	_vec3 vDir;
	_vec3 vPos, vLook;
	_vec3 vTargetPos;
	//CComponent* m_pBossTransform = nullptr;
	CTransform* m_pPlayerTransform = nullptr;

	CVoxel*		m_pBody = nullptr;
	CVoxel*		m_pHead = nullptr;
	CVoxel*		m_pTail = nullptr;

	bool m_bDirSet = false;
	bool m_bSkillOn = false;

	float m_fCoolTimer = 0.f;
	const float m_fCoolTime = 8.f;

	float m_fTimer = 0.f;
	const float m_fTimeOut = 5.f;

	float m_fReloadTime = 0.f;
	const float m_fReload = 0.1f;
};

