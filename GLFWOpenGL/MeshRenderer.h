#pragma once

#include "Component.h"

/*
 * 'MeshRenderer' must have an owner, otherwise it won't work as it's not intended to be used as an independent object
 * It is a class responsible for rendering the meshes on screen based on owner's data & its own data
 */
class MeshRenderer : public Component
{
public:
	//~MeshRenderer() override;
	void Init(GameObject* owner) override;
	void Update() override;
};