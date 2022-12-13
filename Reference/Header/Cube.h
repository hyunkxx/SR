#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube : public CVIBuffer
{
private:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube(const CCube& rhs);
	virtual ~CCube();
public://CComponent
	virtual _int		Update_Component(const _float& fTimeDelta);
	virtual void		LateUpdate_Component(void) {}
	virtual CComponent* Clone(void) override;
	virtual void		Free(void) override;
public://CViBuffer
	virtual HRESULT		Ready_Buffer(void) override;
public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static void	  SetColor(CCube* pCube, D3DXCOLOR color);
	void		  SetColor(D3DXCOLOR color);
	void		  Get_Color(D3DXCOLOR* color);
public:
	void Initalize(CUBE_DATA refData);
public:
	void SetWorldMatrix(const _matrix matWorld) { m_matWorld = matWorld; };
	const _matrix& GetWorld() const { return m_matWorld; }
	_matrix GetWorldMatix() { return m_matWorld; }

	const CUBE_DATA GetData() const { return m_tagData; }

	void SetData(const CUBE_DATA& tagData) { m_tagData = tagData; }

	_vec3& GetRotation() { return m_tagData.rotation; }
	void SetRotation(_vec3 vRotation) { m_tagData.rotation = vRotation; }

	_vec3& GetScale() { return m_tagData.scale; }
	void SetScale(_vec3 vScale) { m_tagData.scale = vScale; }

	_vec3& GetPosition() { return m_tagData.position; }
	void SetPosition(_vec3 vPosition) { m_tagData.position = vPosition; }
private:
	_matrix m_matWorld;
	CUBE_DATA m_tagData;
};

END