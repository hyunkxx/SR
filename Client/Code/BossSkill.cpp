#include "stdafx.h"
#include "..\Header\BossSkill.h"

#include "Export_Function.h"
#include "RushMode.h"

CBossSkill * CBossSkill::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossSkill* pInstance = new CBossSkill(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBossSkill::CBossSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CBossSkill::CBossSkill(const CBossSkill & rhs)
	: CGameObject(rhs)
{
}

CBossSkill::~CBossSkill()
{
}

_int CBossSkill::Update_Object(const _float & fTimeDelta)
{
	m_pPlayerTransform = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	m_pBossTransform = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Boss", L"Proto_TransformBody", ID_DYNAMIC));

	if (!CRushMode::GetInstance()->m_bBossBegin)
		return 0;

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	/* ÇÃ·¹ÀÌ¾î, º¸½ºÅÊÅ© */

	m_fCoolTimer += fTimeDelta;

	if (m_fCoolTimer >= m_fCoolTime)
	{
		m_fCoolTimer = 0.f;
		m_bSkillOn = true;

		//_vec3 vBossPos;
		m_pBossTransform->Get_Info(INFO_POS, &vPos);

		//vPos = { vBossPos.x, 0.f, vBossPos.y };
		vPos.y = 0.f;
		m_pTransform->Set_Pos(vPos.x, vPos.y + 25.f, vPos.z);

		m_pPlayerTransform->Get_Info(INFO_POS, &vTargetPos);

		vPos.y += 25.f;

		vTargetPos.y = 25.f;
		vDir = vTargetPos - vPos;

		/* Dir Setting */
		m_pTransform->Get_Info(INFO_RIGHT, &vRightPos);
		vRightPos = vPos + vRightPos * 50.f;
		m_pRightTransform->Set_Pos(vRightPos.x, vRightPos.y + 15.f, vRightPos.z);

		m_pTransform->Get_Info(INFO_RIGHT, &vLefttPos);
		vLefttPos = vPos + vLefttPos * -50.f;
		m_pLeftTransform->Set_Pos(vLefttPos.x, vLefttPos.y + 15.f, vLefttPos.z);
	}

	if (!m_bSkillOn)
	{
		return 0;
	}
	else
	{
		Engine::PlaySound_SR(L"minigun.mp3", AH_64A_VOICE3, 1.f);
		Engine::PlaySound_SR(L"Helicopter.mp3", AH_64A_SOUND, 1.f);
	}

	m_fReloadTime += fTimeDelta;

	_vec3 vBodyPos, vBodyLook;
	m_pTransform->Get_Info(INFO_POS, &vBodyPos);
	m_pTransform->Get_Info(INFO_LOOK, &vBodyLook);
	D3DXVec3Normalize(&vBodyLook, &vBodyLook);

	_vec3 v_LeftPos, v_LeftLook;
	m_pLeftTransform->Get_Info(INFO_POS, &v_LeftPos);
	m_pLeftTransform->Get_Info(INFO_LOOK, &v_LeftLook);
	D3DXVec3Normalize(&v_LeftLook, &v_LeftLook);

	_vec3 v_RightPos, v_LRightLook;
	m_pRightTransform->Get_Info(INFO_POS, &v_RightPos);
	m_pRightTransform->Get_Info(INFO_LOOK, &v_LRightLook);
	D3DXVec3Normalize(&v_LRightLook, &v_LRightLook);

	if (m_fReloadTime > m_fReload)
	{
		m_fReloadTime = 0.f;
		Engine::Reuse_Object(vBodyPos, vBodyLook, (_float)300.f, -30.f, m_pTransform->Get_Angle(ROT_Y), BULLET_ID::AH_64A_BULLET);

		Engine::Reuse_Object(v_LeftPos, v_LeftLook, (_float)300.f, -30.f, m_pTransform->Get_Angle(ROT_Y), BULLET_ID::AH_64A_BULLET);

		Engine::Reuse_Object(v_RightPos, v_LRightLook, (_float)300.f, -30.f, m_pTransform->Get_Angle(ROT_Y), BULLET_ID::AH_64A_BULLET);
	}

	RotationBody();
	SkillTimer(fTimeDelta);
	Set_Transform(fTimeDelta);

	Movement(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CBossSkill::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CBossSkill::Render_Object(void)
{
	if (!m_bSkillOn)
		return;

	/* Áß¾Ó */
	_matrix mat = *m_pTransformBody->Get_WorldMatrix() * *m_pTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pBody->Render(&mat);

	mat = *m_pTransformHead->Get_WorldMatrix() * *m_pTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pHead->Render(&mat);

	mat = *m_pTransformTail->Get_WorldMatrix() * *m_pTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pTail->Render(&mat);

	/* ¿À¸¥ÂÊ */
	mat = *m_pTransformBody->Get_WorldMatrix() * *m_pRightTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pBody->Render(&mat);

	mat = *m_pTransformHead->Get_WorldMatrix() * *m_pRightTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pHead->Render(&mat);

	mat = *m_pTransformTail->Get_WorldMatrix() * *m_pRightTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pTail->Render(&mat);

	/* ¿ÞÂÊ */
	mat = *m_pTransformBody->Get_WorldMatrix() * *m_pLeftTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pBody->Render(&mat);

	mat = *m_pTransformHead->Get_WorldMatrix() * *m_pLeftTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pHead->Render(&mat);

	mat = *m_pTransformTail->Get_WorldMatrix() * *m_pLeftTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pTail->Render(&mat);
}

void CBossSkill::RenderGUI(void)
{
}

HRESULT CBossSkill::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pRightTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pRightTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_right", pComponent });


	pComponent = m_pLeftTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pLeftTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_left", pComponent });


	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Body_Transform", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Head_Transform", pComponent });

	pComponent = m_pTransformTail = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformTail, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Tail_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"AH_64A_enemy_body", false);
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"AH_64A_head", false);
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pTail = CVoxel::Create(m_pGraphicDev, L"AH_64A_object", false);
	NULL_CHECK_RETURN(m_pTail, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelTail", pComponent });

	return S_OK;
}

HRESULT CBossSkill::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Rotation(ROTATION::ROT_X, D3DXToRadian(20.f));
	m_pRightTransform->Rotation(ROTATION::ROT_X, D3DXToRadian(20.f));
	m_pLeftTransform->Rotation(ROTATION::ROT_X, D3DXToRadian(20.f));

	return S_OK;
}

void CBossSkill::Set_Transform(const _float & fTimeDelta)
{
}

void CBossSkill::RotationWing(const float & fDeltaTime)
{
	if (!m_bSkillOn)
		return;

	m_pTransformHead->Rotation(ROTATION::ROT_Y, D3DXToRadian(360.f * fDeltaTime * 4.f));
	m_pTransformTail->Rotation(ROTATION::ROT_X, D3DXToRadian(360.f * fDeltaTime * 4.f));
}

void CBossSkill::RotationBody()
{
	m_pBossTransform->Get_Info(INFO_POS, &vPos);
	m_pPlayerTransform->Get_Info(INFO_POS, &vTargetPos);

	vPos.y = 0.f;
	vTargetPos.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Get_Info(INFO_LOOK, &vLook);

	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);
	fRadian = D3DXVec3Dot(&vLook, &vDir);

	m_pTransform->Rotation(ROTATION::ROT_Y, fRadian - 1.f);
	m_pRightTransform->Rotation(ROTATION::ROT_Y, fRadian - 1.f);
	m_pLeftTransform->Rotation(ROTATION::ROT_Y, fRadian - 1.f);
}

void CBossSkill::Movement(const float & fDeltaTime)
{
	if (!m_bSkillOn)
		return;

	RotationWing(fDeltaTime);

	_vec3 vLook;
	m_pTransform->Get_Info(INFO_LOOK, &vLook);

	D3DXVec3Normalize(&vDir, &vDir);
	vDir *= fDeltaTime * 100.f;
	m_pTransform->Move_Pos(&vDir);
	m_pRightTransform->Move_Pos(&vDir);
	m_pLeftTransform->Move_Pos(&vDir);
}

void CBossSkill::SkillTimer(const float& fDeltaTime)
{
	if (!m_bSkillOn)
		return;

	m_fTimer += fDeltaTime;

	if (m_fTimer >= m_fTimeOut)
	{
		m_fTimer = 0.f;
		m_bSkillOn = false;
	}
}

void CBossSkill::ShotFire()
{
}
