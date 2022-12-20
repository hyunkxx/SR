#pragma once
#include "Include.h"

class CUI_FontMgr
{
	DECLARE_SINGLETON(CUI_FontMgr)
private:
	explicit CUI_FontMgr();
	virtual ~CUI_FontMgr();

	//	War ����
#pragma region

public:
	//function

	// ��Ʈ (��ü) �غ� -> �׳� main app �Ѳ����� �����ϱ��~
	//void Initialize(LPDIRECT3DDEVICE9 pGraphicDev);

	// �� �ð� �����ϴ� ���ڿ�
	void Update(const _float& fTimeDelta);


	// �������� �ð��� �޾� �� �Լ�
	const _int Get_Time(void) { return iPlayTime; }


	// �ð� ���ڿ� �޾� �� �Լ���
	const _tchar* Get_Time_Colon(void) { return m_szTime_colon; }
	const _tchar* Get_Time_MIn(void) { return m_szTime_min; }
	const _tchar* Get_Time_TenSec(void) { return m_szTime_tenSec; }
	const _tchar* Get_Time_OneSec(void) { return m_szTime_oneSec; }

	const _int Get_PlayTime(void) { return iPlayTime; }
	const void Plus_PlayTime(_int PlayTime) { iPlayTime += PlayTime; }

	// ���� ų ī��Ʈ
	const _tchar* Get_BlueTeam_Kill(void) { return m_szRedTeamDeath; }
	const _tchar* Get_RedTeam_Kill(void) { return m_szBlueTeamDeath; }

	// ��ũ �̸�
	const _tchar* Get_Tank_Name(void) {
		lstrcpy(m_szTank_Name, szPlayerName.c_str());
		return m_szTank_Name;
	}

	// ���� �޾� �� �Լ���
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
			szFullChat_Log = (wstring(L" ���� ���� �� ��ũ\t")) + szColon + to_wstring(iLiveCount) + wstring(L"/") + to_wstring(iAllCount);
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

	// �ð�(Ÿ�̸�) ������Ʈ
	void Time_Font_UI_Update(const _float& fTimeDelta);

	// ų(����) ���� ������Ʈ
	void Death_UI_Update(const _float& fTimeDelta);

	//// ������ ��Ʈ ������Ʈ
	//void Item_UI_Update(const _float& fTimeDelta);

	//value

	// ��ŸŸ������ �������� ���ѽð��� ���� ����
	_int iPlayTime = 599;

	// �ð� ī��Ʈ �Ǽ�
	_float m_fTime = 0.f;
	_float m_fOneSec = 1.f;

	// �ð� ���ڿ�
	_tchar m_szTime_min[8] = L"";
	_tchar m_szTime_tenSec[8] = L"";
	_tchar m_szTime_oneSec[8] = L"";
	_tchar	m_szTime_colon[8] = L" : ";

	// ��-ų(����) ����
	_tchar m_szBlueTeamDeath[8] = L"0";
	_tchar m_szRedTeamDeath[8] = L"0";

	// ��ũ �̸� // ��ũ ����
	_tchar m_szTank_Name[64] = L"��ũ �̸�/����";


	/////////////////////////////////////////////////////////////////////// ���� ����
	// START
	_tchar m_szStart[16] = L"";
	// END
	_tchar m_szEND[16] = L"";

	// ����
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


	// Rush ����
#pragma region
public:

	void Rush_Update_PlayTIme(const _float& fTimeDelta);



	void Update_Score(void);
	void Plus_Score(_int Plus_Score) { iRENDER_Score += Plus_Score; }
	_int Get_Score(void) { return iRENDER_Score; }
	//void Set_Score(_int Setting_Score) {	iRENDER_Score = Setting_Score;	}


	// ��� ���� ����
	void Set_Rush_LifeCount(_int LifeCount) { iRush_LifeCount = LifeCount; }
	_int Get_Rush_LifeCount(void) { return iRush_LifeCount; }
	void Rush_Plus_Life(void) { iRush_LifeCount += 1; if (iRush_LifeCount >= 10) { iRush_LifeCount = 10; } }
	void Rush_Minus_Life(void) { iRush_LifeCount -= 1; if (iRush_LifeCount <= 0) { iRush_LifeCount = 0; } }




	// Rush ������ ���� ������ ��.
	void RUSH_RENDER(void);

private:

	// Rush -> �ð�
	_float fRush_PlayTIme = 0.f;
	_float Rush_One_Sec = 1.f;
	_int i_Rush_PlayTime = 0;
	wstring szRushColon = L":";
	wstring szRush_One_Sec;
	wstring szRush_Ten_Sec;
	wstring szRush_One_Min;
	wstring szRush_Ten_Min;


	// Rush -> ���ھ�
	_int iRENDER_Score = 100;
	wstring szScore;



	// Rush -> Life Count
	_int iRush_LifeCount;

#pragma endregion



};

