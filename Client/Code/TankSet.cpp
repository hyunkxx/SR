#include "stdafx.h"
#include "..\Header\TankSet.h"

#include "Export_Function.h"
#include "Voxel.h"

CTankSet::CTankSet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTankSet::CTankSet(const CTankSet & rhs)
	: Engine::CGameObject(rhs)
{
}

CTankSet::~CTankSet()
{
}

_int CTankSet::Update_Object(const _float & fTimeDelta)
{
	m_stInfo.fReloadTime += fTimeDelta;
	m_fDT = fTimeDelta;

	m_pTransformBody->Set_Scale(m_fScale , m_fScale, m_fScale);
	m_pTransformHead->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformPosin->Set_Scale(m_fScale, m_fScale, m_fScale);
	__super::Update_Object(fTimeDelta);
	Update_OBB();

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CTankSet::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTankSet::Render_Object(void)
{
	__super::Render_Object();
}

void CTankSet::Rotation_Body(ROTATION eID,_float fAngle)
{
	m_pTransformBody->Rotation(eID, D3DXToRadian(fAngle));
}

void CTankSet::Rotation_Head(ROTATION eID, _float fAngle)
{
	m_pTransformHead->Rotation(eID, D3DXToRadian(fAngle));
}

void CTankSet::Rotation_Posin(ROTATION eID, _float fAngle)
{
	m_pTransformPosin->Rotation(eID, D3DXToRadian(fAngle));
}
_float CTankSet::Plus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	if(!m_stInfo.bBack)
		m_stInfo.bAdvance = true;

	m_stInfo.fAccum += fTimeDelta;
	m_stInfo.fSpeed = (m_stInfo.fAccel_Ad * m_fScale) * m_stInfo.fAccum;

	if (m_stInfo.fAccel_top_speed * m_fScale >= m_stInfo.fSpeed)
		return m_stInfo.fSpeed;
	else
		Minus_Advance_AccelSpeed(fTimeDelta);
		
	return m_stInfo.fSpeed;
}

_float CTankSet::Minus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	m_stInfo.fAccum -= fTimeDelta;

	if (m_stInfo.fAccum <= 0.f)
	{
		m_stInfo.fAccum = 0.f;
		m_stInfo.bAdvance = false;
	}

	return m_stInfo.fSpeed = (m_stInfo.fAccel_Ad * m_fScale) * m_stInfo.fAccum;
}

_float CTankSet::Plus_Back_AccelSpeed(const _float & fTimeDelta)
{
	m_stInfo.fAccum += fTimeDelta;

	if (m_stInfo.fAccum >= 0.f)
	{
		m_stInfo.fAccum = 0.f;
		m_stInfo.bBack = false;
	}
	
	return m_stInfo.fSpeed = (m_stInfo.fAccel_Back * m_fScale) * m_stInfo.fAccum;
}

_float CTankSet::Minus_Back_AccelSpeed(const _float & fTimeDelta)
{
	if (!m_stInfo.bAdvance)
		m_stInfo.bBack = true;

	m_stInfo.fAccum -= fTimeDelta;

	m_stInfo.fSpeed = (m_stInfo.fAccel_Back * m_fScale) * m_stInfo.fAccum;

	if (m_stInfo.fBack_top_speed * m_fScale <= m_stInfo.fSpeed)
		return m_stInfo.fSpeed;
	else
		Plus_Back_AccelSpeed(fTimeDelta);

	return m_stInfo.fSpeed;

}

void CTankSet::Free(void)
{

}


const _vec3 CTankSet::Get_Info(void)
{
	_vec3 Info;
	m_pTransformBody->Get_Info(INFO_POS, &Info);

	return Info;
}

void CTankSet::Move_Info(_vec3 _Info)
{
	m_pTransformBody->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CTankSet::OBB_Collision_EX(void)
{
	_vec3 Look;
	m_pTransformBody->Get_Info(INFO_LOOK, &Look);
	D3DXVec3Normalize(&Look, &Look);
	Reset_Speed();

	Move_Info(-Look * m_stInfo.fSpeed * m_fDT);

}

void CTankSet::Update_OBB(void)
{
	_vec3 Pos, Right, Up, Look;

	m_pTransformBody->Get_Info(INFO_POS, &m_stBody.vPos);
	m_pTransformBody->Get_Info(INFO_RIGHT, &m_stBody.vDir[x]);
	m_pTransformBody->Get_Info(INFO_UP, &m_stBody.vDir[y]);
	m_pTransformBody->Get_Info(INFO_LOOK, &m_stBody.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stBody.vDir[i], &m_stBody.vDir[i]);

	m_pTransformHead->Get_Info(INFO_POS, &m_stHead.vPos);
	m_pTransformHead->Get_Info(INFO_RIGHT, &m_stHead.vDir[x]);
	m_pTransformHead->Get_Info(INFO_UP, &m_stHead.vDir[y]);
	m_pTransformHead->Get_Info(INFO_LOOK, &m_stHead.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stHead.vDir[i], &m_stHead.vDir[i]);
}

HRESULT CTankSet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformBody", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformHead", pComponent });

	pComponent = m_pTransformPosin = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformPosin", pComponent });

	return S_OK;
}

void CTankSet::Shoot_Bullet(BULLET_ID eID)
{
	_vec3 Pos, Dir;

	m_pTransformPosin->Get_Info(INFO_POS, &Pos);
	m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
	D3DXVec3Normalize(&Dir, &Dir);
	Pos += Dir * m_stInfo.fPosinDist * m_fScale;
	Engine::Reuse_Object(Pos, Dir, (float)m_stInfo.iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
	m_stInfo.fReloadTime = 0.f;
}

void CTankSet::Posin_Setting(const _vec3 & _SetPos)
{
	m_pTransformPosin->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CTankSet::Head_Setting(const _vec3 & _SetPos)
{
	m_pTransformHead->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CTankSet::Body_Setting(const _vec3 & _SetPos)
{
	m_pTransformBody->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}




