#include "MeshLoader.h"
#include "GraphicDev.h"
#include "Cube.h"

USING(Engine)

IMPLEMENT_SINGLETON(CMeshLoader)

HWND g_hWnd;

CMeshLoader::CMeshLoader()
{
	strFilePath = L"../Bin/Resource/Mesh/";
}

CMeshLoader::~CMeshLoader()
{
	auto iter = mapMeshData.begin();
	for (; iter != mapMeshData.end(); ++iter)
	{
		for (auto pCube = iter->second.begin(); pCube != iter->second.end(); ++pCube)
		{
			Safe_Release(*pCube);
		}
	}
}

void CMeshLoader::Initalize()
{
	//기본적으로 ../Bin/Resource/Mesh/ 로 설정됨
	//Add Mesh... [파일명(경로)] [키값]

	//Humvee 
	LoadMesh(L"Humvee_ally_body.bin", L"Humvee_ally_body");
	LoadMesh(L"Humvee_ally_head.bin", L"Humvee_ally_head");
	LoadMesh(L"Humvee_ally_posin.bin", L"Humvee_ally_posin");
	LoadMesh(L"Humvee_enemy_body.bin", L"Humvee_enemy_body");
	LoadMesh(L"Humvee_enemy_head.bin", L"Humvee_enemy_head");
	LoadMesh(L"Humvee_enemy_posin.bin", L"Humvee_enemy_posin");

	//Small
	LoadMesh(L"Small_ally_body.bin", L"Small_ally_body");
	LoadMesh(L"Small_ally_head.bin", L"Small_ally_head");
	LoadMesh(L"Small_ally_posin.bin", L"Small_ally_posin");
	LoadMesh(L"Small_enemy_body.bin", L"Small_enemy_body");
	LoadMesh(L"Small_enemy_head.bin", L"Small_enemy_head");
	LoadMesh(L"Small_enemy_posin.bin", L"Small_enemy_posin");

	//Middle
	LoadMesh(L"Middle_ally_body.bin", L"Middle_ally_body");
	LoadMesh(L"Middle_ally_head.bin", L"Middle_ally_head");
	LoadMesh(L"Middle_ally_posin.bin", L"Middle_ally_posin");
	LoadMesh(L"Middle_enemy_body.bin", L"Middle_enemy_body");
	LoadMesh(L"Middle_enemy_head.bin", L"Middle_enemy_head");
	LoadMesh(L"Middle_enemy_posin.bin", L"Middle_enemy_posin");

	//Big
	LoadMesh(L"Big_ally_body.bin", L"Big_ally_body");
	LoadMesh(L"Big_ally_head.bin", L"Big_ally_head");
	LoadMesh(L"Big_ally_posin.bin", L"Big_ally_posin");
	LoadMesh(L"Big_enemy_body.bin", L"Big_enemy_body");
	LoadMesh(L"Big_enemy_head.bin", L"Big_enemy_head");
	LoadMesh(L"Big_enemy_posin.bin", L"Big_enemy_posin");

	//Long
	LoadMesh(L"Long_ally_body.bin", L"Long_ally_body");
	LoadMesh(L"Long_ally_head.bin", L"Long_ally_head");
	LoadMesh(L"Long_ally_posin.bin", L"Long_ally_posin");
	LoadMesh(L"Long_enemy_body.bin", L"Long_enemy_body");
	LoadMesh(L"Long_enemy_head.bin", L"Long_enemy_head");
	LoadMesh(L"Long_enemy_posin.bin", L"Long_enemy_posin");

	//RUSH_TANK
	LoadMesh(L"M1_body.bin", L"M1_body");
	LoadMesh(L"M1_head.bin", L"M1_head");
	LoadMesh(L"M1_posin.bin", L"M1_posin");
	LoadMesh(L"M1_enemy_body.bin", L"M1_enemy_body");
	LoadMesh(L"M1_enemy_head.bin", L"M1_enemy_head");
	LoadMesh(L"M1_enemy_posin.bin", L"M1_enemy_posin");

	//Boss
	LoadMesh(L"Boss_body.bin", L"Boss_body");
	LoadMesh(L"Boss_head.bin", L"Boss_head");
	LoadMesh(L"Boss_posin.bin", L"Boss_posin");


	//Turret
	LoadMesh(L"Turret_body.bin", L"Turret_body");
	LoadMesh(L"Turret_head.bin", L"Turret_head");
	LoadMesh(L"Turret_posin.bin", L"Turret_posin");
	//BattleShip
	LoadMesh(L"Ham_body.bin", L"Ham_body");
	LoadMesh(L"Ham_head.bin", L"Ham_head");
	LoadMesh(L"Ham_posin.bin", L"Ham_posin");
	LoadMesh(L"Ham_head.bin", L"Ham_head2");
	LoadMesh(L"Ham_posin.bin", L"Ham_posin2");

	//AH_64A
	LoadMesh(L"AH_64A_body.bin", L"AH_64A_body");
	LoadMesh(L"AH_64A_enemy_body.bin", L"AH_64A_enemy_body");
	LoadMesh(L"AH_64A_head.bin", L"AH_64A_head");
	LoadMesh(L"AH_64A_object.bin", L"AH_64A_object");

	//Bomber
	LoadMesh(L"air_ally_body.bin", L"air_ally_body");
	//Boom
	LoadMesh(L"Boom_ally_body.bin", L"Boom_ally_body");

	/* Plant */
	LoadMesh(L"Cactus_body.bin", L"Plant_1");
	LoadMesh(L"Eucalyptus_body.bin", L"Plant_2");
	LoadMesh(L"WhiteCactus_body.bin", L"Plant_3");

	/* Building */
	LoadMesh(L"Oasis_body.bin", L"Oasis");

	LoadMesh(L"Building_000_object.bin", L"Building_000_object"); //정상
	LoadMesh(L"Building_100_object.bin", L"Building_100_object"); //파괴

	LoadMesh(L"Building_001_object.bin", L"Building_001_object");

	/* base */
	LoadMesh(L"ally_base_object.bin", L"Base_ally");
	LoadMesh(L"enemy_base_object.bin", L"Base_enemy");

	/* 기존 건물 */
	LoadMesh(L"Building_01_object.bin", L"Building_01_object");
	//LoadMesh(L"Building_001_object.bin", L"Building_001_object");
	LoadMesh(L"Rock_object.bin", L"Rock_object");
	LoadMesh(L"Building_object.bin", L"Building_object");
}

void CMeshLoader::LoadMesh(wstring strFileName, wstring key)
{
	wstring strFullPath = strFilePath + strFileName;
	HANDLE hRead = CreateFileW(strFullPath.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hRead == INVALID_HANDLE_VALUE)
	{
		wstring msg = strFileName + L" 파일이 없습니다.";
		MessageBox(g_hWnd, msg.c_str(), L"mesh", MB_OK);
		return;
	}

	char buf[100];
	ZeroMemory(&buf, sizeof(buf));

	DWORD dwRead = 0;

	char Buff[4];
	int nCubeCount;
	ZeroMemory(&Buff, sizeof(Buff));
	ReadFile(hRead, Buff, sizeof(int), &dwRead, NULL);
	nCubeCount = static_cast<int>(*Buff);

	vector<CCube*> vecCube;
	vecCube.reserve(20);

	for (int i = 0; i < nCubeCount; ++i)
	{
		CUBE_DATA tagData;
		ZeroMemory(&tagData, sizeof(CUBE_DATA));

		ReadFile(hRead, buf, sizeof(_vec3), &dwRead, NULL);
		memcpy(&tagData.scale, &buf, sizeof(_vec3));

		ReadFile(hRead, buf, sizeof(_vec3), &dwRead, NULL);
		memcpy(&tagData.rotation, &buf, sizeof(_vec3));

		ReadFile(hRead, buf, sizeof(_vec3), &dwRead, NULL);
		memcpy(&tagData.position, &buf, sizeof(_vec3));

		ReadFile(hRead, buf, sizeof(D3DXCOLOR), &dwRead, NULL);
		memcpy(&tagData.originColor, &buf, sizeof(D3DXCOLOR));

		CCube* pCube = CCube::Create(Engine::CGraphicDev::GetInstance()->Get_GraphicDev());

		pCube->Initalize(tagData);

		vecCube.push_back(pCube);
	}

	mapMeshData.insert({ key, vecCube });
	CloseHandle(hRead);
}

vector<CCube*> CMeshLoader::GetMeshData(wstring key)
{
	vector<CCube*> mesh;
	mesh.clear();

	auto iter = mapMeshData.begin();
	for (; iter != mapMeshData.end(); ++iter)
	{
		if (iter->first == key)
		{
			mesh = iter->second;
			break;
		}
	}

	if (iter == mapMeshData.end())
	{
		MSG_BOX("해당 키값이 존재하지 않음 =ㅅ=");
	}

	return mesh;
}

bool CMeshLoader::HasValue(wstring key)
{
	auto iter = mapMeshData.begin();
	for (; iter != mapMeshData.end(); ++iter)
	{
		if (iter->first == key)
		{
			break;
		}
	}

	if (iter == mapMeshData.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CMeshLoader::KeyConflictCheack(wstring key)
{
	auto iter = mapMeshData.begin();
	for (; iter != mapMeshData.end(); ++iter)
	{
		if (iter->first.compare(key))
		{
			MSG_BOX("메쉬 키값 충돌 =ㅅ=");
			return true;
		}
	}

	return false;
}