#include "..\..\Header\CameraMgr.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}


CCameraMgr::~CCameraMgr()
{
	Free();
}

HRESULT CCameraMgr::Add_Camera(const _tchar * CameraTag, CCamera * pCamera)
{
	if (!pCamera)
		return E_FAIL;

	m_mapObject.insert({ CameraTag ,pCamera });

	return  S_OK;
}

void CCameraMgr::Update_CameraMgr(const _float & fTimeDelta)
{
	if (m_pCamera)
		m_pCamera->Update_Object(fTimeDelta);
}

void CCameraMgr::LateUpdate_CameraMgr(void)
{
	if (m_pCamera)
		m_pCamera->LateUpdate_Object();
}

void CCameraMgr::Camera_Change(const _tchar * CameraTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(CameraTag));

	if (iter == m_mapObject.end())
		return;

	m_pCamera = iter->second;
}

CAMERA_ID CCameraMgr::Get_Camera_ID(void)
{
	return m_pCamera->Get_ID();
}

CCamera * CCameraMgr::Get_Camera(void)
{
	return m_pCamera;
}

CCamera * CCameraMgr::Get_Camera(const _tchar * CameraTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(CameraTag));

	return iter->second;
}

void CCameraMgr::Free(void)
{
	m_pCamera = nullptr;

	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
