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
		GEAR_API void queueModels(std::vector<ModelInstance>* models);
		GEAR_API void queueParticles(std::vector<ParticleSystem*>* particles);
		GEAR_API void drawParticle(Camera* camera);
	private:
		std::vector<staticNonModels*> statModels;

		/*std::vector<ModelInstance>* staticModels;
		std::vector<ModelInstance> defaultModelList = std::vector<ModelInstance>(0);*/

		RenderQueue queue;	
	/*	std::vector<ModelInstance>* dynamicModels;*/
		std::vector<AnimatedInstance>* animatedModels;
		std::vector<ParticleSystem*>* particleSystems;
	};
}