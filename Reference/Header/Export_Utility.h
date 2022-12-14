#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "ComponentMgr.h"
#include "Management.h"
#include "Renderer.h"

#include "Collisionable.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include"Location.h"

// ÅÊÅ© ¸öÃ¼
#include "Tank_Body.h"
#include "Tank_Head.h"
#include "TankPosin.h"
#include "HitPoint.h"

//Æ÷Åº ¸ð¾ç
#include "VIBullet.h"
#include "VIBullet_Re.h"
#include "VISmoke.h"

#include "Transform.h"
#include "Texture.h"
#include "Calculator.h"

#include "Camera.h"
#include "Pipeline.h"
#include "Bullet.h"
#include "Smoke_Bullet.h"
#include "Boom_Bullet.h"

#include "LightMgr.h"
#include "MeshLoader.h"
#include "Effector.h"
#include "ExplosionEffect.h"
#include "EffectPool.h"

BEGIN(Engine)

// 
inline HRESULT			Ready_Prototype(const _tchar* pComponentTag, CComponent* pComponent);
inline CComponent*		Clone_Prototype(const _tchar* pComponentTag);

// management
inline CLayer*			Get_Layer(const _tchar* pLayerTag);
inline CGameObject*		Get_Object(const _tchar* pLayerTag, const _tchar* pObjTag);
inline CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline HRESULT			Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppInstance);
inline HRESULT			Set_Scene(CScene* pScene);
inline 	_int			Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene(void);
inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
inline void				RenderGUI();
inline void				Delete_Object(const _tchar * pLayerTag, const _tchar * pObjTag);

// Renderer
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup(void);
// LightMgr

inline HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo, const _uint& iIndex);

inline void				Release_Utility(void);

//MeshLoader
inline void Initalize();
inline void LoadMesh(wstring strFileName, wstring key);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
