#include "stdafx.h"
#include "..\Header\RedCarpet.h"

#include "Export_Function.h"
#include "Boss.h"
CRedCarpet::CRedCarpet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CRedCarpet::CRedCarpet(const CRedCarpet & rhs)
	: CGameObject(rhs)
{
}

CRedCarpet::~CRedCarpet()
{
}

_int CRedCarpet::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	m_bDeadCount += fTimeDelta;
	

	if (3.f > m_bDeadCount)
	{
		if (400.f > m_vLen.z)
			m_vLen.z += 200 * fTimeDelta;
		else
			m_vLen.z = 450.f;

		m_pTransCom->Set_Scale(m_vLen.x, m_vLen.z, 1.f);
		m_pTransCom->Move_Pos(&(m_vDir * 60.f * fTimeDelta));
	}
	else
	{
		m_vLen.z -= 150 * fTimeDelta;
		m_pTransCom->Set_Scale(m_vLen.x, m_vLen.z, 1.f);
		m_pTransCom->Move_Pos(&(m_vDir * 200.f * fTimeDelta));
	}
	
	if (5.f < m_bDeadCount)
		m_bDead = true;
	
	
	
	Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CRedCarpet::LateUpdate_Object(void)
{
	if (m_bDead)
		return;
	CGameObject::LateUpdate_Object();
}

void CRedCarpet::Render_Object(void)
{
	if (m_bDead)
		return;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrix());
	m_pRcCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CRedCarpet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransCom = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pRcCom = static_cast<CRcCol*>(Clone_Prototype(L"Proto_RcCol"));
	NULL_CHECK_RETURN(m_pRcCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcCol", pComponent });

	return S_OK;
}

HRESULT CRedCarpet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	return S_OK;
}

void CRedCarpet::Set_Carpet(CTransform * pTrans, OBB* pOBB)
{
	_vec3 BossRight;
	m_bDead = false;
	m_fXDist = 1.f;
	m_pBossTransCom = pTrans;
	m_pBossTransCom->Get_Info(INFO_POS, &m_vPos);
	m_pBossTransCom->Get_Info(INFO_LOOK, &m_vDir);
	m_pBossTransCom->Get_Info(INFO_RIGHT, &BossRight);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	D3DXVec3Normalize(&BossRight, &BossRight);
	_vec3 Right = { 1.f,0.f,0.f };

	_float Radian = acosf(D3DXVec3Dot(&BossRight, &Right));

	if (BossRight.z > Right.z)
		Radian *= -1;

	m_pTransCom->Reset_Trans();
	m_bDeadCount = 0.f;

	m_vLen = pOBB->fLen;

	m_pTransCom->Set_Scale(pOBB->fLen[x], pOBB->fLen[z], 1.f);
	m_pTransCom->Set_Pos(m_vPos.x, 0.1f, m_vPos.z);
	m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransCom->Rotation(ROT_Y, Radian);

}

CRedCarpet * CRedCarpet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRedCarpet* pInstance = new CRedCarpet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CRedCarpet Create Failed");
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CRedCarpet::Free(void)
{
	CGameObject::Free();
}
