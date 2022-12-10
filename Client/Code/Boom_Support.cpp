#include "stdafx.h"
#include "..\Header\Boom_Support.h"

#include "Export_Function.h"
#include "Bomber.h"
#include "DroneCamera.h"
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
	
	return S_OK;
}

_int CBoom_Support::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_NOEVENT;

	if (m_bStrike)
	{
		m_fDaedCount += fTimeDelta;
		if (0.3f < m_fDaedCount)
			m_bDead = true;
	}
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(200* fTimeDelta));
	Key_Input(fTimeDelta);




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

}

void CBoom_Support::RenderGUI(void)
{
}

void CBoom_Support::Air_Rain(_vec3	_vPos)
{
	m_bStrike = false;
	m_bDead = false;
	m_fDaedCount = 0.f;
	m_pTransformCom->Reset_Trans();
	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Set_Pos(_vPos.x, 2.f, _vPos.z);
	// 여기에 카메라 체인지로 폭격 카메라 넣어주기
	Engine::Camera_Change(L"DroneCamera");
	if (dynamic_cast<CDroneCamera*>(Engine::Get_Camera()))
		dynamic_cast<CDroneCamera*>(Engine::Get_Camera())->Reset_Pos();

}

void CBoom_Support::Key_Input(const _float & fTimeDelta)
{
	_vec3 Move = { 0.f, 0.f, 0.f };

	if (Engine::Get_DIKeyState_Custom(DIK_W) == KEY_STATE::HOLD)
		Move.z += 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_S)==KEY_STATE::HOLD)
		Move.z -= 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_A) == KEY_STATE::HOLD)
		Move.x -= 100.f * fTimeDelta;
	if (Engine::Get_DIKeyState_Custom(DIK_D) == KEY_STATE::HOLD)
		Move.x += 100.f * fTimeDelta;

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80  && !m_bStrike)
	{
		_vec3 Pos;
		m_pTransformCom->Get_Info(INFO_POS, &Pos);
		static_cast<CBomber*>(Engine::Get_Object(L"GameLogic", L"Bomber"))->Strike(Pos);
		Engine::Camera_Change(L"TankCamera");
		m_bStrike = true;
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
	__super::Free();
}
