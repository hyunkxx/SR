#pragma once

#include "Include.h"
#include "Base.h"
#include "GraphicDev.h"
#include "Logo.h"

BEGIN(Engine)

class CManagement;

END

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	int			Update_MainApp(const _float& fTimeDelta);
	void		LateUpdate_MainApp(void);
	void		Render_MainApp(void);

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);

private:
	Engine::CGraphicDev*		m_pDeviceClass = nullptr;
	Engine::CManagement*		m_pManagementClass = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	static		CMainApp*		Create(void);

private:
	virtual		void			Free(void);

};

