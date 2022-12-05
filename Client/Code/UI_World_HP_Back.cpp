#include "stdafx.h"
#include "..\Header\UI_World_HP_Back.h"
#include "Export_Function.h"

#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"

CUI_World_HP_Back::CUI_World_HP_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_World_HP_Back::CUI_World_HP_Back(const CUI_World_HP_Back& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_World_HP_Back::~CUI_World_HP_Back()
{
}

HRESULT CUI_World_HP_Back::Ready_Object(_float fOrgin_HP)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_fOrgin_HP = m_fHP = fOrgin_HP;

	//m_fOrgin_ScaleX = m_fScaleX = 1.5f;
	//m_fOrgin_PosX = m_fPosX = 30.f;

	//m_fScaleY = 0.5f;
	//m_fScaleZ = 1.f;

	////m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);

	//m_fPosY = 5.f;
	//m_fPosZ = 30.f;

	m_fOrgin_HP = m_fHP = fOrgin_HP;

	m_fOrgin_ScaleX = m_fScaleX = 3.f;
	m_fOrgin_PosX = m_fPosX = 30.f;

	m_fScaleY = 0.5f;
	m_fScaleZ = 1.f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);

	m_fPosY = 5.f;
	m_fPosZ = 30.f;

	return S_OK;
}

_int CUI_World_HP_Back::Update_Object(const _float & fTimeDelta)
{
	//Update_HP();
	Update_HP();

	_vec3 vTemp;
	m_pTransform->Get_Info(INFO_POS, &vTemp);
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

	fScale[ROT_X] = m_fScaleX;// 0
	fScale[ROT_Y] = m_fScaleY;// 1
	fScale[ROT_Z] = m_fScaleZ;// 2

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

void CUI_World_HP_Back::LateUpdate_Object(void)
{
	/*_matrix		matWorld, matView, matBill;
	m_pTransform->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransform->Set_WorldMatrix(&(matBill * matWorld));
	*/

	__super::LateUpdate_Object();
}

void CUI_World_HP_Back::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 업데이트 된 view로 렌더링
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matView);
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
	m_pTexture->Set_Texture(0);
	m_pRcTex->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

CUI_World_HP_Back * CUI_World_HP_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fOrigin_HP)
{
	CUI_World_HP_Back*		pInstance = new CUI_World_HP_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(fOrigin_HP)))
	{
		MSG_BOX("World Hp Back Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_World_HP_Back::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_World_Hp_Back_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_World_Hp_Back_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Back_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Hp_Back_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_WHP1", pComponent });


	return S_OK;
}

void CUI_World_HP_Back::Free(void)
{
	CGameObject::Free();
}

void CUI_World_HP_Back::Update_HP(void)
{
	if (m_fHP > m_fOrgin_HP) { m_fHP = m_fOrgin_HP; }
	if (m_fHP < 0.f) { m_fHP = 0.f; }
	_float HP_Percent = (m_fHP / m_fOrgin_HP);
	if (HP_Percent >= 1.f) { HP_Percent = 1.f; }
	if (HP_Percent <= 0.f) { HP_Percent = 0.f; }

	m_fScaleX = (m_fOrgin_ScaleX * HP_Percent);
	//m_fScaleX = m_fOrgin_ScaleX - m_fOrgin_ScaleX * (1.f - HP_Percent);


	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);


	m_fPosX = (m_fOrgin_PosX - (0.019125f *(100.f - (HP_Percent * 100))));



	//m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	//m_pTransform->Set_Pos(m_fPosX, m_fPosY, m_fPosZ);

	/*
	1. 예외 처리 한다. ( HP가 100%를 넘을 때와 0 보다 작을 때)
	2. 실시간 HP 현황을 저장한다.(비율로)
	3. 비율에 대한 예외처리도 한다.

	*/





}