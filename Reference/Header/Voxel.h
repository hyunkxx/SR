#pragma once

#include "Component.h"
#include "Include.h"

#include "Cube.h"

BEGIN(Engine)

class ENGINE_DLL CVoxel : public CComponent
{
	enum class Axis { X, Y, Z, MAX };
public:
	explicit CVoxel(LPDIRECT3DDEVICE9 pDevice);
	explicit CVoxel(const CVoxel& rhs);
	virtual ~CVoxel();
public://Component
	virtual _int		Update_Component(const _float& fTimeDelta);
	virtual void		LateUpdate_Component(void);
	virtual CComponent*	Clone(void) override;
	virtual void		Free(void)	override;

	void SetColor(D3DXCOLOR color);
public:
	static CVoxel*		Create(LPDIRECT3DDEVICE9 pDevice, wstring key);
public:
	HRESULT Initalize(wstring key);
	void	Render(const _matrix* matWorld);
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(_uint nIndex);
	LPDIRECT3DINDEXBUFFER9  GetIndexBuffer(_uint nIndex);
private:
	_matrix matWorld; // ºÎ¸ð
	vector<CCube*> m_vecCube;
};

END
