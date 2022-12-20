#pragma once
#include "Include.h"

class CUI_FontMgr
{
	DECLARE_SINGLETON(CUI_FontMgr)
private:
	explicit CUI_FontMgr();
	virtual ~CUI_FontMgr();

	//	War 전용
#pragma region

public:
	//function

	// 폰트 (서체) 준비 -> 그냥 main app 한꺼번에 실행하기로~
	//void Initialize(LPDIRECT3DDEVICE9 pGraphicDev);

	// 매 시간 갱신하는 문자열
	void Update(const _float& fTimeDelta);


	// 스테이지 시간을 받아 갈 함수
	const _int Get_Time(void) { return iPlayTime; }


	// 시간 문자열 받아 갈 함수들
	const _tchar* Get_Time_Colon(void) { return m_szTime_colon; }
	const _tchar* Get_Time_MIn(void) { return m_szTime_min; }
	const _tchar* Get_Time_TenSec(void) { return m_szTime_tenSec; }
	const _tchar* Get_Time_OneSec(void) { return m_szTime_oneSec; }

	const _int Get_PlayTime(void) { return iPlayTime; }
	const void Plus_PlayTime(_int PlayTime) { iPlayTime += PlayTime; }

	// 팀의 킬 카운트
	const _tchar* Get_BlueTeam_Kill(void) { return m_szRedTeamDeath; }
	const _tchar* Get_RedTeam_Kill(void) { return m_szBlueTeamDeath; }

	// 탱크 이름
	const _tchar* Get_Tank_Name(void) {
		lstrcpy(m_szTank_Name, szPlayerName.c_str());
		return m_szTank_Name;
	}

	// 색상값 받아 갈 함수들
	const D3DXCOLOR Get_Black(void) { return Font_Col_Black; }
	const D3DXCOLOR Get_White(void) { return Font_Col_White; }
	const D3DXCOLOR Get_blue(void) { return Font_Col_blue; }
	const D3DXCOLOR Get_Red(void) { return Font_Col_Red; }
	const D3DXCOLOR Get_Green(void) { return Font_Col_Green; }
	const D3DXCOLOR Get_Yellow(void) { return Font_Col_Yellow; }
	const D3DXCOLOR Get_Hecks_B(void) { return Font_Col_Hec_B; }
	const D3DXCOLOR Get_Hecks_R(void) { return Font_Col_Hec_R; }
	//value							  

	void				SavePlayerName(wstring PlayerName) { szPlayerName = PlayerName; }
	wstring				Get_PlayerName(void) { return szPlayerName; }

	_uint			iAllCount{ 0 }, iLiveCount{ 0 };
	_uint			Get_LiveCount(void) { return iLiveCount; }
	void				Set_AllCount(_uint _AllCount) { iLiveCount = iAllCount = _AllCount; }
	void				Set_LiveCount(_uint DeathCount) { iLiveCount -= DeathCount; }

	wstring check = L"NUM";
	void				SendChatLog(wstring Name, wstring OriginChat) {
		if (OriginChat == check)
		{
			if (iLiveCount >= iAllCount) { iLiveCount = iAllCount; }
			if (iLiveCount <= 0) { iLiveCount = 0; }
			szFullChat_Log = (wstring(L" 적팀 남은 적 탱크\t")) + szColon + to_wstring(iLiveCount) + wstring(L"/") + to_wstring(iAllCount);
		}
		else if (!((OriginChat == L"") || (OriginChat == L" ") || (OriginChat == L"  ")))
		{
			szFullChat_Log = Name + szColon + OriginChat;
		}
	}

	const wstring	Get_Chat_Log(void) { return szFullChat_Log; }


protected:
	//function
	//value
private:
	//function

	// 시간(타이머) 업데이트
	void Time_Font_UI_Update(const _float& fTimeDelta);

	// 킬(데스) 숫자 업데이트
	void Death_UI_Update(const _float& fTimeDelta);

	//// 아이템 폰트 업데이트
	//void Item_UI_Update(const _float& fTimeDelta);

	//value

	// 델타타임으로 스테이지 제한시간을 만들 정수
	_int iPlayTime = 599;

	// 시간 카운트 실수
	_float m_fTime = 0.f;
	_float m_fOneSec = 1.f;

	// 시간 문자열
	_tchar m_szTime_min[8] = L"";
	_tchar m_szTime_tenSec[8] = L"";
	_tchar m_szTime_oneSec[8] = L"";
	_tchar	m_szTime_colon[8] = L" : ";

	// 팀-킬(데스) 숫자
	_tchar m_szBlueTeamDeath[8] = L"0";
	_tchar m_szRedTeamDeath[8] = L"0";

	// 탱크 이름 // 탱크 종류
	_tchar m_szTank_Name[64] = L"탱크 이름/종류";


	/////////////////////////////////////////////////////////////////////// 수정 시작
	// START
	_tchar m_szStart[16] = L"";
	// END
	_tchar m_szEND[16] = L"";

	// 색상값
	D3DXCOLOR Font_Col_Black{ 0.f , 0.f, 0.f, 1.f };
	D3DXCOLOR Font_Col_White{ 1.f , 1.f, 1.f, 1.f };
	D3DXCOLOR Font_Col_blue{ 0.f , 0.f, 1.f, 1.f };
	D3DXCOLOR Font_Col_Red{ 1.f , 0.f, 0.f, 1.f };
	D3DXCOLOR Font_Col_Green{ 0.f , 1.f, 0.f, 1.f };
	D3DXCOLOR Font_Col_Yellow{ 1.f , 1.f, 0.f, 1.f };
	D3DXCOLOR Font_Col_Hec_B{ 0.f , 0.9608f, 1.f, 1.f };
	D3DXCOLOR Font_Col_Hec_R{ 1.f , 0.251f, 0.251f, 1.f };

	wstring		szColon = L":";
	wstring		szPlayerName = L"";
	wstring		szFullChat_Log = L"";

#pragma endregion


	// Rush 전용
#pragma region
public:

	void Rush_Update_PlayTIme(const _float& fTimeDelta);



	void Update_Score(void);
	void Plus_Score(_int Plus_Score) { iRENDER_Score += Plus_Score; }
	_int Get_Score(void) { return iRENDER_Score; }
	//void Set_Score(_int Setting_Score) {	iRENDER_Score = Setting_Score;	}


	// 목숨 갯수 셋팅
	void Set_Rush_LifeCount(_int LifeCount) { iRush_LifeCount = LifeCount; }
	_int Get_Rush_LifeCount(void) { return iRush_LifeCount; }
	void Rush_Plus_Life(void) { iRush_LifeCount += 1; if (iRush_LifeCount >= 10) { iRush_LifeCount = 10; } }
	void Rush_Minus_Life(void) { iRush_LifeCount -= 1; if (iRush_LifeCount <= 0) { iRush_LifeCount = 0; } }




	// Rush 씬에서 렌더 돌리는 것.
	void RUSH_RENDER(void);

private:

	// Rush -> 시간
	_float fRush_PlayTIme = 0.f;
	_float Rush_One_Sec = 1.f;
	_int i_Rush_PlayTime = 0;
	wstring szRushColon = L":";
	wstring szRush_One_Sec;
	wstring szRush_Ten_Sec;
	wstring szRush_One_Min;
	wstring szRush_Ten_Min;


	// Rush -> 스코어
	_int iRENDER_Score = 100;
	wstring szScore;



	// Rush -> Life Count
	_int iRush_LifeCount;

#pragma endregion



};

