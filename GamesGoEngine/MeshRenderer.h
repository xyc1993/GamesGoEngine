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

	void Update() override;
	void LateUpdate() override;
	void SetMaterial(const std::shared_ptr<Material>& material);
	void SetMaterial(const std::shared_ptr<Material>& material, size_t materialIndex);
	void SetMesh(const std::shared_ptr<MeshBase>& mesh);
	size_t GetMaterialSlotsNumber() const;

private:
	void CleanMaterialList();
	
	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
	// used for outlining selected game objects
	Material* editorOutlineMaterial;
};