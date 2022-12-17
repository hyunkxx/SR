#include "stdafx.h"
#include "..\Header\UI_FontMgr.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CUI_FontMgr)

CUI_FontMgr::CUI_FontMgr()
{
}


CUI_FontMgr::~CUI_FontMgr()
{
}

//void CUI_FontMgr::Initialize(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	// Main App 에서 하기로~
//}

void CUI_FontMgr::Update(const _float & fTimeDelta)
{
	lstrcpy(m_szTank_Name, L"탱크 이름/종류");

	Time_Font_UI_Update(fTimeDelta);
	Death_UI_Update(fTimeDelta);
}

void CUI_FontMgr::Time_Font_UI_Update(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (iPlayTime > 0)
	{
		if (m_fTime >= m_fOneSec)
		{
			iPlayTime = iPlayTime - 1;
			m_fTime = 0.f;
		}
	}

	_int iMin = (iPlayTime / 60);
	errno_t iCheck_TM = _itow_s(iMin, m_szTime_min, sizeof(m_szTime_min) / sizeof(m_szTime_min[0]), 10);

	if (iPlayTime <= 0)	{	lstrcpy(m_szTime_min, L"0");}

	_int iTen_Sec = ((iPlayTime % 60) / 10);
	errno_t iCheck_TT = _itow_s(iTen_Sec, m_szTime_tenSec, sizeof(m_szTime_tenSec) / sizeof(m_szTime_tenSec[0]), 10);

	_int iOne_Sec = ((iPlayTime % 60) - (iTen_Sec * 10));
	errno_t iCheck_TS = _itow_s(iOne_Sec, m_szTime_oneSec, sizeof(m_szTime_oneSec) / sizeof(m_szTime_oneSec[0]), 10);

}

void CUI_FontMgr::Death_UI_Update(const _float & fTimeDelta)
{



}
