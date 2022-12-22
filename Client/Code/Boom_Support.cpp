#include "stdafx.h"
#include "..\Header\Boom_Support.h"

#include "Export_Function.h"
#include "Bomber.h"
#include "DroneCamera.h"
#include "BoomCamera.h"
#include "TankSet.h"
#include "TankManager.h"

CBoom_Support::CBoom_Support(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoom_Support::CBoom_Support(const CBoom_Support & rhs)
	: CGameObject(rhs)
{
}

CBoom_Support::~CBoom_Support()
{
}

HRESULT CBoom_Support::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bDead = true;
	m_bStrike = true;
	m_fScale = 5.f;
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	// UI_Minimap
	D3DXMatrixOrthoLH(&UI_Minimap_matProj, WINCX, WINCY, 0.f, 1.f);
	m_fMinimap[SCALEX] = m_fMinimap[SCALEY] = 4.f;
	m_fMinimap[SCALEZ] = 1.f;
	m_pMinimap_Transform->Set_Scale(m_fMinimap[SCALEX], m_fMinimap[SCALEY], m_fMinimap[SCALEZ]);


	return S_OK;
}

_int CBoom_Support::Update_Object(const _float & fTimeDelta)
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
			CTankManager::GetInstance()->MouseLBTLock(false);
		}
		if (1.f < m_fDaedCount && !m_bSetting)
		{
			Engine::StopSound(DRONE_SOUND);
			m_bSetting = true;
			_vec3 Pos;
			m_pTransformCom->Get_Info(INFO_POS, &Pos);
			static_cast<CBomber*>(Engine::Get_Object(L"GameLogic", L"Bomber"))->Strike(Pos);
		}
	}
	else
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(200 * fTimeDelta));

	Key_Input(fTimeDelta);


	Update_Minimap();

	Add_RenderGroup(RENDER_ALPHA, this);

	return __super::Update_Object(fTimeDelta);
}

void CBoom_Support::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	__super::LateUpdate_Object();
}

void CBoom_Support::Render_Object(void)
{
	if (m_bDead)
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	// Minimap UI
	if (Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{
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
	}

}

void CBoom_Support::RenderGUI(void)
{
}

void CBoom_Support::Air_Rain(_vec3	_vPos)
{
	m_fScale = 5.f;
	m_bStrike = false;
	m_bSetting = false;
	m_bDead = false;
	m_bRock = false;
	m_fDaedCount = 0.f;
	m_pTransformCom->Reset_Trans();
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Set_Pos(_vPos.x, 0.1f, _vPos.z);
	// 여기에 카메라 체인지로 폭격 카메라 넣어주기
	Engine::Camera_Change(L"DroneCamera");
	if (dynamic_cast<CDroneCamera*>(Engine::Get_Camera()))
	{
		dynamic_cast<CDroneCamera*>(Engine::Get_Camera())->Reset_Pos();
		dynamic_cast<CDroneCamera*>(Engine::Get_Camera())->Target_Setting(m_pTransformCom);
	}

	static_cast<CTankSet*>(Engine::Get_Object(L"GameLogic", L"PlayerVehicle"))->Set_Rock(true);
}

void CBoom_Support::Key_Input(const _float & fTimeDelta)
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
		m_bStrike = true;
		m_bRock = true;
		Engine::StopSound(CAMERA_CHANGE_SOUND);
		Engine::PlaySound_SR(L"bbibic.mp3", CAMERA_CHANGE_SOUND, 1.f);
	}

	m_pTransformCom->Move_Pos(&Move);

}

HRESULT CBoom_Support::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"target_3"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"target_3", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });



	//MiniMap UI
	pComponent = m_pMinimap_RcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pMinimap_RcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pMinimap_Texure = static_cast<CTexture*>(Clone_Prototype(L"Proto_Minimap_Boom_Tex"));
	NULL_CHECK_RETURN(m_pMinimap_Texure, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Minimap_Boom_Tex", pComponent });

	pComponent = m_pMinimap_Transform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pMinimap_Transform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_Minimap_Boom", pComponent });



	return S_OK;
}

CBoom_Support * CBoom_Support::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoom_Support* pInstance = new CBoom_Support(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Boom_Support_Create_Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoom_Support::Free(void)
{
	Safe_Release(m_pMinimap_RcTex);
	Safe_Release(m_pMinimap_Texure);
	Safe_Release(m_pMinimap_Transform);
	__super::Free();
}

void CBoom_Support::Update_Minimap(void)
{


	_vec3 vTankPos;

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTankPos);

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

}
