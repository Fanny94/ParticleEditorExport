#include "ParticleSystem.h"

namespace Gear
{
	ParticleSystem::ParticleSystem()
	{

	}

	ParticleSystem::ParticleSystem(int n, float life, float speed, float rate, int number)
	{
		particleEmitters.push_back(new Gear::ParticleEmitter(n, life, speed, rate, number));
		systemPos = { 0, 0, 0 };

		newEmitterPos = new glm::vec3[3];
		
		emitterPos = glm::vec3( 0, 0, 0 );

		v1 = { 1, 0, 0 };
		v2 = { 0, 1, 0 };
		v3 = { 0, 0, 1 };
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] newEmitterPos;
	}

	GEAR_API std::vector<Gear::ParticleEmitter*> ParticleSystem::getParticleEmitters()
	{
		return particleEmitters;
	}

	GEAR_API void ParticleSystem::update(const float &dt, int nrOfEmitters)
	{
	
		for (int i = 0; i < particleEmitters.size(); i++)
		{
			emitterPos = particleEmitters.at(i)->localPos;

			newEmitterPos[0] = v1 * emitterPos.x;
			newEmitterPos[1] = emitterPos.y * v2;
			newEmitterPos[2] = emitterPos.z * v3;
			/*newEmitterPos.y = emitterPos.y * v2; 
			newEmitterPos.z = emitterPos.z * v3;*/

			emitterPos = systemPos + newEmitterPos[0] + newEmitterPos[1] + newEmitterPos[2];
			//newEmitterPos[1] += systemPos.y;
			//newEmitterPos[2] += systemPos.z;
			
			particleEmitters.at(i)->setEmitterPos(emitterPos);
			particleEmitters.at(i)->update(dt);
			
		}
	
	}

	//void ParticleSystem::setEmmiterPos(glm::vec3 pos)
	//{
	//	this->position = pos;
	//}

	GEAR_API void ParticleSystem::addNewEmitter(std::vector<ParticleEmitter*> partEmitter)
	{
		particleEmitters = partEmitter;
		
	}

}
