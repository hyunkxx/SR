#pragma once
#include "Scene.h"

BEGIN(Engine)
class CGameObject;
END

class CModeSelectMenu : public CScene
{
private:
	explicit CModeSelectMenu(LPDIRECT3DDEVICE9 pGraphic);
	virtual ~CModeSelectMenu();
public:
	virtual void Free(void) override;
public:
	static CModeSelectMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual void Collison_Object(void) override;
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;
private:
	HRESULT	Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_UI(const _tchar* pLayerTag);
public:
	void KeyInput();
private:
	float m_fSound = 1.0;

	CGameObject* m_pCamera;
	_vec3 m_vCamPosition;

	CGameObject* m_pBackground;

	/* GMODE Button*/
	CGameObject* m_pWarButton;
	CGameObject* m_pRushButton;
	CGameObject* m_pStartButton;
	CGameObject* m_pExitButton;
};