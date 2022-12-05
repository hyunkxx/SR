#pragma once

#include "Base.h"
#include "Include.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement :	public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CLayer*		Get_Layer(const _tchar* pLayerTag);
	CGameObject*	Get_Object(const _tchar* pLayerTag, const _tchar* pObjTag);
	CComponent*		Get_Component(const _tchar* pLayerTag,
								  const _tchar* pObjTag,
								  const _tchar* pComponentTag,
								  COMPONENTID eID);
	HRESULT			Add_GameObject(const _tchar * pObjTag, CGameObject* pObject);
	class CLayer*   Find_Layer(const _tchar* pLayerTag);
public:
	HRESULT			Set_Scene(CScene* pScene);

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(void);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	void			RenderGUI();
private:
	CScene*			m_pScene;

public:
	virtual void	Free(void);
};

END

