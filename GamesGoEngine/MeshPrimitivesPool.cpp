#include "MeshPrimitivesPool.h"

namespace GamesGoEngine
{
	MeshPrimitivesPool* MeshPrimitivesPool::instance = nullptr;

	MeshPrimitivesPool::MeshPrimitivesPool()
	{

	}

	MeshPrimitivesPool* MeshPrimitivesPool::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new MeshPrimitivesPool();
		}
		return instance;
	}

	std::shared_ptr<MeshPrimitiveCube> MeshPrimitivesPool::GetCubePrimitive()
	{
		return GetInstance()->GetCubePrimitiveInternal();
	}

	std::shared_ptr<MeshPrimitiveQuad> MeshPrimitivesPool::GetQuadPrimitive()
	{
		return GetInstance()->GetQuadPrimitiveInternal();
	}

	std::shared_ptr<MeshPrimitiveSphere> MeshPrimitivesPool::GetSpherePrimitive()
	{
		return GetInstance()->GetSpherePrimitiveInternal();
	}

	std::shared_ptr<MeshPrimitiveCube> MeshPrimitivesPool::GetCubePrimitiveInternal()
	{
		if (cube == nullptr)
		{
			cube = std::make_shared<MeshPrimitiveCube>();
		}
		return cube;
	}

	std::shared_ptr<MeshPrimitiveQuad> MeshPrimitivesPool::GetQuadPrimitiveInternal()
	{
		if (quad == nullptr)
		{
			quad = std::make_shared<MeshPrimitiveQuad>();
		}
		return quad;
	}

	std::shared_ptr<MeshPrimitiveSphere> MeshPrimitivesPool::GetSpherePrimitiveInternal()
	{
		if (sphere == nullptr)
		{
			sphere = std::make_shared<MeshPrimitiveSphere>();
		}
		return sphere;
	}
}