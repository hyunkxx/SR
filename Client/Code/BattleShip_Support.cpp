#include "stdafx.h"
#include "..\Header\BattleShip_Support.h"

#include "Export_Function.h"
#include "DroneCamera.h"
#include "TankSet.h"
#include "TankManager.h"
#include "BattleShip.h"
CBattleShip_Support::CBattleShip_Support(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	
}

CBattleShip_Support::CBattleShip_Support(const CBattleShip_Support & rhs)
	: CGameObject(rhs)
{
}

CBattleShip_Support::~CBattleShip_Support()
{
}

HRESULT CBattleShip_Support::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_bStrike = true;
	m_fScale = 5.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	return S_OK;
}

_int CBattleShip_Support::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_NOEVENT;
	 
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);


	if (m_bStrike)
	{
		m_fDaedCount += fTimeDelta;

		if (0.1f > m_fDaedCount)
		{
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			Pos.y = -0.5f;
			m_pTransformCom->Set_Pos(Pos.x, Pos.y, Pos.z);

		}
		else if (0.2f > m_fDaedCount)
		{
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			Pos.y = 0.1f;
			m_pTransformCom->Set_Pos(Pos.x, Pos.y, Pos.z);
		}

		if (6.f < m_fDaedCount)
		{
			m_fDaedCount = 0.f;
			m_bDead = true;
		}
		if (1.f < m_fDaedCount && !m_bSetting)
		{
			Engine::StopSound(DRONE_SOUND);
			m_bSetting = true;
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			//여기서 카메라 교체
			Engine::Camera_Change(L"TankCamera");
		}
	}
	else
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(200 * fTimeDelta));


	Key_Input(fTimeDelta);
	Add_RenderGroup(RENDER_ALPHA, this);
	return __super::Update_Object(fTimeDelta);
}

void CBattleShip_Support::LateUpdate_Object(void)
{
	if (m_bDead)
		return;


	__super::LateUpdate_Object();
}

void CBattleShip_Support::Render_Object(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	for (int i = 0; m_iShootCount > i; i++)
	{
		if (3 <= i)
			break;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom_Copy[i]->Get_WorldMatrix());
		m_pTextureCom_Copy[i]->Set_Texture();
		m_pBufferCom_Copy[i]->Render_Buffer();
	}
}

void CBattleShip_Support::RenderGUI(void)
{
}

void CBattleShip_Support::Air_Rain(_vec3 _vPos)
{
	m_fScale = 5.f;
	m_bStrike = false;
	m_bSetting = false;
	m_bDead = false;
	m_bRock = false;
	m_fDaedCount = 0.f;
	m_iShootCount = 0;
	m_pTransformCom->Reset_Trans();
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Set_Pos(_vPos.x, 0.1f, _vPos.z);

	for (int i = 0; 3 > i; i++)
	{
		m_pTransformCom_Copy[i]->Reset_Trans();
		m_pTransformCom_Copy[i]->Set_Scale(m_fScale, m_fScale, m_fScale);
		m_pTransformCom_Copy[i]->Rotation(ROT_X, D3DXToRadian(90.f));
	}

	// 여기에 카메라 체인지로 폭격 카메라 넣어주기
	Engine::Camera_Change(L"DroneCamera");
	if (dynamic_cast<CDroneCamera*>(Engine::Get_Camera()))
	{
		dynamic_cast<CDroneCamera*>(Engine::Get_Camera())->Reset_Pos();
		dynamic_cast<CDroneCamera*>(Engine::Get_Camera())->Target_Setting(m_pTransformCom);
	}
	static_cast<CTankSet*>(Engine::Get_Object(L"GameLogic", L"PlayerVehicle"))->Set_Rock(true);

}

void CBattleShip_Support::Key_Input(const _float & fTimeDelta)
{
	if (m_bRock)
		return;

	_vec3 Move = { 0.f, 0.f, 0.f };

	if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD)
		Move.z += 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_S) == KEY_STATE::HOLD)
		Move.z -= 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
		Move.x -= 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
		Move.x += 100.f * fTimeDelta;

	if (Engine::Get_DIMouseState_Custom(DIM_LB) == KEY_STATE::TAP && !m_bStrike)
	{

		if(3 > m_iShootCount)
		{
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			m_pTransformCom_Copy[m_iShootCount]->Set_Pos(Pos.x, 0.1f, Pos.z);
			m_pTransformCom_Copy[m_iShootCount]->Rotation(ROT_Y, m_pTransformCom->Get_Angle(ROT_Y));
		}
		++m_iShootCount;
		Engine::StopSound(CAMERA_CHANGE_SOUND);
		Engine::PlaySound_SR(L"Target_Select.wav", CAMERA_CHANGE_SOUND, 1.f);
		if (4 == m_iShootCount)
		{
			static_cast<CBattleShip*>(Engine::Get_Object(L"GameLogic", L"BattleShip"))->Strike();
			m_bStrike = true;
			m_bRock = true;

		}
	}

	m_pTransformCom->Move_Pos(&Move);
}

HRESULT CBattleShip_Support::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"target_4"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_4", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

///////////////////////////////////////////////////////////////////////////////////////////////////////


	pComponent = m_pBufferCom_Copy[0] = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom_Copy[0], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex_1", pComponent });

	pComponent = m_pTextureCom_Copy[0] = dynamic_cast<CTexture*>(Clone_Prototype(L"target_4_1"));
	NULL_CHECK_RETURN(m_pTextureCom_Copy[0], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_4_1", pComponent });

	pComponent = m_pTransformCom_Copy[0] = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom_Copy[0], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_1", pComponent });

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pComponent = m_pBufferCom_Copy[1] = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom_Copy[1], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex_2", pComponent });

	pComponent = m_pTextureCom_Copy[1] = dynamic_cast<CTexture*>(Clone_Prototype(L"target_4_2"));
	NULL_CHECK_RETURN(m_pTextureCom_Copy[1], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_4_2", pComponent });

	pComponent = m_pTransformCom_Copy[1] = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom_Copy[1], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_2", pComponent });

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	pComponent = m_pBufferCom_Copy[2] = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom_Copy[2], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex_3", pComponent });

	pComponent = m_pTextureCom_Copy[2] = dynamic_cast<CTexture*>(Clone_Prototype(L"target_4_3"));
	NULL_CHECK_RETURN(m_pTextureCom_Copy[2], E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_4_3", pComponent });

	pComponent = m_pTransformCom_Copy[2] = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom_Copy[2], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_3", pComponent });

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

CBattleShip_Support * CBattleShip_Support::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBattleShip_Support* pInstance = new CBattleShip_Support(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CBattleShip_Support_Create_Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattleShip_Support::Free(void)
{
	__super::Free();
}