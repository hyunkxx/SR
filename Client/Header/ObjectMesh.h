#pragma once

#include "GameObject.h"
#include "Collisionable.h"

BEGIN(Engine)
class CVoxel;
END

class CObjectMesh : public CGameObject
{
public:
	static CObjectMesh* Create(LPDIRECT3DDEVICE9 pGraphic, wstring strMeshName);
private:
	explicit CObjectMesh(LPDIRECT3DDEVICE9 pGraphic, wstring strMeshName);
	virtual ~CObjectMesh();
public:
	virtual void Free() override;

	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
	virtual void	RenderGUI(void) override;

public:
	virtual	HRESULT	Add_Component(void);
	virtual HRESULT	Ready_Object(void);
	const	 _vec3  Get_Info(void);
	void			Move_Info(_vec3 _Info);
public:
	void SetPosition(const _vec3& vPosition) {
		m_vPosition = vPosition;
		m_vOriginPosition = { m_vPosition.x , m_vPosition.y, m_vPosition.z };
		m_vViewPosition = { m_vPosition.x - 5.f , m_vPosition.y - 10.f , m_vPosition.z };
	}
	void KeyInput();

	void Rotation(const float& fDeltaTime);
	void Selected();
	void UnSelect();
private:
	_matrix		m_matWorld;
	_vec3		m_vPosition;
	_float		m_fAngle;

	wstring m_strMeshName;
	bool m_bSeleted;
	CVoxel* m_pBodyMesh;
	CVoxel* m_pHeadMesh;
	CVoxel* m_PosinMesh;

	_vec3 m_vViewPosition;
	_vec3 m_vOriginPosition;
};
