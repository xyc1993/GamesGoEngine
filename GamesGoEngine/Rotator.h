#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

namespace GamesGoEngine
{
	/*
	 * Test class used for testing rotation change
	 */
	class Rotator : public Component
	{
	public:
		Rotator();

		virtual void Update() override;

		// rotation speed expressed in degrees per second
		void SetSpeed(glm::vec3 speed);

	protected:
		virtual void InitMetaData() override;

	private:
		glm::quat identityQuat;
		// rotation expressed in Euler angles
		glm::vec3 speed;
	};
}