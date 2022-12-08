#include "..\..\Header\Voxel.h"

#include "MeshLoader.h"
#include "Export_Function.h"
#include "Utility.h"

USING(Engine)

CVoxel::CVoxel(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
	m_vecCube.reserve(50);
}

CVoxel::CVoxel(const CVoxel& rhs)
	: CComponent(rhs)
{
	m_vecCube.reserve(50);
}

CVoxel::~CVoxel()
{
}

_int CVoxel::Update_Component(const _float & fTimeDelta)
{
	_vec3 vPos = { matWorld._41,matWorld._42,matWorld._43 };
	if (Utility::Cuilling(m_pGraphicDev, vPos))
		return 0;

	auto iter = m_vecCube.begin();
	for (; iter != m_vecCube.end(); ++iter)
	{
		_matrix matScale, matTrans;
		_matrix matRot[(UINT)Axis::MAX];

		D3DXMatrixScaling(&matScale,
			(*iter)->GetData().scale.x,
			(*iter)->GetData().scale.y,
			(*iter)->GetData().scale.z);

		D3DXMatrixRotationX(&matRot[(UINT)Axis::X], D3DXToRadian((*iter)->GetData().rotation.x));
		D3DXMatrixRotationY(&matRot[(UINT)Axis::Y], D3DXToRadian((*iter)->GetData().rotation.y));
		D3DXMatrixRotationZ(&matRot[(UINT)Axis::Z], D3DXToRadian((*iter)->GetData().rotation.z));

		D3DXMatrixTranslation(&matTrans,
			(*iter)->GetData().position.x,
			(*iter)->GetData().position.y,
			(*iter)->GetData().position.z);

		_matrix matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale * 	matRot[(UINT)Axis::X] * matRot[(UINT)Axis::Y] * matRot[(UINT)Axis::Z] * matTrans;

		(*iter)->SetWorldMatrix(matWorld);
	}
	return _int();
}

void CVoxel::LateUpdate_Component(void)
{
}

CComponent * CVoxel::Clone(void)
{
	return new CVoxel(*this);
}

void CVoxel::Free(void)
{
	__super::Free();
}

void CVoxel::SetColor(D3DXCOLOR color)
{
	auto iter = m_vecCube.begin();
	for (; iter != m_vecCube.end(); ++iter)
	{
		(*iter)->SetColor(color);
	}
}

CVoxel* CVoxel::Create(LPDIRECT3DDEVICE9 pDevice, wstring key)
{
	CVoxel* pInstance = new CVoxel(pDevice);

	if (FAILED(pInstance->Initalize(key)))
	{
		MSG_BOX("Failed CVoxel->Initalize()");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CVoxel::Initalize(wstring key)
{
	m_vecCube = Engine::GetMeshData(key);

	if (m_vecCube.empty())
	{
		MSG_BOX("Mesh Empty");
		return E_FAIL;
	}
	return S_OK;
}

void CVoxel::Render(const _matrix* matParent)
{
	if (m_vecCube.empty())
	{
		MSG_BOX("Failed CVoxel::Render() :  mesh size 0 ");
		return;
	}

	for (auto iter = m_vecCube.begin(); iter != m_vecCube.end(); iter++)
	{
		_matrix matWorld = (*iter)->GetWorld() * (*matParent);

		_vec3 pos = { matWorld._41, matWorld._42, matWorld._43 };
		if (Utility::Cuilling(m_pGraphicDev, pos))
			continue;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		(*iter)->Render_Buffer();
	}
}

LPDIRECT3DVERTEXBUFFER9 CVoxel::GetVertexBuffer(_uint nIndex)
{
	if (nIndex >= m_vecCube.size())
	{
		MSG_BOX("인덱스가 큐브가 보유갯수보다 많습니다. GetVertexBuffer() Fail -> return null");
		return nullptr;
	}

	return m_vecCube[nIndex]->GetVertexBuffer();
}

LPDIRECT3DINDEXBUFFER9 CVoxel::GetIndexBuffer(_uint nIndex)
{
	if (nIndex >= m_vecCube.size())
	{
		MSG_BOX("인덱스가 큐브가 보유갯수보다 많습니다. GetVIndexBuffer() Fail -> return null");
		return nullptr;
	}

	return m_vecCube[nIndex]->GetIndexBuffer();
}