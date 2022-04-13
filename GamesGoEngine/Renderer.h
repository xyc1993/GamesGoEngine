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
	virtual glm::mat4 GetViewMatrix() const;
	virtual glm::mat4 GetProjectionMatrix() const;

	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
};