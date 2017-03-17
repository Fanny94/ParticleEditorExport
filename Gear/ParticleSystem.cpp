#include "ParticleSystem.h"

namespace Gear
{
	ParticleSystem::ParticleSystem()
	{
		systemPos = { 0, 0, 0 };
		newEmitterPos = new glm::vec3[3];
		emitterPos = glm::vec3(0, 0, 0);

		v1 = { 1, 0, 0 };
		v2 = { 0, 1, 0 };
		v3 = { 0, 0, 1 };
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] newEmitterPos;
	}

	GEAR_API void ParticleSystem::update(const float &dt, glm::vec3 systemPos)
	{
		for (int i = 0; i < particleEmitters.size(); i++)
		{
			emitterPos = particleEmitters.at(i)->localPos;

			newEmitterPos[0] = v1 * emitterPos.x;
			newEmitterPos[1] = emitterPos.y * v2;
			newEmitterPos[2] = emitterPos.z * v3;

			emitterPos = systemPos + newEmitterPos[0] + newEmitterPos[1] + newEmitterPos[2];
			
			particleEmitters.at(i)->setEmitterPos(emitterPos);
			particleEmitters.at(i)->update(dt);
		}
	}

	GEAR_API void ParticleSystem::addEmitter(ParticleEmitter* emitter)
	{
		particleEmitters.push_back(emitter);
	}
}
