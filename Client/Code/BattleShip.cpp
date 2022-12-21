#include "stdafx.h"
#include "..\Header\BattleShip.h"

#include "Export_Function.h"
#include "UI_Volume.h"
#include "TankSet.h"
#include "ShootEffect.h"
#include "DroneCamera.h"
#include "ShipCamera.h"
#include "BattleShip_Support.h"

CBattleShip::CBattleShip(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBattleShip::CBattleShip(const CBattleShip & rhs)
	: CGameObject(rhs)
{
}

CBattleShip::~CBattleShip()
{
}

_int CBattleShip::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return 0;
	m_fMoveTime += fTimeDelta;

	m_pTransformSee->Set_Scale(1.2f, 1.f, 2.f);
	m_pTransformSee->Set_Pos(-800.f, 0.1f, -200.f);

	if (!m_bShootReady && m_fMoveTime > 1.f)
	{
		Engine::PlaySound_SR(L"Ship_Horn.mp3", SoundType::SHIP_ENGINE_SOUND3, 1.f);
		Engine::PlaySound_SR(L"Hamsun_BGM.mp3", SoundType::SHIP_ENGINE_SOUND5, 0.2f);
	}

	if (!m_bShootReady && m_fMoveTime  > 2.f)
		Engine::PlaySound_SR(L"Ship_Bell.mp3", SoundType::SHIP_ENGINE_SOUND4, 0.7f);
	if (m_fMoveTime  > 10.f)
		Engine::StopSound(SHIP_ENGINE_SOUND4);

	if (!m_bShootReady && m_fMoveTime > 10.f)
	{
		static_cast<CShipCamera*>(Engine::Get_Camera())->Set_Dist(100.f);

		Engine::StopSound(SHIP_ENGINE_SOUND3);
		if (70.f >= m_fAngle_1)
		{
			Engine::PlaySound_SR(L"Ham_Posin_Sound.wav", SoundType::SHIP_ENGINE_SOUND1, 0.5f);
			m_fAngle_1 += 30.f * fTimeDelta;
			m_pTransformHead->Rotation(ROT_Y, D3DXToRadian(30.f * fTimeDelta));
			m_pTransformPosin->Rotation(ROT_Y, D3DXToRadian(30.f * fTimeDelta));
		}
		if (75.f >= m_fAngle_2)

		{
			Engine::PlaySound_SR(L"Ham_Posin_Sound.wav", SoundType::SHIP_ENGINE_SOUND2, 0.5f);
			m_fAngle_2 += 25.f * fTimeDelta;
			m_pTransformHead2->Rotation(ROT_Y, D3DXToRadian(25.f * fTimeDelta));
			m_pTransformPosin2->Rotation(ROT_Y, D3DXToRadian(25.f * fTimeDelta));
		}

		if (70.f <= m_fAngle_1 && 70.f <= m_fAngle_2)
		{
			Engine::StopSound(SHIP_ENGINE_SOUND5);
			Engine::StopSound(SHIP_ENGINE_SOUND1);
			Engine::StopSound(SHIP_ENGINE_SOUND2);
			Engine::PlaySound_SR(L"Ham_Posin_Sound.wav", SoundType::SHIP_ENGINE_SOUND1, 1.f);
			Engine::PlaySound_SR(L"Ham_Posin_Sound.wav", SoundType::SHIP_ENGINE_SOUND2, 1.f);
			m_fMoveTime = 0.f;
			m_bShootReady = true;

		}
	}
	else if (m_bShootReady && m_fMoveTime < 1.f)
	{
		m_pTransformPosin->Rotation(ROT_X, D3DXToRadian(-5.f * fTimeDelta));
		m_pTransformPosin2->Rotation(ROT_X, D3DXToRadian(-5.f * fTimeDelta));
	}
	else if (m_bShootReady && m_fMoveTime > 1.f)
	{
		Engine::StopSound(SHIP_ENGINE_SOUND1);
		Engine::StopSound(SHIP_ENGINE_SOUND2);
		m_fShootTime += fTimeDelta;
		if (0.4f < m_fShootTime && 4 > m_iShootCount)
		{
			m_fShootTime = 0.f;
			if (0 == m_iShootCount)
			{
				CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
				pObject->Set_Dead(false);
				pObject->Set_Scale(30.f);
				static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformHead);
				static_cast<CShootEffect*>(pObject)->Set_Dist(10.f*m_fScale, 1.f*m_fScale, 3.f * m_fScale);
				//static_cast<CShootEffect*>(pObject)->Set_Smoke(true);
				Engine::Get_Camera()->Shake_On();
				Engine::StopSound(SHIP_SHOOT_SOUND1);
				Engine::PlaySound_SR(L"Ham_Fire_1.wav", SHIP_SHOOT_SOUND1, 1.f);

				_vec3 Pos, Dir;
				m_pTransformPosin->Get_Info(INFO_POS, &Pos);
				m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
				D3DXVec3Normalize(&Dir, &Dir);
				Pos += Dir * 10.f;
				Engine::Reuse_Object(Pos, Dir, 1000.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), SHIP_BULLET);
			}
			else if (1 == m_iShootCount)
			{
				CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
				pObject->Set_Dead(false);
				pObject->Set_Scale(30.f);
				static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformHead);
				static_cast<CShootEffect*>(pObject)->Set_Dist(10.f*m_fScale, -1.f*m_fScale, 3.f * m_fScale);
				//static_cast<CShootEffect*>(pObject)->Set_Smoke(true);
				Engine::Get_Camera()->Shake_On();
				Engine::StopSound(SHIP_SHOOT_SOUND2);
				Engine::PlaySound_SR(L"Ham_Fire_2.wav", SHIP_SHOOT_SOUND2, 1.f);

				_vec3 Pos, Dir;
				m_pTransformPosin->Get_Info(INFO_POS, &Pos);
				m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
				D3DXVec3Normalize(&Dir, &Dir);
				Pos += Dir * 10.f;
				Engine::Reuse_Object(Pos, Dir, 1000.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), SHIP_BULLET);
			}
			else if (2 == m_iShootCount)
			{
				CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
				pObject->Set_Dead(false);
				pObject->Set_Scale(30.f);
				static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformHead2);
				static_cast<CShootEffect*>(pObject)->Set_Dist(10.f*m_fScale, 1.f*m_fScale, 3.f * m_fScale);
				//static_cast<CShootEffect*>(pObject)->Set_Smoke(true);
				Engine::Get_Camera()->Shake_On();
				Engine::StopSound(SHIP_SHOOT_SOUND3);
				Engine::PlaySound_SR(L"Ham_Fire_3.wav", SHIP_SHOOT_SOUND3, 1.f);

				_vec3 Pos, Dir;
				m_pTransformPosin2->Get_Info(INFO_POS, &Pos);
				m_pTransformPosin2->Get_Info(INFO_LOOK, &Dir);
				D3DXVec3Normalize(&Dir, &Dir);
				Pos += Dir * 10.f;
				Engine::Reuse_Object(Pos, Dir, 1000.f, m_pTransformPosin2->Get_Angle(ROT_X), m_pTransformPosin2->Get_Angle(ROT_Y), SHIP_BULLET);
			}
			else if (3 == m_iShootCount)
			{
				CGameObject* pObject = Engine::Reuse_Effect(EFFECT_ID::TANK_SHOOT_SMOKE);
				pObject->Set_Dead(false);
				pObject->Set_Scale(30.f);
				static_cast<CShootEffect*>(pObject)->Set_Target(m_pTransformHead2);
				static_cast<CShootEffect*>(pObject)->Set_Dist(10.f*m_fScale, -1.f*m_fScale, 3.f * m_fScale);
				//static_cast<CShootEffect*>(pObject)->Set_Smoke(true);
				Engine::Get_Camera()->Shake_On();
				Engine::StopSound(SHIP_SHOOT_SOUND4);
				Engine::PlaySound_SR(L"Ham_Fire_4.wav", SHIP_SHOOT_SOUND4, 1.f);

				_vec3 Pos, Dir;
				m_pTransformPosin2->Get_Info(INFO_POS, &Pos);
				m_pTransformPosin->Get_Info(INFO_LOOK, &Dir);
				D3DXVec3Normalize(&Dir, &Dir);
				Pos += Dir * 10.f;
				m_pNewTarget = Engine::Reuse_Object(Pos, Dir, 1000.f, m_pTransformPosin2->Get_Angle(ROT_X), m_pTransformPosin2->Get_Angle(ROT_Y), SHIP_BULLET);
				static_cast<CShipCamera*>(Engine::Get_Camera())->Set_Target(static_cast<CTransform*>(m_pNewTarget->Get_Component(L"Proto_Bullet_Transform", ID_DYNAMIC)));
				static_cast<CShipCamera*>(Engine::Get_Camera())->Eye_Lock(true);
			}

			m_iShootCount++;
		}
		else if (4 == m_iShootCount)
		{
			m_fChangeTime += fTimeDelta;

			if (m_fChangeTime > 1.f)
			{
				static_cast<CBattleShip_Support*>(Engine::Get_Object(L"GameLogic", L"BattleShip_Support"))->Fire();
				m_bDead = true;
				static_cast<CShipCamera*>(Engine::Get_Camera())->Eye_Lock(false);
				Engine::Camera_Change(L"TankCamera");
				_vec3 Pos;
				static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"BattleShip_Support", L"Proto_Transform_1", ID_DYNAMIC))->Get_Info(INFO_POS, &Pos);
				Engine::Get_Camera()->Camera_Setting(Pos);
				//여기서 발사 사운드 다 없애기
				Engine::StopSound(SHIP_SHOOT_SOUND1);
				Engine::StopSound(SHIP_SHOOT_SOUND2);
				Engine::StopSound(SHIP_SHOOT_SOUND3);
				Engine::StopSound(SHIP_SHOOT_SOUND4);
				static_cast<CTankSet*>(Engine::Get_Object(L"GameLogic", L"PlayerVehicle"))->Set_Rock(false);
			}
		}
	}

	Add_RenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CBattleShip::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	return CGameObject::LateUpdate_Object();
}

void CBattleShip::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformBody->Get_WorldMatrix());
	m_pBody->Render(m_pTransformBody->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead2->Get_WorldMatrix());
	m_pHead2->Render(m_pTransformHead2->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin2->Get_WorldMatrix());
	m_pPosin2->Render(m_pTransformPosin2->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformSee->Get_WorldMatrix());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

void CBattleShip::RenderGUI(void)
{
}

void CBattleShip::Strike(void)
{
	m_bShootReady = false;
	m_bBoomShoot = true;
	m_bDead = false;
	m_fAngle_1 = 0.f;
	m_fAngle_2 = 0.f;
	m_fShootTime = 0.f;
	m_iShootCount = 0;
	m_fChangeTime = 0.f;
	m_fMoveTime = 0.f;
	m_pTransformBody->Reset_Trans();
	m_pTransformHead->Reset_Trans();
	m_pTransformHead2->Reset_Trans();
	m_pTransformPosin->Reset_Trans();
	m_pTransformPosin2->Reset_Trans();

	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformHead->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformHead2->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformPosin->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformPosin2->Set_Scale(m_fScale, m_fScale, m_fScale);

	m_pTransformBody->Set_Pos(0.f, 5.f * m_fScale, 0.f * m_fScale);
	m_pTransformHead->Set_Pos(0.f, 11.f * m_fScale, 4.f * m_fScale);
	m_pTransformHead2->Set_Pos(0.f, 9.f * m_fScale, 15.f * m_fScale);
	m_pTransformPosin->Set_Pos(0.f, 13.5f * m_fScale, 4.f * m_fScale);
	m_pTransformPosin2->Set_Pos(0.f, 11.5f * m_fScale, 15.f * m_fScale);


	_vec3 Move = { -500.f, 1.f, 100.f };
	m_pTransformBody->Move_Pos(&Move);
	m_pTransformHead->Move_Pos(&Move);
	m_pTransformHead2->Move_Pos(&Move);
	m_pTransformPosin->Move_Pos(&Move);
	m_pTransformPosin2->Move_Pos(&Move);
}

HRESULT CBattleShip::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformBody = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Ship_Body_Transform", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Ship_Head_Transform", pComponent });

	pComponent = m_pTransformHead2 = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Ship_Head2_Transform", pComponent });

	pComponent = m_pTransformPosin = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Ship_Posin_Transform", pComponent });

	pComponent = m_pTransformPosin2 = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Ship_Posin2_Transform", pComponent });

	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Ham_body", false);
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Ham_head", false);
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Ham_posin", false);
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	pComponent = m_pHead2 = CVoxel::Create(m_pGraphicDev, L"Ham_head2", false);
	NULL_CHECK_RETURN(m_pHead2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelHead2", pComponent });

	pComponent = m_pPosin2 = CVoxel::Create(m_pGraphicDev, L"Ham_posin2", false);
	NULL_CHECK_RETURN(m_pPosin2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin2", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Prototype(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_See"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_See", pComponent });

	pComponent = m_pTransformSee = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformSee, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_See_Transform", pComponent });


	return S_OK;
}

HRESULT CBattleShip::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fScale = 3.f;
	m_pTransformBody->Reset_Trans();
	m_pTransformHead->Reset_Trans();
	m_pTransformHead2->Reset_Trans();
	m_pTransformPosin->Reset_Trans();
	m_pTransformPosin2->Reset_Trans();

	m_pTransformBody->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformHead->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformHead2->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformPosin->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformPosin2->Set_Scale(m_fScale, m_fScale, m_fScale);

	m_pTransformBody->Set_Pos(0.f, 5.f * m_fScale, 0.f * m_fScale);
	m_pTransformHead->Set_Pos(0.f, 11.f * m_fScale, 4.f * m_fScale);
	m_pTransformHead2->Set_Pos(0.f, 9.f * m_fScale, 15.f * m_fScale);
	m_pTransformPosin->Set_Pos(0.f, 13.5f * m_fScale, 4.f * m_fScale);
	m_pTransformPosin2->Set_Pos(0.f, 11.5f * m_fScale, 15.f * m_fScale);

	_vec3 Move = { 0.f, 1.f, 0.f };
	m_pTransformBody->Move_Pos(&Move);
	m_pTransformHead->Move_Pos(&Move);
	m_pTransformHead2->Move_Pos(&Move);
	m_pTransformPosin->Move_Pos(&Move);
	m_pTransformPosin2->Move_Pos(&Move);

	return S_OK;
}

CBattleShip * CBattleShip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBattleShip* pInstance = new CBattleShip(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBattleShip::Free(void)
{
	CGameObject::Free();
}
