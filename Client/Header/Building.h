#pragma once
#include "GameObject.h"
#include "Collisionable.h"
BEGIN(Engine)
class CVoxel;
class CTransform;
END

class CBuilding : public CGameObject, public ICollisionable
{
public:
	enum class TYPE { BUILDING, ROCK, PLANT_1, PLANT_2, PLANT_3, MAX };
	static _int ID;
public:
	static CBuilding* Create(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos, TYPE eType);
	virtual void Free(void);
	virtual ~CBuilding();
private:
	explicit CBuilding(LPDIRECT3DDEVICE9 pGramphicDev, wstring strMeshKey, const _vec3& vPos, TYPE eType);
	explicit CBuilding(const CBuilding& rhs);
public:
	virtual	HRESULT	Ready_Object(void) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	void	LateUpdate_Object(void) override;
	virtual	void	Render_Object(void) override;
	virtual	void	RenderGUI(void) override;
	virtual _bool	Get_Dead(void) { return m_bDead; }
public:
	virtual const   _vec3 Get_Info(void) override;
	virtual			void  Move_Info(_vec3 _Info) override;
	virtual			void  OBB_Collision_EX(void) override;
	virtual			void  Update_OBB(void) override;
	virtual			OBB*  Get_OBB(void) override;
public:
	wstring& GetID() { return m_ID; }
	const _vec3& GetRotation() const { return m_vRotation; }
	void SetRotation(const _vec3& vRot);
	const _vec3& GetPosition() const { return m_vPosition; }
	void SetPosition(const _vec3& vPos);
private:
	void	CollisionSetting();
	HRESULT AddComponent();
private:
	TYPE	m_eType;
	wstring m_ID;
	_matrix m_matWorld;
	_vec3	m_vPosition;
	_vec3	m_vRotation;

	wstring m_strMeshKey;
	CVoxel* m_pMesh;
	CComponent* m_pTransform;
};