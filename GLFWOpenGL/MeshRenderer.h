#pragma once

#include "Component.h"

class MeshRenderer : public Component
{
public:
	~MeshRenderer() override;
	void Init(GameObject* owner) override;
	void Update() override;
};