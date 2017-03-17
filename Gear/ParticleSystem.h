#pragma once
#include "BaseIncludes.h"
#include "ParticleEmitter.h"

namespace Gear
{
	class ParticleSystem
	{
	public:
		GEAR_API ParticleSystem();
		GEAR_API ~ParticleSystem();

		GEAR_API void update(const float &dt, glm::vec3 systemPos);
		GEAR_API void addEmitter(ParticleEmitter* emitter);

		glm::vec3 systemPos;
		glm::vec3 systemRot;

		glm::vec3* newEmitterPos;
		glm::vec3 emitterPos;

		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 v3;
		std::vector<Gear::ParticleEmitter*> particleEmitters;
	};
}