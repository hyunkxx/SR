#include "stdafx.h"
#include "..\Header\VehicleSelector.h"

#include "Export_Function.h"

#include "ButtonUI.h"

CVehicleSelector::CVehicleSelector(LPDIRECT3DDEVICE9 pGramphicDev)
	: CGameObject(pGramphicDev)
	, m_bShow(false)
{
}

CVehicleSelector * CVehicleSelector::Create(LPDIRECT3DDEVICE9 pGramphicDev)
{
	return nullptr;
}

void CVehicleSelector::Free(void)
{
	__super::Free();
}

CVehicleSelector::~CVehicleSelector()
{
}

HRESULT CVehicleSelector::AddComponent()
{
	CComponent* pComponent = nullptr;

	//pComponent = m_pButton[0] = CButtonUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_ DYNAMIC].insert({ L"btn_00", pComponent });

	return S_OK;
}

HRESULT CVehicleSelector::Ready_Object(void)
{
	if (FAILED(AddComponent()))
		return E_FAIL;



	return S_OK;
}

_int CVehicleSelector::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CVehicleSelector::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CVehicleSelector::Render_Object(void)
{
}

void CVehicleSelector::RenderGUI(void)
{
}
