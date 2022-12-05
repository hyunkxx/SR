#include "stdafx.h"
#include "..\Header\TestPlayer.h"

#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "Voxel.h"

CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTestPlayer::CTestPlayer(const CTestPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}

CTestPlayer::~CTestPlayer()
{
}

HRESULT CTestPlayer::Ready_Object(void)
{
	m_fScale = 3.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CTestPlayer::Update_Object(const _float& fTimeDelta)
{
	m_fReloadTime += fTimeDelta;
	Key_Input(fTimeDelta);
	Head_Spin(fTimeDelta);

	m_pTransformBody->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformHead->Set_Scale(3.f, 3.f, 3.f);
	m_pTransformPosin->Set_Scale(3.f, 3.f, 3.f);
	Expect_Hit_Point(fTimeDelta);
	__super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	
	return OBJ_NOEVENT;
}

void CTestPlayer::LateUpdate_Object(void)
{
	
	SetUp_OnTerrain();

	__super::LateUpdate_Object();
}

void CTestPlayer::Expect_Hit_Point(const _float & fTimeDelta)
{
	_vec3 Pos, Dir;

	_vec3 HitPos;
	_float fAngle;
	m_pTransformPosin->Get_Info(INFO_POS, &Pos);
	m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
	fAngle = m_pTransformPosin->Get_Angle(ROT_X);
	D3DXVec3Normalize(&Dir, &Dir);

	_float fFlyTime = 0.f;
	while (true)
	{
		fFlyTime += fTimeDelta;
		_vec3 Move;

		Move.x = Dir.x * m_iCannonSpeed * fTimeDelta * cosf(-fAngle);
		Move.z = Dir.z * m_iCannonSpeed * fTimeDelta * cosf(-fAngle);
		Move.y = ((Dir.y * m_iCannonSpeed * fTimeDelta) * sinf(-fAngle) - (0.5f * 9.8f * (fFlyTime * fFlyTime)));
		Pos.y += Move.y;
		if (Pos.y <= 0.f)
			break;
	}

	HitPos.x = Dir.x * m_iCannonSpeed * fFlyTime * cosf(-fAngle);
	HitPos.z = Dir.z * m_iCannonSpeed * fFlyTime * cosf(-fAngle);
	HitPos += Pos;
	HitPos.y = 0.f;

	m_pTransHitPoint->Set_Pos(HitPos.x, HitPos.y, HitPos.z);
}

void CTestPlayer::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	//m_pBodyBuffer->Render_Buffer();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	//m_pHeadBuffer->Render_Buffer();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	//m_pPosinBuffer->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransHitPoint->Get_WorldMatrix());
	m_pHitPointBuffer->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pHead->Render(m_pTransformBody->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pBody->Render(m_pTransformHead->Get_WorldMatrix());


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTestPlayer::RenderGUI(void)
{
	ImGui::SetNextWindowSize(ImVec2(200.f, 100.f));
	ImGui::Begin("MainTank State");

	string speed  = "m_fSpeed     " + to_string(m_fSpeed);
	string reload = "m_fReloadTime" + to_string(m_fReloadTime);
	ImGui::Text(speed.c_str());
	ImGui::Text(reload.c_str());

	ImGui::End();
}

const _vec3 CTestPlayer::Get_Info(void)
{
	_vec3 Info;
	m_pTransformBody->Get_Info(INFO_POS, &Info);

	return Info;
}

void CTestPlayer::Move_Info(_vec3 _Info)
{
	m_pTransformBody->Move_Pos(&_Info);
	m_pTransformHead->Move_Pos(&_Info);
	m_pTransformPosin->Move_Pos(&_Info);
}


void CTestPlayer::Free(void)
{
	__super::Free();
}

CTestPlayer* CTestPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestPlayer*		pInstance = new CTestPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("TestPlayer Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CTestPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBodyBuffer = static_cast<CTank_Body*>(Clone_Prototype(L"Proto_TankBody"));
	NULL_CHECK_RETURN(m_pBodyBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TankBody", pComponent });


	pComponent = m_pHeadBuffer = static_cast<CTank_Head*>(Clone_Prototype(L"Proto_TankHead"));
	NULL_CHECK_RETURN(m_pHeadBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TankHead", pComponent });

	pComponent = m_pPosinBuffer = static_cast<CTankPosin*>(Clone_Prototype(L"Proto_TankPosin"));
	NULL_CHECK_RETURN(m_pPosinBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TankPosin", pComponent });

	pComponent = m_pHitPointBuffer = static_cast<CHitPoint*>(Clone_Prototype(L"Proto_HitPoint"));
	NULL_CHECK_RETURN(m_pHitPointBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HitPoint", pComponent });


	pComponent = m_pCalculatorCom = static_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	/*pComponent = m_pTextureCom = static_cast<CTexture*>(Clone_Prototype(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });*/
	
	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformBody", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformHead", pComponent });
	_vec3 a = { 0.f,2.4f,0.f };
	//m_pTransformHead->Set_Pos(a.x, a.y, a.z);
	m_pTransformHead->Set_Pos(0.f, 0.f, 0.f);

	pComponent = m_pTransformPosin = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformPosin", pComponent });
	m_pTransformPosin->Set_Pos(a.x, a.y, a.z);

	pComponent = m_pTransHitPoint = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransHitPoint, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformHitPoint", pComponent });

	/* Voxel Mesh */
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"test");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	//pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"test_backhead");
	//NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Voxel2", pComponent });


	//Engine::Ready_KeyMgr();
	return S_OK;
}

void CTestPlayer::Key_Input(const _float& fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
		m_pTransformBody->Rotation(ROT_Y, D3DXToRadian(30.f * fTimeDelta));

	if (Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
		m_pTransformBody->Rotation(ROT_Y, D3DXToRadian(-30.f * fTimeDelta));

	if (Get_DIKeyState_Custom(DIK_E) == KEY_STATE::HOLD)
	{
		if (m_pTransformPosin->Get_Angle(ROT_X) <= 0.f)
			m_pTransformPosin->Rotation(ROT_X, D3DXToRadian(30.f * fTimeDelta));
	}

	if (Get_DIKeyState_Custom(DIK_Q) == KEY_STATE::HOLD)
	{
		if (m_pTransformPosin->Get_Angle(ROT_X) >= -0.5f)
			m_pTransformPosin->Rotation(ROT_X, D3DXToRadian(-30.f * fTimeDelta));
	}

	if (Get_DIMouseState(DIM_LB) & 0x80 && m_fReloadTime > m_fReload)
	{
		_vec3 Pos, Dir;

		m_pTransformPosin->Get_Info(INFO_POS, &Pos);
		m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
		D3DXVec3Normalize(&Dir, &Dir);
		Pos += Dir* 3.f;
		Get_Layer(L"GameLogic")->Reuse_Object(Pos, Dir, m_iCannonSpeed, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
		m_fReloadTime = 0.f;
		/*static_cast<CStaticCamera*>(m_pCameraInst)->Shake_On();*/
	}
	if (Get_DIKeyState_Custom(DIK_V) == KEY_STATE::TAP)
	{
		CGameObject* pTankCamera = Engine::Get_Object(L"Environment", L"TankCamera");
		CGameObject* pStaticCamera = Engine::Get_Object(L"Environment", L"StaticCamera");
		if (static_cast<CTankCamera*>(pTankCamera)->Get_CameraOn())
		{
			static_cast<CTankCamera*>(pTankCamera)->Set_Camera(false);
			static_cast<CStaticCamera*>(pStaticCamera)->Set_Camera(true);
		}
		else if (static_cast<CStaticCamera*>(pStaticCamera)->Get_CameraOn())
		{
			static_cast<CStaticCamera*>(pStaticCamera)->Set_Camera(false);
			static_cast<CTankCamera*>(pTankCamera)->Set_Camera(true);
		}

	}

	_vec3	vDir;
	m_pTransformBody->Get_Info(INFO_LOOK, &vDir);

	if (Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD)
	{
		if (!m_bAdvance && !m_bBack)
			m_bAdvance = true;
		// 전징상태
		if (m_bAdvance)
			Plus_Advance_AccelSpeed(fTimeDelta);

		// 후진 상태
		if (m_bBack)
		{
			Plus_Back_AccelSpeed(fTimeDelta);

			if (m_fAccum > 0.f)
			{
				m_bAdvance = true;
				m_bBack = false;
			}
		}
		// 최고 속도 제한
		if (m_fBoosterAccum > 0.f)
		{
			if (m_fSpeed > 0.15f* m_fScale && m_bAdvance)
				Minus_Advance_AccelSpeed(fTimeDelta);
		}
		else if (m_fSpeed > 0.1f * m_fScale && m_bAdvance)
			Minus_Advance_AccelSpeed(fTimeDelta);
	}
	else if (Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD)
	{
		if (!m_bAdvance && !m_bBack)
			m_bBack = true;

		if (m_bAdvance)
		{
			Minus_Advance_AccelSpeed(fTimeDelta);
			if (m_fAccum < 0.f)
			{
				m_bAdvance = false;
				m_bBack = true;
			}
		}
		if (m_bBack)
			Minus_Back_AccelSpeed(fTimeDelta);

		// 최고 속도 제한
		if (m_fSpeed < -0.05f * m_fScale && m_bBack)
			Plus_Back_AccelSpeed(fTimeDelta);
	}
	else
	{
		if (m_fAccum == 0.f)
			return;
		else if (m_fAccum > 0.f)
		{
			Minus_Advance_AccelSpeed(fTimeDelta);
			if (m_fAccum <= 0.f)
			{
				m_fAccum = 0.f;
				m_fSpeed = 0.f;
				m_bAdvance = false;
				m_bBack = false;
				return;
			}
		}
		else
		{
			Plus_Back_AccelSpeed(fTimeDelta);
			if (m_fAccum >= 0.f)
			{
				m_fAccum = 0.f;
				m_fSpeed = 0.f;
				m_bAdvance = false;
				m_bBack = false;
				return;
			}
		}
	}

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformBody->Move_Pos(&(vDir * m_fSpeed));
	m_pTransformHead->Move_Pos(&(vDir * m_fSpeed));
	m_pTransformPosin->Move_Pos(&(vDir * m_fSpeed));
}

void CTestPlayer::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformBody->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainTexCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	if (m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos()))
	{
		m_pTransformBody->Set_Pos(vPos.x, vPos.y , vPos.z);
		m_pTransformHead->Set_Pos(vPos.x, vPos.y, vPos.z);
		m_pTransformPosin->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	else
		return;
}

void CTestPlayer::Head_Spin(const _float& fTimeDelta)
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

_float CTestPlayer::Plus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	if (Get_DIKeyState_Custom(DIK_LSHIFT) == KEY_STATE::HOLD)
		m_fBoosterAccum += fTimeDelta;
	else if (m_fBoosterAccum > 0.f && Get_DIKeyState_Custom(DIK_LSHIFT) == KEY_STATE::NONE)
	{
		m_fBoosterAccum -= fTimeDelta;
	}
	else
		m_fBoosterAccum = 0.f;

	m_fAccum += fTimeDelta;

	return m_fSpeed = (m_fAccel_Ad * m_fScale) * m_fAccum + (m_fBooster * m_fScale) * m_fBoosterAccum;

}

_float CTestPlayer::Minus_Advance_AccelSpeed(const _float & fTimeDelta)
{
	if (m_fBoosterAccum > 0.f)
		m_fBoosterAccum -= fTimeDelta;
	else
		m_fBoosterAccum = 0.f;

	m_fAccum -= fTimeDelta;
	return m_fSpeed = (m_fAccel_Ad * m_fScale) * m_fAccum + (m_fBooster * m_fScale) * m_fBoosterAccum;

}

_float CTestPlayer::Plus_Back_AccelSpeed(const _float & fTimeDelta)
{
	m_fAccum += fTimeDelta;
	return m_fSpeed = m_fAccel_Back * m_fAccum;
}

_float CTestPlayer::Minus_Back_AccelSpeed(const _float & fTimeDelta)
{
	m_fAccum -= fTimeDelta;
	return m_fSpeed = m_fAccel_Back * m_fAccum;
}

_vec3 CTestPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*	pTerrainBufferCom = static_cast<CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransformCom = static_cast<CTransform*>(Engine::Get_Component(L"Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
}

