#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CGrass : public Engine::CGameObject
{
public:
	enum class TYPE { GRASS1, GRASS2, GRASS3 };
private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType);
	explicit CGrass(const CGrass& rhs);
	virtual ~CGrass();
public:
	static CGrass*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TYPE eType);
public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const _vec3	Get_Info(void) { return _vec3(); }
	virtual		  void	Move_Info(_vec3 _Info) {}
private:
	virtual void Free(void) override;
	HRESULT		Add_Component(void);
public:
	static int ID;
	const wstring& GetID() { return m_ID; }
	void Set_ScaleX(const _float& fX) { m_fScaleX = fX; m_bUpdateTransform = true; }
	void Set_ScaleY(const _float& fY) { m_fScaleY = fY; m_bUpdateTransform = true; }
	void Set_ScaleZ(const _float& fZ) { m_fScaleZ = fZ; m_bUpdateTransform = true; }

	void Set_PosX(const _float& fX) { m_fPosX = fX; m_bUpdateTransform = true; }
	void Set_PosY(const _float& fY) { m_fPosY = fY; m_bUpdateTransform = true; }
	void Set_PosZ(const _float& fZ) { m_fPosZ = fZ; m_bUpdateTransform = true; }

	void SetTransform(const _vec3& vPos, float angle)
	{
		m_fPosX = vPos.x;
		m_fPosY = vPos.y;
		m_fPosZ = vPos.z;
		iRotaion = (int)angle;

		m_bUpdateTransform = true;
	}
private:
	void UpdateTransform();
	void RenderImage();
private: /* UI value */
	wstring m_ID;

	TYPE m_eType;
	bool m_bUpdateTransform;

	CRcTex*		m_pRcTex1 = nullptr;
	CRcTex*		m_pRcTex2 = nullptr;

	CTexture*	m_pTexture = nullptr;

	CTransform*	m_pTransform = nullptr;
	CTransform*	m_pTransformTwo = nullptr;

	_matrix  m_matProj;

	_float	m_fScaleX;
	_float	m_fScaleY;
	_float	m_fScaleZ;

	_float	m_fPosX;
	_float	m_fPosY;
	_float	m_fPosZ;

	_int	iRotaion;
private:
	const _float m_fOriginY = -100.f;
	const _float m_fDeltaY = +110.f;
private:
};

