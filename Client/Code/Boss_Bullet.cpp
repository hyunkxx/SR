#include "stdafx.h"
#include "..\Header\Boss_Bullet.h"

#include "Export_Function.h"
#include "BoomEffect.h"

CBoss_Bullet::CBoss_Bullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBoss_Bullet::CBoss_Bullet(const CBoss_Bullet & rhs)
	: CGameObject(rhs)
{
}

CBoss_Bullet::~CBoss_Bullet()
{
	Free();
}

HRESULT CBoss_Bullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fScale = 20.f;
	m_stBody.fLen[x] = 3.f * m_fScale;
	m_stBody.fLen[y] = 3.f * m_fScale;
	m_stBody.fLen[z] = 3.f * m_fScale;

	CGameObject::Ready_Object();

	return S_OK;
}

_int CBoss_Bullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale * 2);


	_vec3 Move, Dir1, Dir2, Dir3;

	Move = m_vDir * m_fSpeed * fTimeDelta;

	Dir3 = Dir2 = Dir1 = Move;
	Dir1.y = 0.f;
	Dir3.x = 0.f;
	D3DXVec3Normalize(&Dir1, &Dir1);
	D3DXVec3Normalize(&Dir2, &Dir2);
	D3DXVec3Normalize(&Dir3, &Dir3);

	_float RadianX = acosf(D3DXVec3Dot(&Dir2, &Dir1));
	_float RadianY = acosf(D3DXVec3Dot(&Dir2, &Dir3));

	if (Dir2.y < Dir1.y)
		RadianX *= -1;
	if (Dir2.x < Dir3.x)
		RadianY *= -1;

	m_pTransformCom->Rotation(ROT_X, -(m_pTransformCom->Get_Angle(ROT_X)) - RadianX);
	

	m_vPos += Move;

	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale * 2);
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return 	__super::Update_Object(fTimeDelta);

}

void CBoss_Bullet::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	if (0.1f > m_vPos.y)
	{
		m_bDead = true;
		CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::BOOMER_BOOM_SMOKE);
		static_cast<CBoomEffect*>(pObject)->Set_Dead(false);
		m_vPos.y = 0.1f;
		static_cast<CBoomEffect*>(pObject)->Set_Pos(m_vPos);
		_float fShootSound = 8.f;
		Engine::PlaySound_SR(L"Boom_Sound.wav", PLAYER_SHOT_SOUND1, fShootSound);
		Engine::Get_Camera()->Shake_On();
	}
	__super::LateUpdate_Object();
}

void CBoss_Bullet::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pBUlletBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Bullet_Transform", pComponent });

	pComponent = m_pBUlletBuffer = static_cast<CVIBullet*>(Clone_Prototype(L"Proto_VIBullet"));
	NULL_CHECK_RETURN(m_pBUlletBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_VIBullet", pComponent });

	return S_OK;
}

void CBoss_Bullet::Set_ID(BULLET_ID eID)
{
	m_eID = BULLET_ID::BOSS_BULLET;
}

void CBoss_Bullet::Bullet_Setting(_vec3 vPos, _vec3 vDir, const _float fSpeed, _float fAngleX, _float AngleY)
{

	m_bDead = false;
	Reset_Trans();
	m_fAccum = 0.f;
	m_vPos = vPos;
	m_vDir = vDir;
	m_fSpeed = fSpeed;
	m_fScale = 20.f;
	m_fAngleX = fAngleX;
	m_pTransformCom->Rotation(ROT_Y, AngleY);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale * 2);
	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
}

void CBoss_Bullet::Reset_Trans(void)
{
	m_pTransformCom->Reset_Trans();
	m_stBody.vPos = { -100.f,-100.f,-100.f };
	m_stBody.vPos = { -100.f,-50.f ,-100.f };
}

CBoss_Bullet * CBoss_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_Bullet*	pInstance = new CBoss_Bullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Boss_Bullet Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_Bullet::Free(void)
{
	CGameObject::Free();
}
