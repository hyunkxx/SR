#include "stdafx.h"
#include "..\Header\UI_World_Bubble.h"
#include "Export_Function.h"

#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "UI_FontMgr.h"

CUI_World_Bubble::CUI_World_Bubble(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_World_Bubble::CUI_World_Bubble(const CUI_World_Bubble& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_World_Bubble::~CUI_World_Bubble()
{
}

HRESULT CUI_World_Bubble::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);




	m_fScaleX = 3.f;
	m_fScaleY = 2.f;
	m_fScaleZ = 1.f;

	m_pTransform1->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform2->Set_Scale(m_fScaleX - 2.f, m_fScaleY - .1f, m_fScaleZ);
	m_fPosX = 1.f;
	m_fPosY = 1.f;
	m_fPosZ = 1.f;

	return S_OK;
}

_int CUI_World_Bubble::Update_Object(const _float & fTimeDelta)
{

	Update_Pos();

	m_fTime += fTimeDelta;

	if (Get_DIKeyState_Custom(DIK_C) == KEY_STATE::TAP)
	{
		m_bShow = true;
	}
	if (m_fTime >= m_fThreeSec)
	{
		m_bShow = false;
		m_fTime = 0.f;
	}

	_vec3 vTemp;
	m_pTransform1->Get_Info(INFO_POS, &vTemp);
	m_pTransform2->Get_Info(INFO_POS, &vTemp);
	__super::Compute_ViewZ(&vTemp);

	//카메라 객체
	CGameObject* pTankView = Engine::Get_Object(L"Environment", L"TankCamera");

	D3DXMatrixIdentity(&matView);

	// 카메라의 뷰 전체를 뜯어오기
	pTankView->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);


	// 위치값 날리기
	memset(&matView._41, 0, sizeof(_vec3));

	// 역행렬
	D3DXMatrixInverse(&matView, 0, &matView);

	// 여기에 각 오브젝트의 좌표값을 넣어 주어야 한다.
	_vec3 BillPos = _vec3(m_fPosX, m_fPosY, m_fPosZ);

	// 크기 값 조정
	float fScale[ROT_END];

	fScale[ROT_X] = 5.f;// 0
	fScale[ROT_Y] = 2.f;// 1
	fScale[ROT_Z] = 1.f;// 2

						// 고정 위치 지정.
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	// 거리 대비 크기 지정.
	for (int i = 0; i < ROT_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			matView(i, j) *= fScale[i];
		}

	}

	__super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_WORLD_UI, this);
	return 0;
}

void CUI_World_Bubble::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_World_Bubble::Render_Object(void)
{
	if (m_bShow)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matView);

		m_pTexture1->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		m_pTexture2->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

CUI_World_Bubble * CUI_World_Bubble::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_World_Bubble*		pInstance = new CUI_World_Bubble(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("World_Bubble Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_World_Bubble::Update_Pos(void)
{
	_vec3 vPlayerPos;

	CTransform* pPlayerTrans = static_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"PlayerVehicle", L"Proto_TransformBody", ID_DYNAMIC));

	pPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);

	m_fPosX = vPlayerPos.x;
	m_fPosY = vPlayerPos.y + 5.f;
	m_fPosZ = vPlayerPos.z;

	_float fz = m_fPosZ - 0.3f;

	m_pTransform1->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);
	m_pTransform2->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);
}





HRESULT CUI_World_Bubble::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture1 = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Bubble_Tex"));
	NULL_CHECK_RETURN(m_pTexture1, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Bubble_Tex", pComponent });

	pComponent = m_pTexture2 = static_cast<CTexture*>(Clone_Prototype(L"Proto_Steam_Tex"));
	NULL_CHECK_RETURN(m_pTexture2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Steam_Tex", pComponent });

	pComponent = m_pTransform1 = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform1, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_B", pComponent });

	pComponent = m_pTransform2 = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform2, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_S", pComponent });


	return S_OK;
}

void CUI_World_Bubble::Free(void)
{
	CGameObject::Free();
}
