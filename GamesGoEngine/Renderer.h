#pragma once

#include "Component.h"
#include "Material.h"
#include "MeshBase.h"

class Renderer : public Component
{
public:
	virtual void Update() override;
	virtual void Draw();

protected:
	void CleanMaterialList();

	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
};