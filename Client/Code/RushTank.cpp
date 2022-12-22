#include "stdafx.h"
#include "..\Header\RushTank.h"

#include "Export_Function.h"
#include "Voxel.h"
#include "ShootEffect.h"
#include "EffectManager.h"
#include "BattleShip_Support.h"
#include "Boom_Support.h"
#include "AH_64A.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "RushMode.h"
#include "Sakamoto.h"

CRushTank::CRushTank(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRushTank::CRushTank(const CRushTank & rhs)
	: Engine::CGameObject(rhs)
{
	m_bDead = false;
}

CRushTank::~CRushTank()
{
	
}

_int CRushTank::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Motion(fTimeDelta);

		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::LOSE)
		{
			if (Engine::Get_DIKeyState_Custom(DIK_Z) == KEY_STATE::TAP)
			{
				// 여기에 체력 셋팅 다시 해주면 됨
				CRushMode::GetInstance()->m_fPlayerCurHP = CRushMode::GetInstance()->m_fBossMaxHP;
				Re_Start();
			}
		}
	}
	else
	{
		Key_Input(fTimeDelta);
		Head_Spin(fTimeDelta);
		Expect_Hit_Point(fTimeDelta);
		Posin_Shake(fTimeDelta);
		Sound_Setting(fTimeDelta);
	}

	if (m_bGodMode)
	{
		m_fGodCount += (20.f *fTimeDelta);

		if (7 < m_fGodCount)
			m_fGodCount = 0.f;
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

void CRushTank::LateUpdate_Object(void)
{
	SetUp_OnTerrain();
	if (m_stInfo.fReloadTime > 2.f && !m_bReLoad)
	{
		Shoot_Bullet(BULLET_ID::CANNONBALL_RELOAD);

		Engine::StopSound(PLAYER_BULLET_EXIT_SOUND1);
		Engine::PlaySound_SR(L"TANK_RELOAD.wav", PLAYER_BULLET_EXIT_SOUND1, 0.5f);
		m_bReLoad = true;
	}


	__super::LateUpdate_Object();
}

void CRushTank::Render_Object(void)
{
	if (Engine::Get_Camera_ID() != CAMERA_ID::AIM_CAMERA)
	{
		if (m_bDead)
		{
			m_pHead->Change_Color_Dead();
			m_pBody->Change_Color_Dead();
			m_pPosin->Change_Color_Dead();
		}
		else if (m_bGodMode)
		{
			m_pHead->God_Mode((_int)m_fGodCount);
			m_pBody->God_Mode((_int)m_fGodCount);
			m_pPosin->God_Mode((_int)m_fGodCount);
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
		m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
		m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
		m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());
		if (m_bDead || m_bGodMode)
		{
			m_pHead->Return_Color();
			m_pBody->Return_Color();
			m_pPosin->Return_Color();
		}
		__super::Render_Object();
	}
}

const _vec3 CRushTank::Get_Info(void)
{
	_vec3 Info;
	m_pTransformBody->Get_Info(INFO_POS, &Info);

	return Info;
}

void CRushTank::Move_Info(_vec3 _Info)
{
	_vec3 Pos;
	m_pTransformBody->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}

void CRushTank::OBB_Collision_EX(void)
{
	_vec3 Look;
	m_pTransformBody->Get_Info(INFO_LOOK, &Look);
	D3DXVec3Normalize(&Look, &Look);
	Reset_Speed();

	Move_Info(-Look * (m_stInfo.fSpeed * 1.1f) * m_fDT);
}

void CRushTank::Update_OBB(void)
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

HRESULT CRushTank::Add_Component(void)
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


	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	/* Voxel Mesh */
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"M1_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"M1_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"M1_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	/* 동현 세팅 발사위치는 어디? */
	Posin_Setting(_vec3(50.f, 1.f, 50.f));
	Head_Setting(_vec3(50.f, 1.f, 50.f));
	Body_Setting(_vec3(50.f, 1.f, 50.f));

	return S_OK;

}

HRESULT CRushTank::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransformBody->Reset_Trans();
	m_pTransformHead->Reset_Trans();
	m_pTransformPosin->Reset_Trans();

	m_stInfo.fAccel_Ad = 8.f;
	m_stInfo.fAccel_Back = 6.f;

	//최고속도 제한
	m_stInfo.fAccel_top_speed = 20.f;
	m_stInfo.fBack_top_speed = -7.5f;

	m_stInfo.fPosinDist = 5.f * m_fScale;
	m_stInfo.fReload = 1.5f;
	m_stInfo.fReloadTime = 1.5f;
	m_stInfo.iCannonSpeed = 800;
	m_fScale = 1.2f;
	m_stInfo.RotSpeed = 40.f;

	m_stInfo.TopAngle = D3DXToRadian(-20.f);
	m_stInfo.fLowAngle = D3DXToRadian(5.f);

	m_stBody.fLen[x] = 2.5f  * m_fScale;
	m_stBody.fLen[y] = 4.f  * m_fScale;
	m_stBody.fLen[z] = 4.5f * m_fScale;
	
	Engine::StopSound(PLAYER_MOVE_SOUND2);
	Engine::PlaySound_SR(L"Rush_Tank_Engine.mp3", PLAYER_MOVE_SOUND2, 0.3f);

	CGameObject::Ready_Object();

	m_pTransformPosin->Rotation(ROTATION::ROT_X,D3DXToRadian(0.f));

	return S_OK;
}

void CRushTank::Sound_Setting(const _float & fTimeDelta)
{
}

void CRushTank::SetPosition(const _vec3 & vPos)
{
	m_pTransformBody->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransformHead->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransformPosin->Set_Pos(vPos.x, vPos.y, vPos.z);
}


void CRushTank::Shoot_Bullet(BULLET_ID eID)
{
	_vec3 Pos, Dir, UP;

	if (eID == BULLET_ID::CANNONBALL_RELOAD)
	{
		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformHead->Get_Info(INFO_RIGHT, &Dir);
		m_pTransformHead->Get_Info(INFO_UP, &UP);
		Dir += UP;
		Pos.y += 1.f * m_fScale;
		D3DXVec3Normalize(&Dir, &Dir);
		Pos += Dir * 3.f * m_fScale;
		Engine::Reuse_Object(Pos, Dir, (float)m_stInfo.iCannonSpeed / 10.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
	}
	else
	{
		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
		Pos.y += 2.f * m_fScale;
		D3DXVec3Normalize(&Dir, &Dir);
		Pos += Dir *3.f* m_fScale;
		Engine::Reuse_Object(Pos, Dir, (float)m_stInfo.iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), eID);
		m_stInfo.fReloadTime = 0.f;

		Engine::StopSound(PLAYER_SHOT_SOUND1);
		Engine::PlaySound_SR(L"Shoot_Fire.wav", PLAYER_SHOT_SOUND1, 0.5f);
		CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
		pObject->Set_Dead(false);
		pObject->Set_Scale(10.f);
		static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformPosin);
		static_cast<CShootEffect*>(pObject)->Set_Dist(6.f, 0.f, 0.f);
	}
}

void CRushTank::Posin_Setting(const _vec3 & _SetPos)
{
	m_pTransformPosin->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CRushTank::Head_Setting(const _vec3 & _SetPos)
{
	m_pTransformHead->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CRushTank::Body_Setting(const _vec3 & _SetPos)
{
	m_pTransformBody->Set_Pos(_SetPos.x, _SetPos.y, _SetPos.z);
}

void CRushTank::Dead_Motion(const _float & fTimeDelta)
{
	_vec3 Pos;
	m_pTransformHead->Get_Info(INFO_POS, &Pos);

	if (m_bDeadTime == 0)
	{
		m_bRock = true;
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

void CRushTank::Key_Input(const _float & fTimeDelta)
{
	_vec3	vDir;
	m_pTransformBody->Get_Info(INFO_LOOK, &vDir);

	if (!m_bRock)
	{
		// 임시 무적모드

		if (Get_DIKeyState_Custom(DIK_0) == KEY_STATE::TAP)
		{
			if (!m_bGodMode)
			{
				Engine::Get_Camera()->Set_Fov(D3DXToRadian(90.f));
				Engine::StopAll();
				static_cast<CSakamoto*>(Engine::Get_Object(L"UI", L"Sakamoto"))->Set_Sakamoto();
				m_bGodMode = true;
			}
			else
			{
				Engine::StopSound(BOSS_BGM);
				Engine::PlayBGM(L"Boss_BGM.mp3", BOSS_BGM);
				m_bGodMode = false;
				m_fGodCount = 0.f;
			}
		}

		//임시 사망 키 
		if (Get_DIKeyState_Custom(DIK_M) == KEY_STATE::TAP)
		{
			if (!m_bDead)
				m_bDead = true;
		}

		if (Get_DIKeyState_Custom(DIK_3) == KEY_STATE::TAP)
		{
			m_bRock = true;
			m_bStart = false;
			m_fEngineCount = 0.f;
			_vec3 Pos;
			m_pTransformBody->Get_Info(INFO_POS, &Pos);
			if (dynamic_cast<CBoom_Support*>(Engine::Get_Object(L"GameLogic", L"Boom_Support")))
				dynamic_cast<CBoom_Support*>(Engine::Get_Object(L"GameLogic", L"Boom_Support"))->Air_Rain(Pos);
		}
		if (Get_DIKeyState_Custom(DIK_4) == KEY_STATE::TAP)
		{

			m_bStart = false;
			m_fEngineCount = 0.f;
			_vec3 Pos;
			m_pTransformBody->Get_Info(INFO_POS, &Pos);
			if (dynamic_cast<CBattleShip_Support*>(Engine::Get_Object(L"GameLogic", L"BattleShip_Support")))
				dynamic_cast<CBattleShip_Support*>(Engine::Get_Object(L"GameLogic", L"BattleShip_Support"))->Air_Rain(Pos);
		}
		if (Get_DIKeyState_Custom(DIK_5) == KEY_STATE::TAP)
		{
			m_bRock = true;
			static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Start_AH_64A();
		}

		if (Get_DIKeyState_Custom(DIK_G) == KEY_STATE::TAP)
		{
			if (m_bStart)
				m_bStart = false;
			else
				m_bStart = true;
		}

		if (Engine::Get_DIMouseState_Custom(DIM_LB) == KEY_STATE::HOLD
			&& m_stInfo.fReloadTime > m_stInfo.fReload)
		{
			m_bPosinShake = true;
			Shoot_Bullet(BULLET_ID::BIG_CANNONBALL);
			m_bReLoad = false;
		}
		if (Get_DIKeyState_Custom(DIK_K) == KEY_STATE::TAP)
		{

		}
		if (Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
			Rotation_Body(ROT_Y, (_float)m_stInfo.RotSpeed * fTimeDelta);

		if (Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
			Rotation_Body(ROT_Y, (_float)-m_stInfo.RotSpeed * fTimeDelta);

		_long dwMouseMove = 0;

		if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
		{
			if (dwMouseMove < 0)
			{
				if (m_pTransformPosin->Get_Angle(ROT_X) >= m_stInfo.TopAngle)
					if (m_pTransformPosin->Get_Angle(ROT_X) > 0.f)
						Rotation_Posin(ROT_X, (_float)-m_stInfo.RotSpeed / 3.f * fTimeDelta);
					else
						Rotation_Posin(ROT_X, (_float)-m_stInfo.RotSpeed * fTimeDelta);
			}
			else
				if (m_pTransformPosin->Get_Angle(ROT_X) <= m_stInfo.fLowAngle)
					if (m_pTransformPosin->Get_Angle(ROT_X) > 0.f)
						Rotation_Posin(ROT_X, (_float)m_stInfo.RotSpeed / 3.f * fTimeDelta);
					else
						Rotation_Posin(ROT_X, (_float)m_stInfo.RotSpeed * fTimeDelta);
		}

		if (Get_DIKeyState_Custom(DIK_V) == KEY_STATE::TAP)
			Camera_Change();


		if (Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD)
		{
			if (m_stInfo.bBack)
				Plus_Back_AccelSpeed(fTimeDelta);
			else
				Plus_Advance_AccelSpeed(fTimeDelta);
		}
		else if (Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD)
		{
			if (m_stInfo.bAdvance)
				Minus_Advance_AccelSpeed(fTimeDelta);
			else
				Minus_Back_AccelSpeed(fTimeDelta);
		}
		else
		{
			if (m_stInfo.fAccum == 0.f)
				return;
			else if (m_stInfo.fAccum > 0.f)
				Minus_Advance_AccelSpeed(fTimeDelta);
			else
				Plus_Back_AccelSpeed(fTimeDelta);
		}
	}
	else
	{
		if (m_stInfo.fAccum == 0.f)
			return;
		else if (m_stInfo.fAccum > 0.f)
			Minus_Advance_AccelSpeed(fTimeDelta);
		else
			Plus_Back_AccelSpeed(fTimeDelta);
	}

	D3DXVec3Normalize(&vDir, &vDir);
	Move_Info(vDir * m_stInfo.fSpeed * fTimeDelta);
}

void CRushTank::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformBody->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainTexCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	if (m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos()))
	{
		m_pTransformBody->Set_Pos(vPos.x, vPos.y, vPos.z);
		m_pTransformHead->Set_Pos(vPos.x, vPos.y, vPos.z);
		m_pTransformPosin->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	else
		return;
}

void CRushTank::Head_Spin(const _float & fTimeDelta)
{
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		_vec3 MousePos = PickUp_OnTerrain();

		if (MousePos.x <= 0 || MousePos.z <= 0)
			return;

		_vec3 pPlayerPos, pPlayerLook;
		_vec3 Right(1.0f, 0.f, 0.f);
		m_pTransformHead->Get_Info(INFO_POS, &pPlayerPos);
		m_pTransformHead->Get_Info(INFO_LOOK, &pPlayerLook);
		pPlayerPos.y = 0.f;
		_vec3 Dir = MousePos - pPlayerPos;

		D3DXVec3Normalize(&Dir, &Dir);
		D3DXVec3Normalize(&pPlayerLook, &pPlayerLook);

		_float	fDot = D3DXVec3Dot(&Dir, &Right);
		_float	fLookDot = D3DXVec3Dot(&pPlayerLook, &Right);

		_float RadianDot = acosf(fDot);
		_float RadianLook = acosf(fLookDot);

		if (Right.z > Dir.z)
			RadianDot *= -1;

		if (Right.z > pPlayerLook.z)
			RadianLook *= -1;

		RadianDot -= RadianLook;
		if (RadianDot <= 0.01f && RadianDot >= -0.01f)
			return;

		if (pPlayerLook.z > 0.f && pPlayerLook.x > 0.f
			&&Dir.z < 0.f && Dir.x > 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
		else if (pPlayerLook.z  < 0.f && pPlayerLook.x > 0.f
			&&Dir.z > 0.f && Dir.x > 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		if (pPlayerLook.z > 0.f && pPlayerLook.x < 0.f
			&&Dir.z < 0.f && Dir.x < 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		else if (pPlayerLook.z  < 0.f && pPlayerLook.x < 0.f
			&&Dir.z > 0.f && Dir.x < 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
		else if (RadianDot > 0)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		else
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{

		_vec3 pPlayerPos, pPlayerLook;
		_vec3 Right(1.0f, 0.f, 0.f);
		m_pTransformHead->Get_Info(INFO_POS, &pPlayerPos);
		m_pTransformHead->Get_Info(INFO_LOOK, &pPlayerLook);
		pPlayerPos.y = 0.f;
		_vec3 Dir = Engine::Get_Camera()->Get_Look();
		// 와서 카메라 룩벡터 방향과 포신 방향 비교해서 움직이게 하기

		D3DXVec3Normalize(&Dir, &Dir);
		D3DXVec3Normalize(&pPlayerLook, &pPlayerLook);

		_float	fDot = D3DXVec3Dot(&Dir, &Right);
		_float	fLookDot = D3DXVec3Dot(&pPlayerLook, &Right);

		_float RadianDot = acosf(fDot);
		_float RadianLook = acosf(fLookDot);

		if (Right.z > Dir.z)
			RadianDot *= -1;

		if (Right.z > pPlayerLook.z)
			RadianLook *= -1;

		RadianDot -= RadianLook;
		if (RadianDot <= 0.01f && RadianDot >= -0.01f)
			return;

		if (pPlayerLook.z > 0.f && pPlayerLook.x > 0.f
			&&Dir.z < 0.f && Dir.x > 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
		else if (pPlayerLook.z  < 0.f && pPlayerLook.x > 0.f
			&&Dir.z > 0.f && Dir.x > 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		if (pPlayerLook.z > 0.f && pPlayerLook.x < 0.f
			&&Dir.z < 0.f && Dir.x < 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		else if (pPlayerLook.z  < 0.f && pPlayerLook.x < 0.f
			&&Dir.z > 0.f && Dir.x < 0.f)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
		else if (RadianDot > 0)
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(-50.f*fTimeDelta));
		}
		else
		{
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(50.f*fTimeDelta));
		}
	}
}

void CRushTank::Expect_Hit_Point(const _float & fTimeDelta)
{
	_vec3 Pos, Dir;

	m_pTransformPosin->Get_Info(INFO_POS, &Pos);
	m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
	_float fAngle = m_pTransformPosin->Get_Angle(ROT_X);
	D3DXVec3Normalize(&Dir, &Dir);
	Pos.y += 2.f * m_fScale;
	_float fFlyTime = 0.f;
	while (true)
	{
		fFlyTime += 0.01f;
		_vec3 Move;

		Move.x = Dir.x * m_stInfo.iCannonSpeed * 0.01f;
		Move.z = Dir.z * m_stInfo.iCannonSpeed * 0.01f;

		if (m_pTransformPosin->Get_Angle(ROT_X) > 0.f)
			Move.y = ((Dir.y * m_stInfo.iCannonSpeed * 0.01f) * sinf(fAngle) - (0.5f * 9.8f * (fFlyTime * fFlyTime)));
		else
			Move.y = ((Dir.y * m_stInfo.iCannonSpeed * 0.01f) * sinf(-fAngle) - (0.5f * 9.8f * (fFlyTime * fFlyTime)));

		Pos.y += Move.y;
		if (Pos.y <= 0.f)
			break;
	}

	m_fHitPos.x = Dir.x * m_stInfo.iCannonSpeed * fFlyTime;
	m_fHitPos.z = Dir.z * m_stInfo.iCannonSpeed * fFlyTime;
	m_fHitPos += Pos;
	m_fHitPos.y = 0.f;

	_matrix View, Proj;
	_vec3 HitPos;
	HitPos = m_fHitPos;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &View);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &Proj);

	D3DXVec3TransformCoord(&HitPos, &HitPos, &View);
	D3DXVec3TransformCoord(&HitPos, &HitPos, &Proj);

	HitPos.x = (HitPos.x + 1.f) * (0.5f * WINCX);
	HitPos.y = (HitPos.y - 1.f) * (-0.5f * WINCY);

	CComponent* AimTrans = Engine::Get_Component(L"UI", L"Aim_UI", L"Proto_Transform", ID_DYNAMIC);
	dynamic_cast<CTransform*>(AimTrans)->Set_Pos(HitPos.x - WINCX * 0.5f, -HitPos.y + WINCY * 0.5f, 0.f);

	AimTrans = Engine::Get_Component(L"UI", L"Aim_UI_Pers", L"Proto_Transform", ID_DYNAMIC);
	dynamic_cast<CTransform*>(AimTrans)->Set_Pos(m_fHitPos.x, m_fHitPos.y, m_fHitPos.z);
}

_vec3 CRushTank::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = static_cast<CTransform*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

}

void CRushTank::Camera_Change(void)
{
	if (Get_DIMouseState_Custom(DIM_RB) == KEY_STATE::TAP)
	{
		if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
		{
			Engine::Camera_Change(L"AimCamera");
			Engine::Get_Camera()->Camera_Setting(_vec3{ 0.f,0.f,0.f });
		}
		else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
			Engine::Camera_Change(L"TankCamera");

		Engine::StopSound(CAMERA_CHANGE_SOUND);
		Engine::PlaySound_SR(L"Aim_Sound.wav", CAMERA_CHANGE_SOUND, 1.f);
	}
}

void CRushTank::Posin_Shake(const _float & fTimeDelta)
{
	if (m_bPosinShake)
	{
		m_fPosinAccum += fTimeDelta;
		_vec3 Dir;
		m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);

		D3DXVec3Normalize(&Dir, &Dir);
		m_pTransformPosin->Move_Pos(&(Dir * sin(360.f * m_fPosinAccum * fTimeDelta) * m_fScale / 3.f));
		if (m_fPosinAccum >= 0.1f)
		{
			m_fPosinAccum = 0.f;
			m_bPosinShake = false;
			_vec3 Pos, Pos2;
			m_pTransformHead->Get_Info(INFO_POS, &Pos2);
			m_pTransformBody->Get_Info(INFO_POS, &Pos);
			m_pTransformHead->Set_Pos(Pos.x, Pos2.y, Pos.z);
			m_pTransformPosin->Set_Pos(Pos.x, Pos2.y, Pos.z);
		}
	}
}

void CRushTank::Rotation_Body(ROTATION eID, _float fAngle)
{
	m_pTransformBody->Rotation(eID, D3DXToRadian(fAngle));
}

void CRushTank::Rotation_Head(ROTATION eID, _float fAngle)
{
	m_pTransformHead->Rotation(eID, D3DXToRadian(fAngle));
}

void CRushTank::Rotation_Posin(ROTATION eID, _float fAngle)
{
	m_pTransformPosin->Rotation(eID, D3DXToRadian(fAngle));
}

_float CRushTank::Plus_Advance_AccelSpeed(const _float & fTimeDelta)
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

_float CRushTank::Minus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	m_stInfo.fAccum -= fTimeDelta;

	if (m_stInfo.fAccum <= 0.f)
	{
		m_stInfo.fAccum = 0.f;
		m_stInfo.bAdvance = false;
	}

	return m_stInfo.fSpeed = (m_stInfo.fAccel_Ad * m_fScale) * m_stInfo.fAccum;
}

_float CRushTank::Plus_Back_AccelSpeed(const _float & fTimeDelta)
{
	m_stInfo.fAccum += fTimeDelta;

	if (m_stInfo.fAccum >= 0.f)
	{
		m_stInfo.fAccum = 0.f;
		m_stInfo.bBack = false;
	}

	return m_stInfo.fSpeed = (m_stInfo.fAccel_Back * m_fScale) * m_stInfo.fAccum;
}

_float CRushTank::Minus_Back_AccelSpeed(const _float & fTimeDelta)
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

void CRushTank::Free(void)
{
	CGameObject::Free();
}

CRushTank * CRushTank::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRushTank* pInstance = new CRushTank(pGraphicDev);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CRushTank Create Failed");
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CRushTank::Re_Start(void)
{
	m_pTransformBody->Reset_Trans();
	m_pTransformHead->Reset_Trans();
	m_pTransformPosin->Reset_Trans();
	Posin_Setting(_vec3(250.f, 2.f, 200.f));
	Head_Setting(_vec3(250.f, 2.f, 200.f));
	Body_Setting(_vec3(250.f, 2.f, 200.f));
	m_bDead = false;
	m_bRock = false;
}