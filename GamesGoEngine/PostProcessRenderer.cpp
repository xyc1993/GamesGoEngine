#include "PostProcessRenderer.h"

#include "MeshPrimitiveQuad.h"

PostProcessRenderer::PostProcessRenderer()
{
	mesh = std::make_shared<MeshPrimitiveQuad>();
	materialList.clear();
	materialList.resize(mesh->GetSubMeshesCount());
}

void PostProcessRenderer::SetMaterial(const std::shared_ptr<PostProcessMaterial>& material)
{
	if (materialList[0] != material)
	{
		materialList[0].reset();
		materialList[0] = material;
	}
}