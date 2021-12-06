#pragma once

#include "Component.h"
#include "Material.h"
#include "MeshBase.h"

/*
 * 'MeshRenderer' must have an owner, otherwise it won't work as it's not intended to be used as an independent object
 * It is a class responsible for rendering the meshes on screen based on owner's data & its own data
 */
class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer() override;

	void Init(GameObject* owner) override;
	void Update() override;
	void SetMaterial(Material* material);
	void SetMaterial(Material* material, size_t materialIndex);
	void SetMesh(MeshBase* mesh);

private:
	void CleanMaterialList();
	
	MeshBase* mesh;
	std::vector<Material*> materialList;
};