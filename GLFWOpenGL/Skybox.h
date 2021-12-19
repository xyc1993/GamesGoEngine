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
	void SetMaterial(Material* material);

private:
	Material* material;
	MeshBase* mesh;
};