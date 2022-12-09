#pragma once
#include "Base.h"
#include "Include.h"


BEGIN(Engine)

class CCamera;

class ENGINE_DLL CCameraMgr :
	public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

public:
	explicit CCameraMgr();
	virtual ~CCameraMgr();
public:
	HRESULT	Add_Camera(const _tchar* CameraTag, CCamera* pCamera);
	void	Update_CameraMgr(const _float& fTimeDelta);
	void	LateUpdate_CameraMgr(void);
	void	Camera_Change(const _tchar* CameraTag);
	CAMERA_ID	Get_Camera_ID(void);
	CCamera*	Get_Camera(void);
	CCamera*	Get_Camera(const _tchar* CameraTag);
private:
	unordered_map<const _tchar*, CCamera*>		m_mapObject;
	CCamera*	m_pCamera = nullptr;
public:
	virtual void Free(void)override;
};

END