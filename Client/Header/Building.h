#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CVoxel;
class CTransform;
END

class CBuilding : public CGameObject
{
	static _int ID;
public:
	static CBuilding* Create(LPDIRECT3DDEVICE9 pGramphicDev, const _vec3& vPos);
	virtual void Free(void);
	virtual ~CBuilding();
private:
	explicit CBuilding(LPDIRECT3DDEVICE9 pGramphicDev, const _vec3& vPos);
	explicit CBuilding(const CBuilding& rhs);
public:
	virtual	HRESULT	Ready_Object(void) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	void	LateUpdate_Object(void) override;
	virtual	void	Render_Object(void) override;
	virtual	void	RenderGUI(void) override;
	virtual _bool	Get_Dead(void) { return m_bDead; }
public:
	const _vec3& GetPosition() const { return m_vPosition; }
	void SetPosition(const _vec3& vPos) { m_vPosition = vPos; }
	wstring& GetID() { return m_ID; }
private:
	HRESULT AddComponent();
private:
	wstring m_ID;
	_matrix m_matWorld;
	_vec3	m_vPosition;

	CVoxel* m_pMesh;
	CComponent* m_pTransform;
};