#include "Component.h"

void Component::Init(GameObject* owner)
{
	this->owner = owner;
}