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
		GEAR_API void queueParticles(std::vector<ParticleEmitter*>* particleEmitters);
		GEAR_API void draw(Camera* camera);
	private:

		std::vector<ModelInstance>* staticModels;
		std::vector<ModelInstance> defaultModelList = std::vector<ModelInstance>(0);

		RenderQueue queue;	

		std::vector<ParticleEmitter*>* emitters;
	};
}