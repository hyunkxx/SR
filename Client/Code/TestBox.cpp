#include "stdafx.h"
#include "..\Header\TestBox.h"
#include "Export_Function.h"


CTestBox::CTestBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTestBox::CTestBox(const CTestBox & rhs)
	: Engine::CGameObject(rhs)
{
}

CTestBox::~CTestBox()
{
}

HRESULT CTestBox::Ready_Object(void)
{
	m_fScale = 3.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_stBody.fLen[x] = 0.5f * m_fScale;
	m_stBody.fLen[y] = 0.5f * m_fScale;
	m_stBody.fLen[z] = 0.5f * m_fScale;

	CGameObject::Ready_Object();
	return S_OK;
}

_int CTestBox::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	m_pTransformHead->Set_Scale(m_fScale, m_fScale, m_fScale);
	Update_OBB();
	Add_RenderGroup(RENDER_NONALPHA, this);


	return OBJ_NOEVENT;
}

void CTestBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTestBox::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHeadBuffer->Render_Buffer();

}

const _vec3 CTestBox::Get_Info(void)
{
	_vec3 Info;
	m_pTransformHead->Get_Info(INFO_POS, &Info);

	return Info;
}

void CTestBox::Move_Info(_vec3 _Info)
{
	m_pTransformHead->Move_Pos(&_Info);
}

void CTestBox::Update_OBB(void)
{
	_vec3 Pos, Right, Up, Look;

	m_pTransformHead->Get_Info(INFO_POS, &m_stBody.vPos);
	m_pTransformHead->Get_Info(INFO_RIGHT, &m_stBody.vDir[x]);
	m_pTransformHead->Get_Info(INFO_UP, &m_stBody.vDir[y]);
	m_pTransformHead->Get_Info(INFO_LOOK, &m_stBody.vDir[z]);

	for (_int i = 0; i < 3; i++)
		D3DXVec3Normalize(&m_stBody.vDir[i], &m_stBody.vDir[i]);

}

HRESULT CTestBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pHeadBuffer = static_cast<CTank_Head*>(Clone_Prototype(L"Proto_TankHead"));
	NULL_CHECK_RETURN(m_pHeadBuffer, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TankHead", pComponent });

	pComponent = m_pTransformHead = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformHead", pComponent });
	_vec3 a = { 30.f,1.f,30.f };
	m_pTransformHead->Set_Pos(a.x, a.y, a.z);

	return S_OK;
}

CTestBox * CTestBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestBox*		pInstance = new CTestBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("TestPlayer Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestBox::Free(void)
{
	__super::Free();
}
