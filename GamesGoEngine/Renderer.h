#pragma once

#include "Component.h"
#include "Material.h"
#include "MeshBase.h"

class Renderer : public Component
{
public:
	~Renderer() override;

	virtual void Update() override;
	virtual void Draw();
	bool TryGetMaterial(std::shared_ptr<Material>& outMaterial, int index);

protected:
	void CleanMaterialList();

	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
};