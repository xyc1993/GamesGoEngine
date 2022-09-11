#pragma once

#include <memory>

#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveQuad.h"
#include "MeshPrimitiveSphere.h"

/* Class responsible for holding references to primitive meshes that might be used by more than one renderer (such as quads for post process renderers) */
class MeshPrimitivesPool
{
private:
	MeshPrimitivesPool();

	static MeshPrimitivesPool* instance;
	static MeshPrimitivesPool* GetInstance();

public:
	static std::shared_ptr<MeshPrimitiveCube> GetCubePrimitive();
	static std::shared_ptr<MeshPrimitiveQuad> GetQuadPrimitive();
	static std::shared_ptr<MeshPrimitiveSphere> GetSpherePrimitive();

private:
	std::shared_ptr<MeshPrimitiveCube> GetCubePrimitiveInternal();
	std::shared_ptr<MeshPrimitiveQuad> GetQuadPrimitiveInternal();
	std::shared_ptr<MeshPrimitiveSphere> GetSpherePrimitiveInternal();

	std::shared_ptr<MeshPrimitiveCube> cube;
	std::shared_ptr<MeshPrimitiveQuad> quad;
	std::shared_ptr<MeshPrimitiveSphere> sphere;
};