#pragma once

#include "MeshPrimitiveBase.h"

namespace GamesGoEngine
{
	class MeshPrimitiveCube : public MeshPrimitiveBase
	{
	public:
		MeshPrimitiveCube();

	protected:
		virtual SubMesh* GetPrimitiveSubMesh() override;
		virtual void SetupMesh() override;

		// shared submesh among all of the cube instances, generated just once
		static SubMesh* cubeSubMesh;
	};
}