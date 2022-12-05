#pragma once
#include "Base.h"

#include "GameObject.h"
#include "Bullet.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CGameObject*	Get_GameObject(const _tchar* pObjTag);
	CComponent*		Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);

public:
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pObject);

	HRESULT			Ready_Layer(void);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(void);
	void			RenderGUI(void);
public:
	unordered_map<const _tchar*, CGameObject*>* 		Get_mapObject(void) { return &m_mapObject; }
private:
	unordered_map<const _tchar*, CGameObject*>		m_mapObject;
public:
	static		CLayer*			Create(void);
	virtual		void			Free(void);
};

END
