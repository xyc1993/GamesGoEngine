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
	// Draw using provided material, can be used for debug shaders
	virtual void Draw(Material* material);
	bool TryGetMaterial(std::shared_ptr<Material>& outMaterial, int index);

protected:
	void CleanMaterialList();
	virtual glm::vec3 GetCameraPosition() const;

	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
};