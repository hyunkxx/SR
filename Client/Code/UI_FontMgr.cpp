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


// Only Stage
#pragma region

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

	if (iPlayTime <= 0) { lstrcpy(m_szTime_min, L"0"); }

	_int iTen_Sec = ((iPlayTime % 60) / 10);
	errno_t iCheck_TT = _itow_s(iTen_Sec, m_szTime_tenSec, sizeof(m_szTime_tenSec) / sizeof(m_szTime_tenSec[0]), 10);

	_int iOne_Sec = ((iPlayTime % 60) - (iTen_Sec * 10));
	errno_t iCheck_TS = _itow_s(iOne_Sec, m_szTime_oneSec, sizeof(m_szTime_oneSec) / sizeof(m_szTime_oneSec[0]), 10);

}

void CUI_FontMgr::Death_UI_Update(const _float & fTimeDelta)
{



}

#pragma endregion



// Only Rush
#pragma region

// 시간
void CUI_FontMgr::Rush_Update_PlayTIme(const _float & fTimeDelta)
{
	fRush_PlayTIme += fTimeDelta;

	if (fRush_PlayTIme >= Rush_One_Sec)
	{
		i_Rush_PlayTime = i_Rush_PlayTime + 1;
		fRush_PlayTIme = 0.f;
	}

	_int i_Rush_Ten_Min = i_Rush_PlayTime / 600;
	_int i_Rush_One_Min = (i_Rush_PlayTime / 60) % 10;
	_int i_Rush_Ten_Sec = ((i_Rush_PlayTime % 60) / 10); ;
	_int i_Rush_One_Sec = (i_Rush_PlayTime % 60) - (i_Rush_Ten_Sec * 10);

	szRush_One_Sec = to_wstring(i_Rush_One_Sec);
	szRush_Ten_Sec = to_wstring(i_Rush_Ten_Sec);
	szRush_One_Min = to_wstring(i_Rush_One_Min);
	szRush_Ten_Min = to_wstring(i_Rush_Ten_Min);

}

// 점수
void CUI_FontMgr::Update_Score(void)
{
	szScore = to_wstring(iRENDER_Score);
}

void CUI_FontMgr::RUSH_RENDER(void)
{
	// 시간
	Render_Font(L"Font_Retro", (szRush_Ten_Min).c_str(), &_vec2(WINCX_HALF - PERCENTX * 4.f, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	Render_Font(L"Font_Retro", (szRush_One_Min).c_str(), &_vec2(WINCX_HALF - PERCENTX * 2.f, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	Render_Font(L"Font_Retro", (szRushColon).c_str(), &_vec2(WINCX_HALF, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	Render_Font(L"Font_Retro", (szRush_Ten_Sec).c_str(), &_vec2(WINCX_HALF + PERCENTX * 2.f, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	Render_Font(L"Font_Retro", (szRush_One_Sec).c_str(), &_vec2(WINCX_HALF + PERCENTX * 4.f, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	// 스코어
	Render_Font(L"Font_Retro", (szScore).c_str(), &_vec2(WINCX - PERCENTX * 20.f, PERCENTY), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

#pragma endregion
