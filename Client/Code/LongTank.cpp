#include "stdafx.h"
#include "..\Header\LongTank.h"

#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "TankManager.h"
#include "EffectPool.h"

CLongTank::CLongTank(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTankSet(pGraphicDev)
{
	//	m_stInfo.eID = VEHICLE::TANK1;
}

CLongTank::CLongTank(const CLongTank & rhs)
	: CTankSet(rhs)
{
}

CLongTank::~CLongTank()
{
}

_int CLongTank::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	Head_Spin(fTimeDelta);
	Expect_Hit_Point(fTimeDelta);
	Posin_Shake(fTimeDelta);

	return __super::Update_Object(fTimeDelta);
}

void CLongTank::LateUpdate_Object(void)
{
	SetUp_OnTerrain();
	if (m_stInfo.fReloadTime > 2.f && !m_bReLoad)
	{
		Shoot_Bullet(BULLET_ID::CANNONBALL_RELOAD);
		_float fShootSound = 1.f;
		Engine::StopSound(PLAYER_BULLET_EXIT_SOUND1);
		Engine::PlaySound_SR(L"TANK_RELOAD.wav", PLAYER_BULLET_EXIT_SOUND1, fShootSound);
		m_bReLoad = true;
	}
		
	__super::LateUpdate_Object();
}

void CLongTank::Render_Object(void)
{
	CGameObject* pAimCamera = Engine::Get_Object(L"Environment", L"AimCamera");

	if (!static_cast<CAimCamera*>(pAimCamera)->Get_CameraOn())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
		m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
		m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
		m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());
	}

	m_pEffectPool->RenderEffect(CEffectPool::EFFECT_TYPE::EXPLOSION);
	m_pEffectPool->RenderEffect(CEffectPool::EFFECT_TYPE::FIRE);
}

void CLongTank::RenderGUI(void)
{
	ImGui::SetNextWindowSize(ImVec2(200.f, 100.f));
	ImGui::Begin("MainTank State");

	string speed = "SPEED " + to_string(m_stInfo.fReload);
	string reload = "m_fReloadTime" + to_string(m_stInfo.fReloadTime);

	ImGui::Text(speed.c_str());
	ImGui::Text(reload.c_str());

	ImGui::End();
}

HRESULT CLongTank::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	/* Voxel Mesh */
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Long_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Long_ally_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Long_ally_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	/* EffectPool */
	pComponent = m_pEffectPool = CEffectPool::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pEffectPool, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"TankEffector", pComponent });

	FAILED_CHECK_RETURN(__super::Add_Component(), E_FAIL);

	Head_Setting(_vec3(0.f, 0.7f * m_fScale, 0.f));
	Body_Setting(_vec3(0.f, 0.7f * m_fScale, 0.f));
	Posin_Setting(_vec3(0.f, 0.7f * m_fScale, 0.f));

	return S_OK;
}

HRESULT CLongTank::Ready_Object(void)
{
	const TANK_STATE& data = CTankManager::GetInstance()->GetData(VEHICLE::LONG_TANK);

	m_stInfo.fAccel_Ad = data.fAccel_Ad;
	m_stInfo.fAccel_Back = data.fAccel_Back;

	//최고속도 제한
	m_stInfo.fAccel_top_speed = data.fAccel_top_speed;
	m_stInfo.fBack_top_speed = data.fBack_top_speed;

	m_stInfo.fPosinDist = data.fPosinDist;
	m_stInfo.fReload = data.fReload;
	m_stInfo.fReloadTime = data.fReloadTime;
	m_stInfo.iCannonSpeed = data.iCannonSpeed;
	m_fScale = 1.f;
	m_stInfo.RotSpeed = data.RotSpeed;

	m_stInfo.fLowAngle = data.fLowAngle;
	m_stInfo.TopAngle = data.TopAngle;

	m_stBody.fLen[x] = 1.5f * m_fScale;
	m_stBody.fLen[y] = 0.7f * m_fScale;
	m_stBody.fLen[z] = 2.9f * m_fScale;

	m_stHead.fLen[x] = 1.f * m_fScale;
	m_stHead.fLen[y] = 1.f * m_fScale;
	m_stHead.fLen[z] = 1.9f * m_fScale;


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CGameObject::Ready_Object();
	return S_OK;
}

void CLongTank::Key_Input(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_G) == KEY_STATE::TAP)
	{
		if (m_bStart)
			m_bStart = false;
		else
			m_bStart = true;
	}
	
	if (Get_DIKeyState_Custom(DIK_P) == KEY_STATE::TAP)
	{
		_vec3 randPos = _vec3(50.f, 0.f, 30.f);
		m_pEffectPool->UseEffect(CEffectPool::EFFECT_TYPE::EXPLOSION, randPos);
	}
	if (Get_DIKeyState_Custom(DIK_I) == KEY_STATE::TAP)
	{
		_vec3 randPos = _vec3(50.f, 0.f, 50.f);
		m_pEffectPool->UseEffect(CEffectPool::EFFECT_TYPE::EXPLOSION, randPos);
	}
	if (Get_DIKeyState_Custom(DIK_O) == KEY_STATE::TAP)
	{
		_vec3 randPos = _vec3(50.f, 0.f, 70.f);
		m_pEffectPool->UseEffect(CEffectPool::EFFECT_TYPE::FIRE, randPos);
	}

	if (Get_DIMouseState(DIM_LB) & 0x80 && m_stInfo.fReloadTime > m_stInfo.fReload && m_stInfo.fSpeed == 0.f)
	{
		m_bPosinShake = true;
		Shoot_Bullet(BULLET_ID::CANNONBALL);

		_float fShootSound = 1.f;
		Engine::StopSound(PLAYER_SHOT_SOUND1);
		Engine::PlaySound_SR(L"Shoot_Fire.wav", PLAYER_SHOT_SOUND1, fShootSound);
		Engine::Get_Object(L"GameLogic", L"ShootEffect")->Set_Dead(false);
		m_bReLoad = false;
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
				Rotation_Posin(ROT_X, (_float)-m_stInfo.RotSpeed * fTimeDelta);
		}
		else
			if (m_pTransformPosin->Get_Angle(ROT_X) <= m_stInfo.fLowAngle)
				Rotation_Posin(ROT_X, (_float)m_stInfo.RotSpeed * fTimeDelta);
	}

	if (Get_DIKeyState_Custom(DIK_V) == KEY_STATE::TAP)
		Camera_Change();

	_vec3	vDir;
	m_pTransformBody->Get_Info(INFO_LOOK, &vDir);
	if (Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD && m_bStart)
	{
		if (m_stInfo.bBack)
			Plus_Back_AccelSpeed(fTimeDelta);
		else
			Plus_Advance_AccelSpeed(fTimeDelta);
	}
	else if (Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD && m_bStart)
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

	D3DXVec3Normalize(&vDir, &vDir);
	Move_Info(vDir * m_stInfo.fSpeed * fTimeDelta);
}

void CLongTank::SetUp_OnTerrain(void)
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

void CLongTank::Head_Spin(const _float & fTimeDelta)
{
	CGameObject* pTankCamera = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticCamera = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimCamera = Engine::Get_Object(L"Environment", L"AimCamera");

	if (static_cast<CTankCamera*>(pTankCamera)->Get_CameraOn())
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
	else if (static_cast<CStaticCamera*>(pStaticCamera)->Get_CameraOn())
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
	else if (static_cast<CAimCamera*>(pAimCamera)->Get_CameraOn())
	{

		_vec3 pPlayerPos, pPlayerLook;
		_vec3 Right(1.0f, 0.f, 0.f);
		m_pTransformHead->Get_Info(INFO_POS, &pPlayerPos);
		m_pTransformHead->Get_Info(INFO_LOOK, &pPlayerLook);
		pPlayerPos.y = 0.f;
		_vec3 Dir = static_cast<CCamera*>(pAimCamera)->Get_Look();
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

void CLongTank::Expect_Hit_Point(const _float & fTimeDelta)
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
		fFlyTime += fTimeDelta;
		_vec3 Move;

		Move.x = Dir.x * m_stInfo.iCannonSpeed * fTimeDelta * cosf(-fAngle);
		Move.z = Dir.z * m_stInfo.iCannonSpeed * fTimeDelta * cosf(-fAngle);
		Move.y = ((Dir.y * m_stInfo.iCannonSpeed * fTimeDelta) * sinf(-fAngle) - (0.5f * 9.8f * (fFlyTime * fFlyTime)));
		Pos.y += Move.y;
		if (Pos.y <= 0.f)
			break;
	}

	m_fHitPos.x = Dir.x * m_stInfo.iCannonSpeed * fFlyTime * cosf(-fAngle);
	m_fHitPos.z = Dir.z * m_stInfo.iCannonSpeed * fFlyTime * cosf(-fAngle);
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

_vec3 CLongTank::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = static_cast<CTransform*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

}

void CLongTank::Camera_Change(void)
{
	CGameObject* pTankCamera = Engine::Get_Object(L"Environment", L"TankCamera");
	CGameObject* pStaticCamera = Engine::Get_Object(L"Environment", L"StaticCamera");
	CGameObject* pAimCamera = Engine::Get_Object(L"Environment", L"AimCamera");
	NULL_CHECK(pTankCamera);
	NULL_CHECK(pStaticCamera);
	NULL_CHECK(pAimCamera);

	if (static_cast<CTankCamera*>(pTankCamera)->Get_CameraOn())
	{
		static_cast<CTankCamera*>(pTankCamera)->Set_Camera(false);
		static_cast<CStaticCamera*>(pStaticCamera)->Set_Camera(true);
	}
	else if (static_cast<CStaticCamera*>(pStaticCamera)->Get_CameraOn())
	{
		static_cast<CStaticCamera*>(pStaticCamera)->Set_Camera(false);
		static_cast<CAimCamera*>(pAimCamera)->Set_Camera(true);
		_float fChangeSound = 1.f;
		Engine::PlaySound_SR(L"Aim_Sound.wav", PLAYER_SHOT_SOUND1, fChangeSound);
	}
	else if (static_cast<CAimCamera*>(pAimCamera)->Get_CameraOn())
	{
		static_cast<CAimCamera*>(pAimCamera)->Set_Camera(false);
		static_cast<CTankCamera*>(pTankCamera)->Set_Camera(true);
		_float fChangeSound = 1.f;
		Engine::PlaySound_SR(L"Aim_Sound.wav", PLAYER_SHOT_SOUND1, fChangeSound);
	}
}

void CLongTank::Posin_Shake(const _float & fTimeDelta)
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
			_vec3 Pos;
			m_pTransformHead->Get_Info(INFO_POS, &Pos);
			m_pTransformPosin->Set_Pos(Pos.x, Pos.y, Pos.z);
		}
	}
}

CLongTank * CLongTank::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLongTank*		pInstance = new CLongTank(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CLongTank Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLongTank::Free(void)
{
	CGameObject::Free();
}
