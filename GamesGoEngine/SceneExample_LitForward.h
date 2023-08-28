#pragma once

#include "SceneExample.h"

namespace GamesGoEngine
{
	class SceneExample_LitForward : public SceneExample
	{
	public:
		SceneExample_LitForward();
		SceneExample_LitForward(bool shouldAddPostProcessEffects);

	private:
		void InitScene(bool shouldAddPostProcessEffects);
	};
}