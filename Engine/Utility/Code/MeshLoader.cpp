#include "MeshLoader.h"
#include "GraphicDev.h"
#include "Cube.h"

USING(Engine)

IMPLEMENT_SINGLETON(CMeshLoader)

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

	//LoadMesh(L"Main_body.bin", L"body");
	LoadMesh(L"Main_body.bin", L"Main_body");
	LoadMesh(L"Main_head.bin", L"Main_head");
	LoadMesh(L"Main_head.bin", L"Main_posin");

	// Tank_01 
	LoadMesh(L"Tank_01_body.bin", L"Tank_01_body");
	LoadMesh(L"Tank_01_head.bin", L"Tank_01_head");
	LoadMesh(L"Tank_01_posin.bin", L"Tank_01_posin");

	// Tank_02 
	LoadMesh(L"Tank_02_body.bin", L"Tank_02_body");
	LoadMesh(L"Tank_02_head.bin", L"Tank_02_head");
	LoadMesh(L"Tank_02_posin.bin", L"Tank_02_posin");

	// Tank_03 
	LoadMesh(L"Tank_03_body.bin", L"Tank_03_body");
	LoadMesh(L"Tank_03_head.bin", L"Tank_03_head");
	LoadMesh(L"Tank_03_posin.bin", L"Tank_03_posin");

	//Humvee
	LoadMesh(L"Humvee_body.bin", L"Humvee_body");
	LoadMesh(L"Humvee_head.bin", L"Humvee_head");
	LoadMesh(L"Humvee_Posin.bin", L"Humvee_posin");

	//Garbage_Car
	LoadMesh(L"Garbage_Car_body.bin", L"Garbage_Car_body");
	LoadMesh(L"Garbage_Car_head.bin", L"Garbage_Car_head");
	LoadMesh(L"Garbage_Car_posin.bin", L"Garbage_Car_posin");

	//Turret
	LoadMesh(L"Turret_body.bin", L"Turret_body");
	LoadMesh(L"Turret_head.bin", L"Turret_head");
	LoadMesh(L"Turret_posin.bin", L"Turret_posin");

	//M3
	LoadMesh(L"M3_body.bin", L"M3_body");
	LoadMesh(L"M3_head.bin", L"M3_head");
	LoadMesh(L"M3_posin.bin", L"M3_posin");

	//Panzer
	LoadMesh(L"Panzer_01_body.bin", L"Panzer_01_body");
	LoadMesh(L"Panzer_01_head.bin", L"Panzer_01_head");
	LoadMesh(L"Panzer_01_posin.bin", L"Panzer_01_posin");

	/* Building */
	LoadMesh(L"Building_01_object.bin", L"Building_01_object");
}

void CMeshLoader::LoadMesh(wstring strFileName, wstring key)
{
	wstring strFullPath = strFilePath + strFileName;
	HANDLE hRead = CreateFileW(strFullPath.c_str(), GENERIC_READ, NULL, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hRead == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("이름으로 된 매쉬파일이 없데 INVALI HANDLE VALUE");
		return;
	}

	//if(KeyConflictCheack(key))
	//{
	//	MSG_BOX("다른 매쉬 키값을 쓰세여");
	//	return;
	//}

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