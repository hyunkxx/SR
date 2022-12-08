#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTerrainTex;
class CFloorTex;
class CTransform;

END

class CTerrain :	public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_Material(void);
	void		Key_Input(const _float& fTimeDelta);


private:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	_float				m_fSpeed = 10.f;

	CTexture*			m_pBackground = nullptr;
	CFloorTex*			m_pBackgroundRect;
	_matrix				m_matBackground;
public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

