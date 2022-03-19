#pragma once

#include "Component.h"
#include "Material.h"
#include "MeshBase.h"

class Skybox : public Component
{
public:
	Skybox();
	~Skybox();

	void Update() override;
	void Draw();
	void SetMaterial(const std::shared_ptr<Material>& material);

private:
	std::shared_ptr<Material> material;
	MeshBase* mesh;
};