#pragma once

#include "MeshBase.h"

namespace GamesGoEngine
{
	class MeshPrimitiveBase : public MeshBase
	{
	protected:
		void Init();
		virtual SubMesh* GetPrimitiveSubMesh() = 0;
		virtual void SetupMesh() = 0;
	};
}