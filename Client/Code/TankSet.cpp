#include "stdafx.h"
#include "..\Header\TankSet.h"

#include "Export_Function.h"
#include "Voxel.h"
#include "EffectManager.h"
#include "ShootEffect.h"
#include "Gun_Shoot_Effect.h"
#include "GameMode.h"

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
	if (m_bSmoke)
	{
		m_fSmokeCount += fTimeDelta;
		if (m_fSmokeTime <= m_fSmokeCount)
		{
			m_bSmoke = false;
			m_fSmokeCount = 0.f;
		}
	}
	m_stInfo.fReloadTime += fTimeDelta;
	m_fDT = fTimeDelta;

	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
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
	if (((Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA) || (Engine::Get_Camera_ID() == CAMERA_ID::AH_64A_CAMERA)) && !m_bDead)
	{
		if (CGameMode::GetInstance()->UseMenu() || CGameMode::GetInstance()->m_bGameEnd)
			return;

		// HP UI
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &UI_matViewF);
		m_pTextureF->Set_Texture(0);
		m_pRcTexF->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	if ((Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA) && !m_bDead)
	{
		if (CGameMode::GetInstance()->UseMenu() || CGameMode::GetInstance()->m_bGameEnd)
			return;

		// Minimap UI
		_matrix OldViewMatrix, OldProjMatrix;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMinimap_Transform->Get_WorldMatrix());
		_matrix	Minimap_ViewMatrix;
		D3DXMatrixIdentity(&Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &UI_Minimap_matProj);
		m_pMinimap_Texure->Set_Texture(0);
		m_pMinimap_RcTex->Render_Buffer();
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

		// Rader UI
		_matrix OriginViewMatrix, OriginProjMatrix;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OriginViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OriginProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRader_Transform->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &Minimap_ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &UI_Minimap_matProj);
		m_pRader_Texure->Set_Texture(0);
		m_pRader_RcTex->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OriginViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OriginProjMatrix);

	}
}

void CTankSet::Rotation_Body(ROTATION eID, _float fAngle)
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
	if (!m_stInfo.bBack)
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
	_vec3 Pos;
	m_pTransformBody->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);;
}

void CTankSet::OBB_Collision_EX(void)
{
	_vec3 Look;
	m_pTransformBody->Get_Info(INFO_LOOK, &Look);
	D3DXVec3Normalize(&Look, &Look);
	Reset_Speed();

	Move_Info(-Look * (m_stInfo.fSpeed * 1.1f) * m_fDT);

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

	pComponent = m_pRcTexF = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexF, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureF = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTextureF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Hp_Tex", pComponent });

	pComponent = m_pTransformHP_UI = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHP_UI, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_WHP2", pComponent });

	pComponent = m_pMinimap_RcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pMinimap_RcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//MiniMap UI
	pComponent = m_pMinimap_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_P_Tex"));
	NULL_CHECK_RETURN(m_pMinimap_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_P_Tex", pComponent });

	pComponent = m_pMinimap_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pMinimap_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Minimap_P", pComponent });

	// Rader
	pComponent = m_pRader_RcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRader_RcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pRader_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_Rader_Tex"));
	NULL_CHECK_RETURN(m_pRader_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_Rader_Tex", pComponent });

	pComponent = m_pRader_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pRader_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Rader", pComponent });
	return S_OK;
}

void CTankSet::Shoot_Bullet(BULLET_ID eID)
{
	_vec3 Pos, Dir, UP;


	if (eID == BULLET_ID::CANNONBALL_RELOAD || eID == BULLET_ID::MASHINE_BULLET_RELOAD)
	{
		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformHead->Get_Info(INFO_RIGHT, &Dir);
		m_pTransformHead->Get_Info(INFO_UP, &UP);
		Dir += UP;
		Pos.y += 1.f * m_fScale;
		D3DXVec3Normalize(&Dir, &Dir);
		Pos += Dir * m_stInfo.fPosinDist * m_fScale;
		Engine::Reuse_Object(Pos, Dir, (float)m_stInfo.iCannonSpeed / 10.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
	}
	else
	{
		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
		Pos.y += 2.f * m_fScale;
		D3DXVec3Normalize(&Dir, &Dir);
		Pos += Dir * m_stInfo.fPosinDist * m_fScale;
		Engine::Reuse_Object(Pos, Dir, (float)m_stInfo.iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
		m_stInfo.fReloadTime = 0.f;

		
		if (eID == BULLET_ID::MASHINE_BULLET)
		{
			Engine::StopSound(PLAYER_SHOT_SOUND1);
			Engine::PlaySound_SR(L"MACHINEGUN_FIRE.wav", PLAYER_SHOT_SOUND1, CUI_Volume::s_fShotSound);
			CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::HUMVEE_SHOOT_EFFECT);
			pObject->Set_Dead(false);
			static_cast<CGun_Shoot_Effect*>(pObject)->Set_Target(m_pTransformPosin);
		}
		else
		{
			Engine::StopSound(PLAYER_SHOT_SOUND1);
			Engine::PlaySound_SR(L"Shoot_Fire.wav", PLAYER_SHOT_SOUND1, CUI_Volume::s_fShotSound);
			CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
			pObject->Set_Dead(false);
			pObject->Set_Scale(10.f);
			static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformPosin);
			static_cast<CShootEffect*>(pObject)->Set_Dist(6.f, 0.f, 0.f);
		}
	}


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

void CTankSet::Dead_Motion(const _float & fTimeDelta)
{
	_vec3 Pos;
	m_pTransformHead->Get_Info(INFO_POS, &Pos);

	if (m_bDeadTime == 0)
	{
		m_bRock = true;
		m_stInfo.fAccum = 0.f;
		Engine::StopSound(PLAYER_SHOT_SOUND1);
		Engine::PlaySound_SR(L"Tank_Died.wav", PLAYER_SHOT_SOUND1, 0.5f);
		CGameObject* m_pEffectManager = Engine::Get_Object(L"Environment_Object", L"EffectManager");
		static_cast<CEffectManager*>(m_pEffectManager)->GetEffectPool()->UseEffect(CEffectPool::EFFECT_TYPE::EXPLOSION, Pos);
		Engine::Get_Camera()->Shake_On();
	}
	m_bDeadTime += fTimeDelta;
	m_stInfo.fAccum += fTimeDelta;

	if (0.4f < Pos.y && 0 == m_iMotionCount)
	{
		_float x = 5.f * fTimeDelta;
		_float y = (100.f * fTimeDelta) - (0.5f * 9.8f * (m_stInfo.fAccum * m_stInfo.fAccum));
		_vec3 Move = { x , y ,0.f };
		m_pTransformHead->Move_Pos(&Move);
		m_pTransformPosin->Move_Pos(&Move);
		m_pTransformHead->Rotation(ROT_X, 500 * fTimeDelta);
		m_pTransformPosin->Rotation(ROT_X, 500 * fTimeDelta);
	}
	else
	{
		m_iMotionCount++;
		Pos.y = 0.8f;
		m_pTransformHead->Set_Pos(Pos.x, Pos.y, Pos.z);
		m_pTransformPosin->Set_Pos(Pos.x, Pos.y, Pos.z);
	}
}




