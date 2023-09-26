#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

namespace GamesGoEngine
{
	/*
	 * Test class used for testing position change
	 */
	class PositionOscillator : public Component
	{
	public:
		virtual void Init(GameObject* owner) override;
		virtual void Update() override;

		void SetAmplitude(glm::vec3 amplitude);
		void SetSpeed(float speed);

	protected:
		virtual void InitMetaData() override;

	private:
		glm::vec3 startPosition;
		glm::vec3 amplitude;
		float speed;
	};
}