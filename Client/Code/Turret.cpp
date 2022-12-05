#include "stdafx.h"
#include "..\Header\Turret.h"


CTurret::CTurret(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTankSet(pGraphicDev)
{
	m_stInfo.eID = CAR_ID::TURRET;
}

CTurret::CTurret(const CTurret & rhs)
	: CTankSet(rhs)
{
}

CTurret::~CTurret()
{
}

_int CTurret::Update_Object(const _float & fTimeDelta)
{
	return _int();
}

void CTurret::LateUpdate_Object(void)
{
}

void CTurret::Render_Object(void)
{
}

void CTurret::RenderGUI(void)
{
}

HRESULT CTurret::Add_Component(void)
{
	return E_NOTIMPL;
}

HRESULT CTurret::Ready_Object(void)
{
	return E_NOTIMPL;
}

void CTurret::Target_Find(const _float & fTimeDelta)
{
}

void CTurret::Posin_Shake(const _float & fTimeDelta)
{
}

CTurret * CTurret::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CTurret::Free(void)
{
}
