#include "stdafx.h"
#include "..\Header\ResultUI.h"

#include "Export_Function.h"
#include "TankSet.h"
#include "TankManager.h"
#include "GameMode.h"
#include "Utility.h"
#include "ModeSelectMenu.h"
#include "RushMode.h"

CResultUI::CResultUI(LPDIRECT3DDEVICE9 pGraphicDev, MODE eType)
	: Engine::CGameObject(pGraphicDev)
	, m_eType(eType)
	, m_bShow(false)
	, m_bUpdateTransform(true)
{
}

CResultUI::CResultUI(const CResultUI& rhs)
	: Engine::CGameObject(rhs)
	, m_bShow(false)
	, m_bUpdateTransform(true)
	, m_eType(rhs.m_eType)
{
}


CResultUI::~CResultUI()
{
}

CResultUI* CResultUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, MODE eType)
{
	CResultUI*	pInstance = new CResultUI(pGraphicDev, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CResultUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CResultUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 800.f;
	m_fScaleY = 600.f;
	m_fScaleZ = 0.001f;

	m_fPosX = WINCX * 0.5f;
	m_fPosY = WINCY * 0.5f;
	m_fPosZ = 0.1f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_strText[0] = L"WINNER";
	m_strText[1] = L" WINNER";
	m_strText[2] = L" CHICKEN";
	m_strText[3] = L" DINNER!";

	m_strLose[0] = L"괜찮아, ";
	m_strLose[1] = L"이런 날도 ";
	m_strLose[2] = L"있는 거지 ";
	m_strLose[3] = L"뭐... 다시해보자";

	return S_OK;
}

_int CResultUI::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	KeyInput();
	UpdateTransform();

	if (m_eType == MODE::WAR)
	{
		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::WIN)
		{
			StopSound(STAGE_SOUND);
			Engine::PlaySound_SR(L"Victory.mp3", POPUP_SOUND, 1.f);
			SetWinText(fTimeDelta);
		}

		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::LOSE)
		{
			StopSound(STAGE_SOUND);
			Engine::PlaySound_SR(L"Defeat.mp3", POPUP_SOUND, 1.f);
			SetLoseText(fTimeDelta);
		}
	}

	else if (m_eType == MODE::RUSH)
	{
		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::WIN)
		{
			StopSound(STAGE_SOUND);
			Engine::PlaySound_SR(L"Victory.mp3", POPUP_SOUND, 1.f);
			SetWinText(fTimeDelta);
		}

		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::LOSE)
		{
			StopSound(STAGE_SOUND);
			Engine::PlaySound_SR(L"Defeat.mp3", POPUP_SOUND, 1.f);
			SetLoseText(fTimeDelta);
		}
	}



	return 0;
}

void CResultUI::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CResultUI::Render_Object(void)
{
	RenderButton();
}

void CResultUI::Free(void)
{
	__super::Free();
}

HRESULT CResultUI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"rect_texture", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"transform", pComponent });

	pComponent = m_pNonClickTexture = static_cast<CTexture*>(Clone_Prototype(L"game_result"));
	NULL_CHECK_RETURN(m_pNonClickTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"texture_non", pComponent });

	return S_OK;
}

void CResultUI::KeyInput()
{
}

void CResultUI::UpdateTransform()
{
	if (m_bUpdateTransform)
	{
		m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);
		m_bUpdateTransform = false;
	}
}

void CResultUI::SetWinText(const _float & fDeltaTime)
{
	if (m_eType == MODE::WAR)
	{
		if (m_nTitleCount < 4)
			m_fTitleTimer += fDeltaTime;

		if (m_fTitleTimer >= 0.3f)
		{
			if (m_nTitleCount >= 4)
				return;

			m_nTitleCount++;
			m_fTitleTimer = 0.f;
		}

		if (m_nTitleCount >= 4)
		{
			if (m_nTextCount < 3)
				m_fTextTimer += fDeltaTime;

			if (m_fTextTimer > 0.3f)
			{
				if (m_nTextCount >= 3)
					return;

				Engine::StopSound(PAPER_SOUND);
				Engine::PlaySound_SR(L"Paper.wav", PAPER_SOUND, 1.f);

				m_nTextCount++;
				m_fTextTimer = 0.f;
			}
		}

		switch (m_nTitleCount)
		{
		case 0:
			m_strBuff = L"";
			break;
		case 1:
			m_strBuff = m_strText[0];
			break;
		case 2:
			m_strBuff = m_strText[0] + m_strText[1];
			break;
		case 3:
			m_strBuff = m_strText[0] + m_strText[1] + m_strText[2];
			break;
		case 4:
			m_strBuff = m_strText[0] + m_strText[1] + m_strText[2] + m_strText[3];
			break;
		}

		m_strCreateText = L"생성한 차량 " + to_wstring(CGameMode::GetInstance()->m_nCreateCount);
		m_strDestroyText = L"파괴한 차량 " + to_wstring(CGameMode::GetInstance()->m_nDestroyCount);
		m_strKillText = L"파괴된 횟수 " + to_wstring(CGameMode::GetInstance()->m_nKillCount);
	}

	else if (m_eType == MODE::RUSH)
	{
		if (m_nTitleCount < 4)
			m_fTitleTimer += fDeltaTime;

		if (m_fTitleTimer >= 0.3f)
		{
			if (m_nTitleCount >= 4)
				return;

			m_nTitleCount++;
			m_fTitleTimer = 0.f;
		}

		switch (m_nTitleCount)
		{
		case 0:
			m_strBuff = L"";
			break;
		case 1:
			m_strBuff = m_strText[0];
			break;
		case 2:
			m_strBuff = m_strText[0] + m_strText[1];
			break;
		case 3:
			m_strBuff = m_strText[0] + m_strText[1] + m_strText[2];
			break;
		case 4:
			m_strBuff = m_strText[0] + m_strText[1] + m_strText[2] + m_strText[3];
			break;
		}

		m_strCreateText = L"";
		m_strDestroyText = L"";
		m_strKillText = L"";
	}
}


void CResultUI::SetLoseText(const _float & fDeltaTime)
{
	if (m_eType == MODE::WAR)
	{
		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::LOSE)
		{
			if (m_nTitleCount < 4)
				m_fTitleTimer += fDeltaTime;

			if (m_fTitleTimer >= 0.3f)
			{
				if (m_nTitleCount >= 4)
					return;

				m_nTitleCount++;
				m_fTitleTimer = 0.f;
			}

			if (m_nTitleCount >= 4)
			{
				if (m_nTextCount < 3)
					m_fTextTimer += fDeltaTime;

				if (m_fTextTimer > 0.3f)
				{
					if (m_nTextCount >= 3)
						return;

					Engine::StopSound(PAPER_SOUND);
					Engine::PlaySound_SR(L"Paper.wav", PAPER_SOUND, 1.f);

					m_nTextCount++;
					m_fTextTimer = 0.f;
				}
			}


			switch (m_nTitleCount)
			{
			case 0:
				m_strBuff = L"";
				break;
			case 1:
				m_strBuff = m_strLose[0];
				break;
			case 2:
				m_strBuff = m_strLose[0] + m_strLose[1];
				break;
			case 3:
				m_strBuff = m_strLose[0] + m_strLose[1] + m_strLose[2];
				break;
			case 4:
				m_strBuff = m_strLose[0] + m_strLose[1] + m_strLose[2] + m_strLose[3];
				break;
			}

			m_strCreateText = L"생성한 차량 " + to_wstring(CGameMode::GetInstance()->m_nCreateCount);
			m_strDestroyText = L"파괴한 차량 " + to_wstring(CGameMode::GetInstance()->m_nDestroyCount);
		}

		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::WIN)
		{
			if (m_nTitleCount < 4)
				m_fTitleTimer += fDeltaTime;

			if (m_fTitleTimer >= 0.3f)
			{
				if (m_nTitleCount >= 4)
					return;

				m_nTitleCount++;
				m_fTitleTimer = 0.f;
			}

			if (m_nTitleCount >= 4)
			{
				if (m_nTextCount < 3)
					m_fTextTimer += fDeltaTime;

				if (m_fTextTimer > 0.3f)
				{
					if (m_nTextCount >= 3)
						return;

					Engine::StopSound(PAPER_SOUND);
					Engine::PlaySound_SR(L"Paper.wav", PAPER_SOUND, 1.f);

					m_nTextCount++;
					m_fTextTimer = 0.f;
				}
			}

			switch (m_nTitleCount)
			{
			case 0:
				m_strBuff = L"";
				break;
			case 1:
				m_strBuff = m_strText[0];
				break;
			case 2:
				m_strBuff = m_strText[0] + m_strText[1];
				break;
			case 3:
				m_strBuff = m_strText[0] + m_strText[1] + m_strText[2];
				break;
			case 4:
				m_strBuff = m_strText[0] + m_strText[1] + m_strText[2] + m_strText[3];
				break;
			}

			m_strCreateText = L"생성한 차량 " + to_wstring(CGameMode::GetInstance()->m_nCreateCount);
			m_strDestroyText = L"파괴한 차량 " + to_wstring(CGameMode::GetInstance()->m_nDestroyCount);
			m_strKillText = L"파괴된 횟수 " + to_wstring(CGameMode::GetInstance()->m_nKillCount);
		}
	}

	else if (m_eType == MODE::RUSH)
	{
		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::LOSE)
		{
			if (m_nTitleCount < 4)
				m_fTitleTimer += fDeltaTime;

			if (m_fTitleTimer >= 0.3f)
			{
				if (m_nTitleCount >= 4)
					return;

				m_nTitleCount++;
				m_fTitleTimer = 0.f;
			}

			switch (m_nTitleCount)
			{
			case 0:
				m_strBuff = L"";
				break;
			case 1:
				m_strBuff = m_strLose[0];
				break;
			case 2:
				m_strBuff = m_strLose[0] + m_strLose[1];
				break;
			case 3:
				m_strBuff = m_strLose[0] + m_strLose[1] + m_strLose[2];
				break;
			case 4:
				m_strBuff = m_strLose[0] + m_strLose[1] + m_strLose[2] + m_strLose[3];
				break;
			}
		}
		else if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::WIN)
		{
			if (m_nTitleCount < 4)
				m_fTitleTimer += fDeltaTime;

			if (m_fTitleTimer >= 0.3f)
			{
				if (m_nTitleCount >= 4)
					return;

				m_nTitleCount++;
				m_fTitleTimer = 0.f;
			}

			switch (m_nTitleCount)
			{
			case 0:
				m_strBuff = L"";
				break;
			case 1:
				m_strBuff = m_strText[0];
				break;
			case 2:
				m_strBuff = m_strText[0] + m_strText[1];
				break;
			case 3:
				m_strBuff = m_strText[0] + m_strText[1] + m_strText[2];
				break;
			case 4:
				m_strBuff = m_strText[0] + m_strText[1] + m_strText[2] + m_strText[3];
				break;
			}
		}
	}
}

void CResultUI::RenderButton()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	_matrix	ViewMatrix, OldProjection, OldView;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjection);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	if (m_eType == MODE::WAR)
	{
		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::WIN)
		{
			m_pNonClickTexture->Set_Texture(0);
			m_pRcTex->Render_Buffer();

			_vec2 vPos{ 50.f, 50.f };
			Render_Font(L"Font_RetroBig", L"BATTLE RESULT", &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

			vPos = { 315.f, 50.f };
			Render_Font(L"Font_RetroBig", L"VICTORY", &vPos, D3DCOLOR_ARGB(255, 60, 210, 230));

			vPos = { 50.f, 100.f };
			Render_Font(L"Font_RetroBig", m_strBuff.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));

			if (m_nTextCount == 1)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
			else if (m_nTextCount == 2)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 파괴한 차량 수 */
				vPos = { 50.f, 230.f };
				Render_Font(L"Font_RetroMiddle", m_strDestroyText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
			else if (m_nTextCount == 3)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 파괴한 차량 수 */
				vPos = { 50.f, 230.f };
				Render_Font(L"Font_RetroMiddle", m_strDestroyText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 죽은 횟수*/
				vPos = { 50.f, 290.f };
				Render_Font(L"Font_RetroMiddle", m_strKillText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
		}

		if (CGameMode::GetInstance()->m_eGameResult == CGameMode::GAME_RESULT::LOSE)
		{
			m_pNonClickTexture->Set_Texture(0);
			m_pRcTex->Render_Buffer();

			_vec2 vPos{ 50.f, 50.f };
			Render_Font(L"Font_RetroBig", L"BATTLE RESULT", &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

			vPos = { 315.f, 50.f };
			Render_Font(L"Font_RetroBig", L"DEFEAT", &vPos, D3DCOLOR_ARGB(255, 255, 90, 90));

			vPos = { 50.f, 100.f };
			Render_Font(L"Font_RetroBig", m_strBuff.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));

			if (m_nTextCount == 1)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
			else if (m_nTextCount == 2)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 파괴한 차량 수 */
				vPos = { 50.f, 230.f };
				Render_Font(L"Font_RetroMiddle", m_strDestroyText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
			else if (m_nTextCount == 3)
			{
				/* 생성한 차량 수 */
				vPos = { 50.f, 170.f };
				Render_Font(L"Font_RetroMiddle", m_strCreateText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 파괴한 차량 수 */
				vPos = { 50.f, 230.f };
				Render_Font(L"Font_RetroMiddle", m_strDestroyText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));

				/* 죽은 횟수*/
				vPos = { 50.f, 290.f };
				Render_Font(L"Font_RetroMiddle", m_strKillText.c_str(), &vPos, D3DCOLOR_ARGB(255, 240, 240, 240));
			}
		}
	}

	else if (m_eType == MODE::RUSH)
	{
		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::WIN)
		{
			m_pNonClickTexture->Set_Texture(0);
			m_pRcTex->Render_Buffer();

			_vec2 vPos{ 50.f, 50.f };
			Render_Font(L"Font_RetroBig", L"BATTLE RESULT", &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

			vPos = { 315.f, 50.f };
			Render_Font(L"Font_RetroBig", L"VICTORY", &vPos, D3DCOLOR_ARGB(255, 60, 210, 230));

			vPos = { 50.f, 100.f };
			Render_Font(L"Font_RetroBig", m_strBuff.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));
		}

		if (CRushMode::GetInstance()->m_eResult == CRushMode::GAME_RESULT::LOSE)
		{
			m_pNonClickTexture->Set_Texture(0);
			m_pRcTex->Render_Buffer();

			_vec2 vPos{ 50.f, 50.f };
			Render_Font(L"Font_RetroBig", L"BATTLE RESULT", &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

			vPos = { 315.f, 50.f };
			Render_Font(L"Font_RetroBig", L"DEFEAT", &vPos, D3DCOLOR_ARGB(255, 255, 90, 90));

			vPos = { 50.f, 100.f };
			Render_Font(L"Font_RetroBig", m_strBuff.c_str(), &vPos, D3DCOLOR_ARGB(255, 255, 255, 0));
		}
	}


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjection);
}