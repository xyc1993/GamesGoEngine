#pragma once

#include "MeshPrimitiveBase.h"

namespace GamesGoEngine
{
	class MeshPrimitiveSphere : public MeshPrimitiveBase
	{
	public:
		MeshPrimitiveSphere();

	protected:
		virtual SubMesh* GetPrimitiveSubMesh() override;
		virtual void SetupMesh() override;

		// shared submesh among all of the sphere instances, generated just once
		static SubMesh* sphereSubMesh;

		const int latitudesNumber = 12;
		const int longitudesNumber = 20;
	};
}