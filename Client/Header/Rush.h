#pragma once

#include "Scene.h"
#include "Include.h"
#include "SkyBox.h"

class CRush : public Engine::CScene
{
private:
	explicit CRush(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRush();

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
	static CRush*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free(void) override;
private:
	EData    m_eData;
	CGameObject* m_pEffectManager;
	CGameObject* m_pFactoryUI;
};

