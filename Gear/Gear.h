#pragma once
#include "BaseIncludes.h"
#include "Camera.h"
#include "RenderQueue.h"
#include "Inputs.h"
#include "staticNonModels.h"
#include "Material.h"

namespace Gear
{
	class GearEngine {
	public:
		GEAR_API GearEngine();
		GEAR_API ~GearEngine();	
		GEAR_API void queueParticles(std::vector<ParticleSystem*>* particles);
		GEAR_API void drawParticle(Camera* camera);
	private:
		RenderQueue queue;	
		std::vector<ParticleSystem*>* particleSystems;
	};
}