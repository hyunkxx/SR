#pragma once
#include "Scene.h"

BEGIN(Engine)
class CGameObject;
END

class CMainMenu : public CScene
{
private:
	explicit CMainMenu(LPDIRECT3DDEVICE9 pGraphic);
	virtual ~CMainMenu();
public:
	virtual void Free(void) override;
public:
	static CMainMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);
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
	void	CreateVehicle();
public:
	void KeyInput();
	void UpdateCam(const _float & fTimeDelta);
private:
	float m_fSound = 1.0;
	int m_nCurrentIndex;

	CGameObject* m_pVehicle[(UINT)VEHICLE::MAX];
	CGameObject* m_pCamera;

	_vec3 m_vCamPosition;

	CGameObject* m_pBackground;
	CGameObject* m_pGameStartMenu;
};