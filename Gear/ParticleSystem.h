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
		GEAR_API ParticleSystem(int n, float life, float speed, float rate, int number);

		GEAR_API std::vector<Gear::ParticleEmitter*> getParticleEmitters();
		GEAR_API void update(const float &dt, int nrOfEmitters);
	/*	void setEmmiterPos(glm::vec3 pos);*/
		GEAR_API void addNewEmitter(std::vector<Gear::ParticleEmitter*> partEmitter);

	private:
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