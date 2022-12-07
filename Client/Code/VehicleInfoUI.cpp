#include "stdafx.h"
#include "..\Header\VehicleInfoUI.h"

#include "Export_Function.h"
#include "TankManager.h"

USING(Engine)

CVehicleInfoUI::CVehicleInfoUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_nCurrentIndex(0)
{
}


CVehicleInfoUI::~CVehicleInfoUI()
{
}

CVehicleInfoUI * CVehicleInfoUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVehicleInfoUI* pInstance = new CVehicleInfoUI(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CVehicleInfoUI::Add_Component(void)
{
	return S_OK;
}

void CVehicleInfoUI::Free(void)
{
	__super::Free();
}

HRESULT CVehicleInfoUI::Ready_Object(void)
{
	Add_Component();

	return S_OK;
}

_int CVehicleInfoUI::Update_Object(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_UI, this);

	if (m_bUpdateUI)
	{
		UpdateString();
	}

	SmoothString(0.1f, fTimeDelta, m_szTitleBuff, m_szTitle);
	SmoothString(0.1f, fTimeDelta, m_szNameBuff, m_szName);
	SmoothString(0.1f, fTimeDelta, m_szSpeedBuff, m_szSpeed);
	SmoothString(0.1f, fTimeDelta, m_szMaxSpeedBuff, m_szMaxSpeed);
	SmoothString(0.1f, fTimeDelta, m_szRotSpeedBuff, m_szRotSpeed);
	SmoothString(0.1f, fTimeDelta, m_szDamageBuff, m_szDamage);
	SmoothString(0.1f, fTimeDelta, m_szReloadSpeedBuff, m_szReloadSpeed);
	SmoothString(0.1f, fTimeDelta, m_szHPBuff, m_szHP);
	SmoothString(0.1f, fTimeDelta, m_szBulletSpeedBuff, m_szBulletSpeed);

	return _int();
}

void CVehicleInfoUI::LateUpdate_Object(void)
{
}

void CVehicleInfoUI::Render_Object(void)
{
	D3DXCOLOR color = { 0.8f, 0.8f, 0.8f , 1.f };

	_vec2 vPos = { WINCX - 350.f, WINCY - 300.f };
	Render_Font(L"Font_Retro", m_szTitleBuff, &vPos, { 1.f, 1.f, 0.f, 1.f });

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szNameBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szSpeedBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szMaxSpeedBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szRotSpeedBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szDamageBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szReloadSpeedBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szHPBuff, &vPos, color);

	vPos = { vPos.x, vPos.y + 25.f };
	Render_Font(L"Font_Retro", m_szBulletSpeedBuff, &vPos, color);
}

void CVehicleInfoUI::SmoothString(float fSpeed, float fDeltaTime, _tchar * dest, _tchar * src)
{
	m_Timer += fDeltaTime;

	if (m_Timer >= fSpeed)
	{
		m_nIndex++;
		m_Timer = 0.f;
	}

	for (int i = 0; i < m_nIndex; ++i)
	{
		if (!src[i])
			break;

		dest[i] = src[i];
	}
}

void CVehicleInfoUI::UpdateString()
{
	m_nIndex = 0;

	data = CTankManager::GetInstance()->GetData((VEHICLE)m_nCurrentIndex);

	ZeroMemory(m_szTitle, sizeof(m_szTitle));
	ZeroMemory(m_szTitleBuff, sizeof(m_szTitleBuff));

	wstring title = L"차량정보";
	lstrcpy(m_szTitle, title.c_str());

	ZeroMemory(m_szName, sizeof(m_szName));
	ZeroMemory(m_szNameBuff, sizeof(m_szNameBuff));

	wstring name = L"차량  명    " + data.strName;
	lstrcpy(m_szName, name.c_str());

	ZeroMemory(m_szSpeed, sizeof(m_szSpeed));
	ZeroMemory(m_szSpeedBuff, sizeof(m_szSpeedBuff));

	wstring speed = L"엔진출력    " + to_wstring(data.fAccel_Ad) + L" m/sec";
	lstrcpy(m_szSpeed, speed.c_str());

	ZeroMemory(m_szMaxSpeed, sizeof(m_szMaxSpeed));
	ZeroMemory(m_szMaxSpeedBuff, sizeof(m_szMaxSpeedBuff));

	wstring maxSpeed = L"최대속도    " + to_wstring((int)(data.fAccel_top_speed * 3.6f)) + L" km/h";
	lstrcpy(m_szMaxSpeed, maxSpeed.c_str());

	ZeroMemory(m_szRotSpeed, sizeof(m_szRotSpeed));
	ZeroMemory(m_szRotSpeedBuff, sizeof(m_szRotSpeedBuff));

	wstring rotSpeed = L"회전속도    " + to_wstring((int)data.RotSpeed) + L" °/sec";
	lstrcpy(m_szRotSpeed, rotSpeed.c_str());

	ZeroMemory(m_szDamage, sizeof(m_szDamage));
	ZeroMemory(m_szDamageBuff, sizeof(m_szDamageBuff));

	wstring damage = L"포탄공격력  100";
	lstrcpy(m_szDamage, damage.c_str());

	ZeroMemory(m_szReloadSpeed, sizeof(m_szReloadSpeed));
	ZeroMemory(m_szReloadSpeedBuff, sizeof(m_szReloadSpeedBuff));

	wstring reloadSpeed = L"발사속도    " + to_wstring(data.fReload);
	lstrcpy(m_szReloadSpeed, reloadSpeed.c_str());

	ZeroMemory(m_szHP, sizeof(m_szHP));
	ZeroMemory(m_szHPBuff, sizeof(m_szHPBuff));

	wstring hp = L"내 구 도    " + to_wstring(data.fMaxHP);
	lstrcpy(m_szHP, hp.c_str());

	ZeroMemory(m_szBulletSpeed, sizeof(m_szBulletSpeed));
	ZeroMemory(m_szBulletSpeedBuff, sizeof(m_szBulletSpeedBuff));

	wstring bulletSpeed = L"탄    속    " + to_wstring(data.iCannonSpeed);
	lstrcpy(m_szBulletSpeed, bulletSpeed.c_str());

	m_bUpdateUI = false;
}
