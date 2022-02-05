#pragma once

#include "Component.h"
#include "Material.h"
#include "MeshBase.h"
#include "RenderingManager.h"

/*
 * 'MeshRenderer' must have an owner, otherwise it won't work as it's not intended to be used as an independent object
 * It is a class responsible for rendering the meshes on screen based on owner's data & its own data
 */
class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer() override;
	
	virtual void Update() override;
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	void Draw();

	void SetMaterial(const std::shared_ptr<Material>& material);
	void SetMaterial(const std::shared_ptr<Material>& material, size_t materialIndex);
	void SetMesh(const std::shared_ptr<MeshBase>& mesh);
	size_t GetMaterialSlotsNumber() const;
	
	void SetRenderQueuePosition(RenderQueuePosition position, bool selected = false);
	void SetRenderQueuePosition(int position, bool selected = false);
	int GetRenderQueuePosition() const;

private:
	void CleanMaterialList();
	
	std::shared_ptr<MeshBase> mesh;
	std::vector<std::shared_ptr<Material>> materialList;
	// used for outlining selected game objects
	Material* editorOutlineMaterial;
	// used to determine position in the render queue
	int currentRenderQueuePosition;
	// position in the render queue when the game object is not selected in the editor
	int deselectedRenderQueuePosition;
};