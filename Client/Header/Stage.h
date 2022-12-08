#pragma once

#include "Scene.h"
#include "Include.h"
#include "SkyBox.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();
	
public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Environment_Object(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT				Ready_LightInfo(void);
private:
	void				CameraChangeSetting();
private:
	virtual		void		Collison_Object(void);
public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void) override;
	EData    m_eData;
	_float m_fSound = 0.3f;

	void Key_Input(const _float& fTimeDelta);

	wstring m_szOutput_Chatting = L"";
	wstring m_szInput_Chatting = L"";
	wstring m_szChatting = L"";
	_tchar	m_PlayerChating[128] = L"";
	_tchar	m_ChattingWin[128] = L"";

};

