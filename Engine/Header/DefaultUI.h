#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;

class ENGINE_DLL CDefaultUI : public CGameObject
{
public:
	static CDefaultUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* strName, const _tchar* path);
	virtual void Free(void) override;
private:
	explicit CDefaultUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* strName, const _tchar* path);
	virtual ~CDefaultUI();
public:
	virtual	HRESULT	Ready_Object(void);
	virtual	_int	Update_Object(const _float& fTimeDelta);
	virtual	void	LateUpdate_Object(void);
	virtual	void	Render_Object(void);
	virtual	void	RenderGUI(void);
public:
	_vec3 GetPosition() { return m_vPos; }
	_vec3 GetScale() { return m_vScale; }

	void SetPosition(const _vec3& vPos) { m_vPos = vPos; }
	void SetScale(const _vec3& vScale) { m_vScale = vScale; }
protected:
	HRESULT AddComponent();
protected:
	_vec3 m_vPos;
	_vec3 m_vScale;
	_matrix m_matProj;

	CTransform* m_pTransform;
	CTexture*	m_pTexture;
	CRcTex*		m_pRectTexture;

	wstring m_strName;
	wstring m_strPath;
};
END