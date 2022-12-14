#include "stdafx.h"
#include "..\Header\Humvee.h"

#include "Export_Function.h"
#include "TankManager.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "Boom_Support.h"
#include "AH_64A.h"
#include "BattleShip_Support.h"
#include "GameMode.h"

CHumvee::CHumvee(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTankSet(pGraphicDev)
{
	m_bDead = false;
	m_stInfo.eID = VEHICLE::HUMVEE;
}

CHumvee::CHumvee(const CHumvee & rhs)
	: CTankSet(rhs)
{
}

CHumvee::~CHumvee()
{
	CGameMode::GetInstance()->m_nKillCount++;
}

_int CHumvee::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Motion(fTimeDelta);
	}
	else
	{
		Key_Input(fTimeDelta);
		Head_Spin(fTimeDelta);
		Expect_Hit_Point(fTimeDelta);
		Posin_Shake(fTimeDelta);
		Sound_Setting(fTimeDelta);
		Update_UI();
		Update_Minimap();
	}
	return __super::Update_Object(fTimeDelta);
}

void CHumvee::LateUpdate_Object(void)
{
	SetUp_OnTerrain();

	__super::LateUpdate_Object();
}

void CHumvee::Render_Object(void)
{
	if (Engine::Get_Camera_ID() != CAMERA_ID::AIM_CAMERA)
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
		__super::Render_Object();
	}
}

void CHumvee::RenderGUI(void)
{
	//ImGui::SetNextWindowSize(ImVec2(200.f, 100.f));
	//ImGui::Begin("MainTank State");

	//string speed = "m_fSpeed     " + to_string(m_stInfo.fSpeed);
	//string reload = "m_fReloadTime" + to_string(m_stInfo.fReloadTime);
	//ImGui::Text(speed.c_str());
	//ImGui::Text(reload.c_str());

	//ImGui::End();
}

void CHumvee::Sound_Setting(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_fEngineCount += fTimeDelta;
		if (0.7f  < m_fEngineCount && 5.f > m_fEngineCount)
		{

			Engine::StopSound(PLAYER_ENGINE_BGM);
			Engine::PlaySound_SR(L"Humvee_Engine.wav", PLAYER_ENGINE_BGM, CUI_Volume::s_fBGMSound);
			m_fEngineCount = -4.f;
		}
	}
	else
	{
		Engine::StopSound(PLAYER_ENGINE_BGM);
		m_fEngineCount = 0.f;
	}
}

HRESULT CHumvee::Add_Component(void)
{

	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	/* Voxel Mesh */
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Humvee_ally_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	FAILED_CHECK_RETURN(__super::Add_Component(), E_FAIL);

	//Posin_Setting(_vec3(0.f, 8.5f, -0.3f));
	//Head_Setting(_vec3(0.f, 7.5f, 0.f));
	//Body_Setting(_vec3(0.f, 4.f, -0.3f));

	/* ???? ???? ?????????? ????? */
	Posin_Setting(_vec3(50.f, 1.7f, 50.f));
	Head_Setting(_vec3(50.f, 1.7f, 50.f));
	Body_Setting(_vec3(50.f, 1.7f, 50.f));


	return S_OK;
}

HRESULT CHumvee::Ready_Object(void)
{
	const TANK_STATE& tankData = CTankManager::GetInstance()->GetData(VEHICLE::HUMVEE);

	m_stInfo.fAccel_Ad = tankData.fAccel_Ad;
	m_stInfo.fAccel_Back = tankData.fAccel_Back;


	//???????? ????
	m_stInfo.fAccel_top_speed = tankData.fAccel_top_speed;
	m_stInfo.fBack_top_speed = tankData.fBack_top_speed;

	m_stInfo.fPosinDist = tankData.fPosinDist;
	m_stInfo.fReload = tankData.fReload;

	m_stInfo.fReloadTime = tankData.fReloadTime;
	m_stInfo.iCannonSpeed = tankData.iCannonSpeed;
	m_fScale = 1.f;
	m_stInfo.RotSpeed = tankData.RotSpeed;

	m_stInfo.fLowAngle = tankData.fLowAngle;
	m_stInfo.TopAngle = tankData.TopAngle;




	//UI_HP
	m_stInfo.fCurHP = m_stInfo.fMaxHP = tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 4.f;
	UI_fScaleY = 0.9f;
	UI_fScaleZ = 1.f;

	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 4.f;
	m_fMinimap[SCALEZ] = 1.f;

	// UI_Rader
	m_fRader = 6.f;

	m_stBody.fLen[x] = 2.5f  * m_fScale;
	m_stBody.fLen[y] = 4.f  * m_fScale;
	m_stBody.fLen[z] = 4.5f * m_fScale;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);
	m_pRader_Transform->Set_Scale(m_fRader, m_fRader, m_fMinimap[SCALEZ]);

	CGameObject::Ready_Object();
	return S_OK;
}

void CHumvee::Key_Input(const _float & fTimeDelta)
{
	_vec3	vDir;
	m_pTransformBody->Get_Info(INFO_LOOK, &vDir);

	if (!m_bRock)
	{
		//???? ???? ?? 
		if (Get_DIKeyState_Custom(DIK_M) == KEY_STATE::TAP)
		{
			if (!m_bDead)
				m_bDead = true;
		}
		if (Get_DIMouseState_Custom(DIM_LB) == KEY_STATE::HOLD
			&& !CTankManager::GetInstance()->IsLock()
			&& m_stInfo.fReloadTime > m_stInfo.fReload)
		{
			//m_bPosinShake = true;
			Shoot_Bullet(BULLET_ID::MASHINE_BULLET);
			Shoot_Bullet(BULLET_ID::MASHINE_BULLET_RELOAD);
		}
		if (Get_DIKeyState_Custom(DIK_3) == KEY_STATE::TAP)
		{
			if (CGameMode::GetInstance()->m_nGold[0] <= 500)
				return;

			CGameMode::GetInstance()->m_nGold[0] -= 500;

			CTankManager::GetInstance()->MouseLBTLock(true);

			m_bStart = false;
			m_fEngineCount = 0.f;
			_vec3 Pos;
			m_pTransformBody->Get_Info(INFO_POS, &Pos);
			if (dynamic_cast<CBoom_Support*>(Engine::Get_Object(L"GameLogic", L"Boom_Support")))
				dynamic_cast<CBoom_Support*>(Engine::Get_Object(L"GameLogic", L"Boom_Support"))->Air_Rain(Pos);
		}
		if (Get_DIKeyState_Custom(DIK_4) == KEY_STATE::TAP)
		{
			if (CGameMode::GetInstance()->m_nGold[0] <= 500)
				return;

			CGameMode::GetInstance()->m_nGold[0] -= 500;

			m_bStart = false;
			m_fEngineCount = 0.f;
			_vec3 Pos;
			m_pTransformBody->Get_Info(INFO_POS, &Pos);
			if (dynamic_cast<CBattleShip_Support*>(Engine::Get_Object(L"GameLogic", L"BattleShip_Support")))
				dynamic_cast<CBattleShip_Support*>(Engine::Get_Object(L"GameLogic", L"BattleShip_Support"))->Air_Rain(Pos);
		}
		if (Get_DIKeyState_Custom(DIK_5) == KEY_STATE::TAP)
		{
			if (CGameMode::GetInstance()->m_nGold[0] <= 500)
				return;

			CGameMode::GetInstance()->m_nGold[0] -= 500;

			m_bRock = true;
			static_cast<CAH_64A*>(Engine::Get_Object(L"GameLogic", L"AH_64A"))->Start_AH_64A();
		}
		if (Get_DIKeyState_Custom(DIK_G) == KEY_STATE::TAP)
		{
			if (!m_bStart)
			{
				m_bStart = true;
				Engine::PlaySound_SR(L"Start_the_Tank.wav", PLAYER_MOVE_SOUND2, CUI_Volume::s_fShotSound);
			}
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

void CHumvee::SetUp_OnTerrain(void)
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

void CHumvee::Head_Spin(const _float & fTimeDelta)
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
		// ???? ?????? ?????? ?????? ???? ???? ???????? ???????? ????

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

void CHumvee::Expect_Hit_Point(const _float & fTimeDelta)
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

_vec3 CHumvee::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = static_cast<CTransform*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}
void CHumvee::Camera_Change(void)
{
	if (Get_DIKeyState_Custom(DIK_V) == KEY_STATE::TAP)
	{
		if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
		{
			Engine::Camera_Change(L"AimCamera");
			Engine::Get_Camera()->Camera_Setting(_vec3{ 0.f,0.f,0.f });
		}
		else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
			Engine::Camera_Change(L"TankCamera");

		Engine::StopSound(CAMERA_CHANGE_SOUND);
		Engine::PlaySound_SR(L"Aim_Sound.wav", CAMERA_CHANGE_SOUND, CUI_Volume::s_fBGMSound);
	}
}

void CHumvee::Posin_Shake(const _float & fTimeDelta)
{
	if (m_bPosinShake)
	{
		m_fPosinAccum += fTimeDelta;
		_vec3 Dir;
		m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);

		D3DXVec3Normalize(&Dir, &Dir);
		m_pTransformPosin->Move_Pos(&(Dir * sin(360.f * m_fPosinAccum * fTimeDelta) * m_fScale / 3.f));
		if (m_fPosinAccum >= 0.05f)
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

CHumvee * CHumvee::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHumvee*		pInstance = new CHumvee(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Humvee Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHumvee::Free(void)
{
	Safe_Release(m_pMinimap_RcTex);
	Safe_Release(m_pMinimap_Texure);
	Safe_Release(m_pMinimap_Transform);
	Safe_Release(m_pRader_RcTex);
	Safe_Release(m_pRader_Texure);
	Safe_Release(m_pRader_Transform);
	CGameObject::Free();
}

void CHumvee::Update_UI(void)
{
	_vec3 vTankPos, vUI_HPF, vUI_HPB;




	if (m_stInfo.fCurHP >= m_stInfo.fMaxHP)
	{
		m_stInfo.fCurHP = m_stInfo.fMaxHP;
	}
	if (m_stInfo.fCurHP <= 0.f)
	{
		m_stInfo.fCurHP = 0.f;
	}

	_float HP_Percent = (m_stInfo.fCurHP / m_stInfo.fMaxHP);

	if (HP_Percent > 1.f)
	{
		HP_Percent = 1.f;
	}
	if (HP_Percent < 0.f)
	{
		HP_Percent = 0.f;
	}

	UI_fScaleX = (UI_fOrgin_ScaleX * HP_Percent);


	m_pTransformBody->Get_Info(INFO_POS, &vTankPos);



	D3DXMatrixIdentity(&UI_matViewF);

	Engine::Get_Camera(L"TankCamera")->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);

	memset(&UI_matViewF._41, 0, sizeof(_vec3));

	D3DXMatrixInverse(&UI_matViewF, 0, &UI_matViewF);



	_float fScale[ROT_END];
	//???? ????


	vUI_HPF = { vTankPos.x, vTankPos.y + 3.5f, vTankPos.z + 3.f };

	fScale[ROT_X] = UI_fScaleX;
	fScale[ROT_Y] = UI_fScaleY;
	fScale[ROT_Z] = UI_fScaleZ;

	_vec3 BillPos = vUI_HPF;

	memcpy(&UI_matViewF._41, &BillPos, sizeof(_vec3));

	for (int i = 0; i < ROT_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			UI_matViewF(i, j) *= fScale[i];
		}
	}
}

void CHumvee::Update_Minimap(void)
{
	m_fRader = m_fRader + 0.6f;

	if (m_fRader >= 30.f)
	{
		m_fRader = 6.f;
	}

	_vec3 vTankPos;

	m_pTransformBody->Get_Info(INFO::INFO_POS, &vTankPos);

	_float fX_Percent = (roundf(vTankPos.x) / 635.f);
	_float fZ_Percent = (roundf(vTankPos.z) / 635.f);

	if (fX_Percent <= 0.f)
	{
		fX_Percent = 0.f;
	}
	else if (fX_Percent >= 1.f)
	{
		fX_Percent = 1.f;
	}

	if (fZ_Percent <= 0.f)
	{
		fZ_Percent = 0.f;
	}
	else if (fZ_Percent >= 1.f)
	{
		fZ_Percent = 1.f;
	}

	m_fMinimap[POSX] = 640.f + roundf(160.f * fX_Percent);
	m_fMinimap[POSY] = 600.f - roundf(115.f * fZ_Percent);
	m_fMinimap[POSZ] = 0.03f;

	// Minimap _ Pos
	m_pMinimap_Transform->Set_Pos(m_fMinimap[POSX] - (WINCX * 0.5f), (WINCY * 0.5f) - m_fMinimap[POSY], m_fMinimap[POSZ]);

	// Minimap _ Rader
	m_pRader_Transform->Set_Scale(m_fRader, m_fRader, 0.1f);
	m_pRader_Transform->Set_Pos(m_fMinimap[POSX] - (WINCX * 0.5f), (WINCY * 0.5f) - m_fMinimap[POSY], 0.1f);
}
