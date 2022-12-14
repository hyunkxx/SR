#pragma once
#include "Base.h"
#include "Include.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CLayer*				Get_Layer(const _tchar* pLayerTag);
	CGameObject*		Get_Object(const _tchar* pLayerTag, const _tchar* pObjTag);
	CComponent*			Get_Component(const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag,
		COMPONENTID eID);
	HRESULT				Add_GameObject(const _tchar * pObjTag, CGameObject* pObject);
	CGameObject*		Swap_Object(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pObj);
	void				Delete_Object(const _tchar* pLayerTag, const _tchar* pObjTag);
protected:
	virtual		void		Collison_Object(void)PURE;
public:
	virtual		HRESULT		Ready_Scene(void);
	virtual		_int		Update_Scene(const _float& fTimeDelta);
	virtual		void		LateUpdate_Scene(void);
	virtual		void		Render_Scene(void) {}
	virtual		void		RenderGUI(void);
protected:
	LPDIRECT3DDEVICE9							m_pGraphicDev;
	unordered_map<const _tchar*, CLayer*>		m_umapLayer;

public:
	virtual void	Free(void);
};

END