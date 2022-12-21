#include "stdafx.h"
#include "..\Header\Boss.h"

#include "Export_Function.h"
#include "Voxel.h"
#include "ShootEffect.h"
#include "EffectManager.h"
#include "BossHitPoint.h"
#include "Boss_Bullet.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CBoss::CBoss(const CBoss & rhs)
	: CGameObject(rhs)
{

}

CBoss::~CBoss()
{
}

_int CBoss::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if(m_bDeadEvent)
		Dead_Motion(fTimeDelta);
	
	if(m_bAppear)
		m_fShootCount += fTimeDelta;

	
	m_pTransformBody->Set_Scale(1.5f * m_fScale, m_fScale, 1.5f * m_fScale);
	m_pTransformHead->Set_Scale(1.5f * m_fScale, m_fScale, 1.5f * m_fScale);
	m_pTransformPosin->Set_Scale(1.5f * m_fScale, m_fScale, 1.5f * m_fScale);

	Pattern(fTimeDelta);
	if(!m_bPattern_4)
		Export_Hit_Point();

	if(m_bAppear && m_bHeadMove)
		Head_Spin(fTimeDelta);

	Sound_Setting(fTimeDelta);
	

	Add_RenderGroup(RENDER_NONALPHA, this);
	Update_OBB();
	return __super::Update_Object(fTimeDelta);
}

void CBoss::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CBoss::Render_Object(void)
{
	if (m_bDead)
	{
		m_pHead->Change_Color_Dead();
		m_pBody->Change_Color_Dead();
		m_pPosin->Change_Color_Dead();
	}
		
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	if (m_bDead)
	{
		m_pHead->Return_Color();
		m_pBody->Return_Color();
		m_pPosin->Return_Color();
	}

}

const _vec3 CBoss::Get_Info(void)
{
	_vec3 Info;
	m_pTransformBody->Get_Info(INFO_POS, &Info);

	return Info;
}

void CBoss::Move_Info(_vec3 _Info)
{
	m_pTransformBody->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CBoss::Update_OBB(void)
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

void CBoss::Appear(void)
{
	Engine::StopSound(BOSS_BGM);
	Engine::PlayBGM(L"Boss_BGM.mp3",BOSS_BGM);
	Posin_Setting(_vec3(100.f, 6.f * m_fScale, 400.f));
	Head_Setting(_vec3(100.f, 6.f* m_fScale, 400.f));
	Body_Setting(_vec3(100.f, 6.f* m_fScale, 400.f));
}

void CBoss::Pattern(const _float & fTimeDelta)
{
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	_vec3 PlayerPos;

	///////////////////////Appear////////////////////////////////////
	if (pPlayerTransCom)
		pPlayerTransCom->Get_Info(INFO_POS, &PlayerPos);
	else
		return;

	if (150.f <= PlayerPos.z && 0 == m_iAppearCount)
		m_bAppear = true;

	if (m_bAppear && 0 == m_iAppearCount)
	{
		Appear();
		m_bPattern_1 = true;
		m_iAppearCount++;
	}
	////////////////////Pattern_01////////////////////////////////
	if (m_bPattern_1 && 1 == m_iAppearCount)
		Pattern_01(fTimeDelta);
	// 여기까지 보스 등장 무빙
	/////////////////////Pattern_04///////////////////////////////
	// 대포 발사
	if (10.f <= m_fShootCount)
	{
		m_fShootCount = 0.f;
		m_bPattern_4 = true;
		static_cast<CBossHitPoint*>(Engine::Get_Object(L"UI", L"Boss_Hit_Point"))->Hit_Point_Set(m_vHitPoint,5.f);
	}

	if (m_bPattern_4)
	{
		m_bHeadMove = false;
		Pattern_04(fTimeDelta);
	}



}

void CBoss::Pattern_01(const _float & fTimeDelta)
{
	_vec3 vLook, vPos;
	m_pTransformBody->Get_Info(INFO_LOOK, &vLook);
	m_pTransformBody->Get_Info(INFO_POS, &vPos);

	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vDir = vLook * 20.f * fTimeDelta;

	Move_Pos(vDir);

	if (200.f >= vPos.z)
	{
		m_bPattern_1 = false;
		m_bPattern_2 = true;
	}

}

void CBoss::Pattern_02(const _float & fTimeDelta)
{
}

void CBoss::Pattern_03(const _float & fTimeDelta)
{
}

void CBoss::Pattern_04(const _float & fTimeDelta)
{
	if (5.f < m_fShootCount)
	{
		m_fShootCount = 0.f;
		Engine::StopSound(BOSS_SHOOT_SOUND);
		Engine::PlaySound_SR(L"Shoot_Bullet.wav", BOSS_SHOOT_SOUND,1.f);
		Shoot_Bullet(BULLET_ID::BOSS_BULLET);
		m_bPattern_4 = false;
		m_bHeadMove = true;
	}
}

HRESULT CBoss::Add_Component(void)
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


	// 
	/* Voxel Mesh */
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Boss_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Boss_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Boss_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	/* 동현 세팅 발사위치는 어디? */
	Posin_Setting(_vec3(200.f, 8.f * m_fScale, 200.f));
	Head_Setting(_vec3(200.f, 8.f* m_fScale, 200.f));
	Body_Setting(_vec3(200.f, 8.f* m_fScale, 200.f));

	return S_OK;
}

HRESULT CBoss::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bDead = false;
	m_stInfo.fAccel_Ad = 20.f;
	m_stInfo.fAccel_Back = 10.f;
	m_fScale = 4.f;
	//최고속도 제한
	m_stInfo.fAccel_top_speed = 40.f;
	m_stInfo.fBack_top_speed = 30.f;

	m_stInfo.fPosinDist = 5.f * m_fScale;
	m_stInfo.fReload = 1.5f;
	m_stInfo.fReloadTime = 1.5f;
	m_stInfo.iCannonSpeed = 500;
	
	m_stInfo.RotSpeed = 40.f;

	m_stBody.fLen[x] = 2.5f  * m_fScale;
	m_stBody.fLen[y] = 4.f  * m_fScale;
	m_stBody.fLen[z] = 4.5f * m_fScale;

	Posin_Setting(_vec3(300.f, 6.f * m_fScale, 400.f));
	Head_Setting(_vec3(300.f, 6.f* m_fScale, 400.f));
	Body_Setting(_vec3(300.f, 6.f* m_fScale, 400.f));
	
	Rotation_Body(ROT_Y, 180.f);
	Rotation_Head(ROT_Y, 180.f);
	Rotation_Posin(ROT_Y, 180.f);

	Update_OBB();

	

	CGameObject::Ready_Object();

	return S_OK;
}

void CBoss::Sound_Setting(const _float & fTimeDelta)
{

}

void CBoss::Shoot_Bullet(BULLET_ID eID)
{
	_vec3 Pos, Dir , NewDir;

	m_pTransformPosin->Get_Info(INFO_POS, &Pos);
	Pos.y += 8.f  * m_fScale;
	Dir = m_vHitPoint - Pos;
	NewDir  = Dir;
	NewDir.y - 0.f;
	D3DXVec3Normalize(&Dir, &Dir);
	D3DXVec3Normalize(&NewDir, &NewDir);
	_float RadianX = acosf(D3DXVec3Dot(&Dir, &NewDir));

	Pos += Dir * m_stInfo.fPosinDist * m_fScale;
	CGameObject* pBullet = Engine::Reuse_Object(Pos, Dir,500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
	static_cast<CBoss_Bullet*>(pBullet)->Bullet_Setting(Pos, Dir, 500.f, RadianX, m_pTransformPosin->Get_Angle(ROT_Y));
	
	m_stInfo.fReloadTime = 0.f;

}

void CBoss::Posin_Setting(const _vec3 & _SetPos)
{
	m_pTransformPosin->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CBoss::Head_Setting(const _vec3 & _SetPos)
{
	m_pTransformHead->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CBoss::Body_Setting(const _vec3 & _SetPos)
{
	m_pTransformBody->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CBoss::Dead_Motion(const _float & fTimeDelta)
{
	_vec3 Pos;
	m_pTransformHead->Get_Info(INFO_POS, &Pos);

	if (m_bDeadTime == 0)
	{
		m_stInfo.fAccum = 0.f;	Engine::StopSound(PLAYER_SHOT_SOUND1);
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

void CBoss::Head_Spin(const _float & fTimeDelta)
{
	CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));
	_vec3 BossPos, BossLook;
	_vec3 PlayerPos;
	_vec3 Right(1.0f, 0.f, 0.f);
	if (pPlayerTransCom)
		pPlayerTransCom->Get_Info(INFO_POS, &PlayerPos);
	else
		return;

	m_pTransformBody->Get_Info(INFO_POS, &BossPos);
	m_pTransformHead->Get_Info(INFO_LOOK, &BossLook);

	_vec3 vLook = PlayerPos - BossPos;
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&BossLook, &BossLook);

	_float	fLookDot = D3DXVec3Dot(&vLook, &Right);
	_float	fBossLookDot = D3DXVec3Dot(&BossLook, &Right);

	_float RadianDot = acosf(fLookDot);
	_float RadianBoss = acosf(fBossLookDot);


	if (Right.z > vLook.z)
		RadianDot *= -1;

	if (Right.z > BossLook.z)
		RadianBoss *= -1;

	RadianDot -= RadianBoss;
	if (RadianDot <= 0.01f && RadianDot >= -0.01f)
		return;

	if (BossLook.z > 0.f && BossLook.x > 0.f
		&&vLook.z < 0.f && vLook.x > 0.f)
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(20.f*fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(20.f*fTimeDelta));
	}
	else if (BossLook.z  < 0.f && BossLook.x > 0.f
		&&vLook.z > 0.f && vLook.x > 0.f)
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
	}
	if (BossLook.z > 0.f && BossLook.x < 0.f
		&&vLook.z < 0.f && vLook.x < 0.f)
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
	}
	else if (BossLook.z  < 0.f && BossLook.x < 0.f
		&&vLook.z > 0.f && vLook.x < 0.f)
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(20.f*fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(20.f*fTimeDelta));
	}
	else if (RadianDot > 0)
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-20.f*fTimeDelta));
	}
	else
	{
		m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(20.f *fTimeDelta));
		m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(20.f*fTimeDelta));
	}
/////////////////////////////////상하 회전/////////////////////////////////////

	_vec3 PosinLook , vPosinPos;
	m_pTransformPosin->Get_Info(INFO_LOOK, &PosinLook);
	m_pTransformPosin->Get_Info(INFO_POS, &vPosinPos);

	_vec3 Dir = PlayerPos - vPosinPos;

	_vec3 ExPosinLook = PosinLook;
	_vec3 EXDir = Dir;
	
	ExPosinLook.y = 0.f;
	EXDir.y = 0.f;

	D3DXVec3Normalize(&PosinLook, &PosinLook);
	D3DXVec3Normalize(&Dir, &Dir);
	D3DXVec3Normalize(&ExPosinLook, &ExPosinLook);
	D3DXVec3Normalize(&EXDir, &EXDir);

	_float PosinRadian = acosf(D3DXVec3Dot(&ExPosinLook, &PosinLook));
	_float DirRadian = acosf(D3DXVec3Dot(&EXDir, &Dir));
	_float Radian = DirRadian - PosinRadian;


		if(abs(Radian) >= D3DXToRadian(1.f))
	{
		if (0 <= Radian)
			m_pTransformPosin->Rotation(ROT_X, D3DXToRadian(10.f * fTimeDelta));
		else
			m_pTransformPosin->Rotation(ROT_X, D3DXToRadian(-10.f * fTimeDelta));
	}

}

void CBoss::Move_Pos(_vec3 Move)
{
	m_pTransformBody->Move_Pos(&Move);
	m_pTransformHead->Move_Pos(&Move);
	m_pTransformPosin->Move_Pos(&Move);
}

void CBoss::Export_Hit_Point(void)
{
	static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC))->Get_Info(INFO_POS, &m_vHitPoint);
}

void CBoss::Rotation_Body(ROTATION eID, _float fAngle)
{
	m_pTransformBody->Rotation(eID, D3DXToRadian(fAngle));
}

void CBoss::Rotation_Head(ROTATION eID, _float fAngle)
{
	m_pTransformHead->Rotation(eID, D3DXToRadian(fAngle));
}

void CBoss::Rotation_Posin(ROTATION eID, _float fAngle)
{
	m_pTransformPosin->Rotation(eID, D3DXToRadian(fAngle));
}

void CBoss::Free(void)
{
	CGameObject::Free();
}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss* pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBoss Create Failed");
		Safe_Release(pInstance);

	}
	return pInstance;
}
