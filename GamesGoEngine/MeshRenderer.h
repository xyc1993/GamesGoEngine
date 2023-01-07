#pragma once

#include "Renderer.h"
#include "RenderingManager.h"

/*
 * 'MeshRenderer' must have an owner, otherwise it won't work as it's not intended to be used as an independent object
 * It is a class responsible for rendering the meshes on screen based on owner's data & its own data
 */
class MeshRenderer : public Renderer
{
public:
	MeshRenderer();

public:
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	virtual void Draw() override;
	virtual void Draw(Material* material) override;

	void SetMaterial(const std::shared_ptr<Material>& material);
	void SetMaterial(const std::shared_ptr<Material>& material, size_t materialIndex);
	void SetMesh(const std::shared_ptr<MeshBase>& mesh);
	size_t GetMaterialSlotsNumber() const;
	
	void SetRenderQueuePosition(RenderQueuePosition position, bool selected = false);
	void SetRenderQueuePosition(int position, bool selected = false);
	int GetRenderQueuePosition() const;
	void SetIsCastingShadow(bool shadowCastingEnabled);
	bool IsCastingShadow() const;
	// currently for simplicity we treat renderers with deferred materials as deferred renderers, renderers with forward materials as forward renderers and so on
	// NO SUPPORT FOR MIXED MATERIALS LISTS YET!
	LightModelType GetLightModelType() const;

private:
	// used to determine position in the render queue
	int currentRenderQueuePosition;
	// position in the render queue when the game object is not selected in the editor
	int deselectedRenderQueuePosition;

	bool isCastingShadow = true;

	friend RenderingManager;
};