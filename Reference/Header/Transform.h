#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix*		Get_WorldMatrix() { return &m_matWorld; }
	void				Get_WorldMatrix(_matrix* pWorld) const { *pWorld = m_matWorld; }
	void				Get_Info(INFO eType, _vec3* pInfo){	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));	}
	void				Move_Pos(const _vec3* const pDir) { m_vInfo[INFO_POS] += *pDir; }
	void				Move_Look(const _vec3* const pDir) { m_vInfo[INFO_LOOK] += *pDir; }
	void				Rotation(ROTATION eType, const _float& fAngle){	*(((_float*)(&m_vAngle)) + eType) += fAngle; }
	const _float		Get_Angle(ROTATION eType) { return *(((_float*)(&m_vAngle)) + eType); }

	void				Set_WorldMatrix(_matrix* pWorld) { m_matWorld = *pWorld; }

	void				Set_Scale(const _float& fX, const _float& fY, const _float& fZ){	m_vScale = { fX, fY, fZ };	}
	void				Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y= fY;
		m_vInfo[INFO_POS].z = fZ;
	}

	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	void				Reset_Trans(void);

public:
	HRESULT		Ready_Transform(void);
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component(void) override;

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;
};

END