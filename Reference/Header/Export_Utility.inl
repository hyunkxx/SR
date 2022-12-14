#include "Export_Utility.h"

HRESULT			Ready_Prototype(const _tchar* pComponentTag, CComponent* pComponent)
{
	return CComponentMgr::GetInstance()->Ready_Prototype(pComponentTag, pComponent);
}
CComponent*		Clone_Prototype(const _tchar* pComponentTag)
{
	return CComponentMgr::GetInstance()->Clone_Prototype(pComponentTag);
}
CLayer*		Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}
CGameObject*	Get_Object(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_Object(pLayerTag, pObjTag);
}
CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

void Delete_Object(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	CManagement::GetInstance()->Delete_Object(pLayerTag, pObjTag);
}

HRESULT			Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppInstance)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);


	*ppInstance = pManagement;

	return S_OK;
}
HRESULT			Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int			Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void				LateUpdate_Scene(void)
{
	CManagement::GetInstance()->LateUpdate_Scene();
}
void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

void RenderGUI()
{
	CManagement::GetInstance()->RenderGUI();
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}
// LightMgr

HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

inline void Initalize()
{
	CMeshLoader::GetInstance()->Initalize();
}

inline void LoadMesh(wstring strFileName, wstring key)
{
	CMeshLoader::GetInstance()->LoadMesh(strFileName, key);
}

inline vector<CCube*> GetMeshData(wstring key)
{
	return CMeshLoader::GetInstance()->GetMeshData(key);
}
void				Release_Utility(void)
{
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CComponentMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CMeshLoader::GetInstance()->DestroyInstance();
}
